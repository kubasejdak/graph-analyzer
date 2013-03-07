/*
 *  Filename	: CoreSystem.cpp
 *  Author		: Kuba Sejdak
 *  Created on	: 01-05-2012
 */

#include "CoreSystem.h"

CoreSystem::CoreSystem()
{
	/* read options */
	if(!readOptions())
		exit(1);

	/* ensure that log file is not too big */
	SystemLogger::instance()->checkFileSize();

	loadModules();

    SystemLogger::instance()->setStatus("idle");
    SystemLogger::instance()->clearError();

    LOG("created CoreSystem instance, version: [%s]\n\n", VERSION);
}

CoreSystem::~CoreSystem()
{
	/* delete all samples */
	clearSamples();
}

bool CoreSystem::execute(QString funcName)
{
	RPCTag func = m_tagResolver.tag(funcName);
	bool stat = false;
	LOG("Calling RPC function: [%s]\n", funcName.toStdString().c_str());

	switch(func) {
	case ANALYZE_TAG:
		stat = rpcAnalyze();
		break;
	case EXPERIMENT_TAG:
		stat = rpcExperiment();
		break;
	case EXPORT_VERSION_TAG:
		stat = rpcExportVersion();
		break;
	case NULL_TAG:
		LOG_ERROR("Function [%s] is not supported\n", funcName.toStdString().c_str());
		break;
	}

	return stat;
}

QString CoreSystem::status()
{
	return SystemLogger::instance()->status();
}

QString CoreSystem::lastError()
{
	return SystemLogger::instance()->error();
}

QString CoreSystem::version()
{
	return VERSION;
}

bool CoreSystem::rpcAnalyze()
{
	SystemLogger::instance()->setStatus("analyze");

	/* read files to analyze */
	if(readAnalyzeTaskXML() == false) {
		SystemLogger::instance()->setError("cannot parse tasks file");
		SystemLogger::instance()->setStatus("idle");
		LOG_ERROR("cannot parse tasks file\n");
		LOG_ERROR("FAILUR\n\n");
		return false;
	}

	int fileCounter;			/* number of files extracted to analyze */
	int processedCounter;		/* number of processed files */
	int sampleCounter;			/* number of files that were loaded */
	int exploitCounter;			/* number of detected exploits */
	int errorCounter;			/* number od errors */

	/* scan scheduled files for folders */
	fileCounter = addScheduledFiles();

	/* analyze all task files */
	LOG("analyzing task files\n");
	while(!m_taskFiles.isEmpty()) {
		QString currentFile = m_taskFiles.front();
		m_taskFiles.pop_front();
		LOG("processing file: [%s]\n", currentFile.toStdString().c_str());

		/* load */
		LOG("loading\n");
		int loadError = load(currentFile);
		if(loadError) {
			if(loadError == -1) {
				LOG_ERROR("loading file [%s] -> [%s]\n", currentFile.toStdString().c_str(), lastError().toStdString().c_str());
				++errorCounter;
			}
			else
				LOG_ERROR("loading file [%s] -> [no appropriate input module]\n", currentFile.toStdString().c_str());

			++processedCounter;
			continue;
		}

		ExploitSample *s;
		while(!m_samples.isEmpty()) {
			s = m_samples.front();
			m_samples.pop_front();
			++sampleCounter;
			LOG("processing sample: [%s]\n", s->info()->name().toStdString().c_str());

			/* emulate */
			LOG("emulating\n");
			if(emulate(s) == false) {
				LOG_ERROR("emulating [%s] -> [%s]\n", currentFile.toStdString().c_str(), lastError().toStdString().c_str());
				++errorCounter;
				goto cleanup;
			}

			/* analyze graph */
			LOG("analyzing\n");
			if(Options::instance()->skipEmptySamples && !s->info()->isExploitPresent()) {
				LOG("no exploit found, skipping due to skipEmptySamples: [true]\n");
				goto cleanup;
			}
			if(analyze(s) == false) {
				LOG_ERROR("analyzing [%s] -> [%s]\n", currentFile.toStdString().c_str(), lastError().toStdString().c_str());
				++errorCounter;
				goto cleanup;
			}

			/* export results */
			LOG("exporting results\n");
			if(Options::instance()->skipBrokenSamples && s->info()->isBroken()) {
				LOG("broken sample, skipping due to skipBrokenSamples: [true]\n");
				goto cleanup;
			}
			if(s->info()->isExploitPresent())
				++exploitCounter;
			if(exportResults(s) == false) {
				LOG_ERROR("output for [%s] -> [%s]\n", currentFile.toStdString().c_str(), lastError().toStdString().c_str());
				++errorCounter;
				goto cleanup;
			}

cleanup:
			/* clean up */
			delete s;
			LOG("sample analyzing finished\n");
			++sampleCounter;
		}
		++processedCounter;
		LOG("file analyzing finished\n");
	} /* while */

	/* summarize */
	LOG("FINISHED: found %d exploit(s) in %d sample(s) extracted from %d file(s)!\n", exploitCounter, sampleCounter, fileCounter);
	if(errorCounter)
		LOG("some ERRORS occured, errorCounter: [%d]\n", errorCounter);
	else
		LOG("no errors occured\n");

	SystemLogger::instance()->setStatus("idle");
	LOG("SUCCESS\n\n");
	return true;
}

bool CoreSystem::rpcExperiment()
{
	SystemLogger::instance()->setStatus("experiment");

	// TODO: implement

	SystemLogger::instance()->setStatus("idle");
	LOG("SUCCESS\n\n");
	return true;
}

bool CoreSystem::rpcExportVersion()
{
	SystemLogger::instance()->setStatus("export version");

	// TODO: implement

	SystemLogger::instance()->setStatus("idle");
	LOG("SUCCESS\n\n");
	return true;
}

int CoreSystem::load(QString filename)
{
	QList<ExploitSample *> q;
	ExploitSample *s;
	bool moduleFound = false;

	FileTypeAnalyzer fileAnalyzer;
	QString fileType = fileAnalyzer.analyze(filename);
	LOG("fileType: [%s]\n", fileType.toStdString().c_str());

	InputMap::iterator it;
	for(it = m_inputMods->begin(); it != m_inputMods->end(); ++it) {
		if(fileType == it.value()->type()) {
			moduleFound = true;
			bool ok = it.value()->loadInput(filename, &q);
			if(!ok) {
				SystemLogger::instance()->setError("loading file failed");
				return -1;
			}

			/* process all returned samples */
			while(!q.isEmpty()) {
				s = q.front();
				q.pop_front();
				m_samples.push_back(s);
			}

			break;
		}
	}

	if(moduleFound) {
		LOG("SUCCESS\n\n");
		return 0;
	}

	LOG_ERROR("no appropriate input module\n");
	LOG_ERROR("FAILUR\n\n");
	return 1;
}

bool CoreSystem::emulate(ExploitSample *s)
{
	m_emuSystem.loadSample(s);
	bool ret = m_emuSystem.emulate();

	if(!ret) {
		SystemLogger::instance()->setError("general emulation error");
		LOG_ERROR("general emulation error\n");
		LOG_ERROR("FAILURE\n\n");
		return false;
	}

	LOG("SUCCESS\n\n");
	return true;
}

bool CoreSystem::analyze(ExploitSample *s)
{
	m_anaSystem.loadSample(s);
	bool ret = m_anaSystem.analyze();

	if(!ret) {
		SystemLogger::instance()->setError("general analyzing error");
		LOG_ERROR("general analyzing error\n");
		LOG_ERROR("FAILURE\n\n");
		return false;
	}

	LOG("SUCCESS\n\n");
	return true;
}

bool CoreSystem::exportResults(ExploitSample *s)
{
	QList<QString>::iterator it;
	if(!s->info()->isExploitPresent()) {
		LOG("no exploit found, returning\n");
		LOG("SUCCESS\n\n");
		return true;
	}

	bool ret;
	for(it = m_outputMethods.begin(); it != m_outputMethods.end(); ++it) {
		ret = (*m_outputMods)[*it]->generateOutput(s);

		if(!ret) {
			SystemLogger::instance()->setError("generating output failed");
			LOG_ERROR("generating output error\n");
			LOG_ERROR("FAILURE\n\n");
			return false;
		}
	}

	LOG("SUCCESS\n\n");
	return true;
}

bool CoreSystem::readOptions()
{
	if(!Options::instance()->readConfigXML()) {
		LOG_ERROR("failed to read XML configuration\n");
		return false;
	}

	/* logging */
	SystemLogger::instance()->setLogLevel(Options::instance()->logLevel);
	SystemLogger::instance()->setLogFile(Options::instance()->logFile);

	Options::instance()->listOptions();
	return true;
}

bool CoreSystem::readAnalyzeTaskXML()
{
	if(!m_xmlParser.open(TASKS_FILE))
		return false;

	if(!m_xmlParser.hasRoot("Analyze")) {
		m_xmlParser.close();
		return false;
	}

	QDomElement analyze = m_xmlParser.root("Analyze");

	while(m_xmlParser.hasChild(analyze, "File")) {
		QDomElement f = m_xmlParser.child(analyze, "File");
		m_scheduledFiles.push_back(f.text());
		m_xmlParser.removeChild(analyze, f);
	}
	m_xmlParser.removeRoot(analyze);

	m_xmlParser.close();
	listScheduledFiles();
	return true;
}

void CoreSystem::listScheduledFiles()
{
	LOG("scheduled files:\n");
	for(int i = 0; i < m_scheduledFiles.size(); ++i)
		LOG("file: [%s]\n", m_scheduledFiles.at(i).toStdString().c_str());
}

int CoreSystem::addScheduledFiles()
{
	int fileCounter = 0;
	for(int i = 0; i < m_scheduledFiles.size(); ++i)
		fileCounter += addFile(m_scheduledFiles.at(i));

	return fileCounter;
}

int CoreSystem::addFile(QString filename)
{
	if(filename.isEmpty())
		make_pair<int, int>(0, 0);

	int addCounter = 0;
	int fileCounter = 0;
	/* check if directory */
	if(QDir(filename).exists()) {
		QDirIterator it(filename, QDir::Files | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
		while(it.hasNext()) {
			QString entryName = it.next();
			m_taskFiles.push_back(entryName);
			++addCounter;
			++fileCounter;
		}
	}
	else {
		m_taskFiles.push_back(filename);
		++addCounter;
		++fileCounter;
	}

	LOG("Added [%d] file(s) extracted from [%s]\n", addCounter, filename.toStdString().c_str());
	LOG("SUCCESS\n\n");
	return addCounter;
}

void CoreSystem::loadModules()
{
	m_inputMods = ModuleManager::instance()->input();
	m_outputMods = ModuleManager::instance()->output();
}

void CoreSystem::clearSamples()
{
	SampleContainer::iterator it = m_samples.begin();
	while(it != m_samples.end()) {
		delete *it;
		it = m_samples.erase(it);
	}
}

#if 0
bool CoreSystem::dbUpdateSystemInfo()
{
	/* check for info */
	QSqlQuery checkQuery(DatabaseManager::instance()->database());
	checkQuery.prepare("SELECT * FROM options_systeminfo");
	if(!DatabaseManager::instance()->exec(&checkQuery))
		return false;

	/* there is system info record in database */
	if(checkQuery.next()) {
		int id = checkQuery.record().value("id").toInt();

		QSqlQuery updateQuery(DatabaseManager::instance()->database());
		updateQuery.prepare("UPDATE options_systeminfo SET version = ?, status = ?, error = ?, progress = ?, exploits_num = ?, samples_num = ?, files_num = ?, errors_num = ? WHERE id = ?");
		updateQuery.addBindValue(version());
		updateQuery.addBindValue(status());
		updateQuery.addBindValue(lastError());
		updateQuery.addBindValue(progress());
		updateQuery.addBindValue(exploitsNum());
		updateQuery.addBindValue(samplesNum());
		updateQuery.addBindValue(filesNum());
		updateQuery.addBindValue(errorsNum());
		updateQuery.addBindValue(id);
		if(!DatabaseManager::instance()->exec(&updateQuery))
			return false;
	}
	/* there is no system info in database */
	else {
		int id = DatabaseManager::instance()->sequenceValue("options_systeminfo_id_seq");

		QSqlQuery insertQuery(DatabaseManager::instance()->database());
		insertQuery.prepare("INSERT INTO options_systeminfo VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?)");
		insertQuery.addBindValue(id);
		insertQuery.addBindValue(version());
		insertQuery.addBindValue(status());
		insertQuery.addBindValue(error());
		insertQuery.addBindValue(progress());
		insertQuery.addBindValue(exploitsNum());
		insertQuery.addBindValue(samplesNum());
		insertQuery.addBindValue(filesNum());
		insertQuery.addBindValue(errorsNum());
		if(!DatabaseManager::instance()->exec(&insertQuery))
			return false;
	}

	return true;
}
#endif
