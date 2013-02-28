/*
 *  Filename	: CoreSystem.cpp
 *  Author		: Kuba Sejdak
 *  Created on	: 01-05-2012
 */

#include "CoreSystem.h"

CoreSystem::CoreSystem()
{
	m_fileCounter = 0;
	m_processedCounter = 0;
	m_sampleCounter = 0;
	m_exploitCounter = 0;
	m_errorCounter = 0;

	if(!readOptions())
		exit(1);

	/* ensure that log file is not too big */
	SystemLogger::instance()->checkFileSize();

	/* add files to analyze from database */
	for(int i = 0; i < Options::instance()->pendingFiles.size(); ++i)
		addFile(Options::instance()->pendingFiles.at(i));

	/* delete added files from database */
	dbRemovePendingFiles();

	loadModules();

    SystemLogger::instance()->setStatus("idle");
    SystemLogger::instance()->clearError();

	/* mandatory */
	setOutput("DatabaseOutput");

    LOG("created CoreSystem instance, version: [%s]\n\n", VERSION);
}

CoreSystem::~CoreSystem()
{
	/* delete all samples */
	clearSamples();
}

int CoreSystem::addFile(QString file)
{
    if(file.isEmpty())
        return 0;

	int addCounter = 0;
    /* check if directory */
    if(QDir(file).exists()) {
		QDirIterator it(file, QDir::Files | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
        while(it.hasNext()) {
            QString entryName = it.next();
            m_pendingFiles.push_back(entryName);
			++addCounter;
			++m_fileCounter;
        }
    }
	else {
        m_pendingFiles.push_back(file);
		++addCounter;
		++m_fileCounter;
	}

	LOG("Added [%d] file(s) extracted from [%s]\n", addCounter, file.toStdString().c_str());
	LOG("SUCCESS\n\n");
	return addCounter;
}

int CoreSystem::run()
{
	SystemLogger::instance()->setStatus("running");

	/* parse all input files */
    LOG("parsing input files\n");
    while(!m_pendingFiles.isEmpty()) {
        QString currentFile = m_pendingFiles.front();
        m_pendingFiles.pop_front();
        LOG("processing file: [%s]\n", currentFile.toStdString().c_str());

		/* load */
        LOG("loading\n");
		int loadError = load(currentFile);
		if(loadError) {
			if(loadError == -1) {
				LOG_ERROR("loading file [%s] -> [%s]\n", currentFile.toStdString().c_str(), error().toStdString().c_str());
				++m_errorCounter;
			}
			else
				LOG_ERROR("loading file [%s] -> [no appropriate input module]\n", currentFile.toStdString().c_str());

			++m_processedCounter;
			continue;
		}

		ExploitSample *s;
        while(!m_samples.isEmpty()) {
            s = m_samples.front();
            m_samples.pop_front();
			++m_sampleCounter;
			LOG("processing sample: [%s]\n", s->info()->name().toStdString().c_str());

			/* emulate */
            LOG("emulating\n");
			if(!emulate(s)) {
                LOG_ERROR("emulating [%s] -> [%s]\n", currentFile.toStdString().c_str(), error().toStdString().c_str());
				++m_errorCounter;
				goto cleanup;
			}

			/* analyze graph */
			LOG("analyzing\n");
			if(Options::instance()->skipEmptySamples && !s->info()->isExploitPresent()) {
				LOG("no exploit found, skipping due to skipEmptySamples: [true]\n");
				goto cleanup;
			}
			if(!analyze(s)) {
                LOG_ERROR("analyzing [%s] -> [%s]\n", currentFile.toStdString().c_str(), error().toStdString().c_str());
				++m_errorCounter;
				goto cleanup;
			}

			/* make output */
            LOG("generating output\n");
			if(Options::instance()->skipBrokenSamples && s->info()->isBroken()) {
				LOG("broken sample, skipping due to skipBrokenSamples: [true]\n");
				goto cleanup;
			}
			if(s->info()->isExploitPresent())
				++m_exploitCounter;
			if(!makeOutput(s)) {
                LOG_ERROR("output for [%s] -> [%s]\n", currentFile.toStdString().c_str(), error().toStdString().c_str());
				++m_errorCounter;
				goto cleanup;
			}

			m_groupManager.processOneSampleGroup(DatabaseManager::instance()->sampleId(s), DatabaseManager::instance()->groupId(s), Options::instance()->resemblenceLevel);

cleanup:
			/* clean up */
			delete s;
            LOG("sample processing finished\n");
		}
		++m_processedCounter;
        LOG("file processing finished\n");
		dbUpdateSystemInfo();
	} /* while */
	if(m_errorCounter)
		LOG("some errors occured, errorCounter: [%d]", m_errorCounter);

	SystemLogger::instance()->setStatus("processing groups");
	dbUpdateSystemInfo();

	m_groupManager.countGroupsMembers();
	m_groupManager.activateUniqueGroups();

	SystemLogger::instance()->setStatus("idle");
	dbUpdateSystemInfo();

	LOG("SUCCESS\n\n");
	return m_errorCounter;
}

void CoreSystem::remakeGroups()
{
	SystemLogger::instance()->setStatus("making groups");
	dbUpdateSystemInfo();

	m_groupManager.processAllSampleGroups(Options::instance()->resemblenceLevel);
	m_groupManager.countGroupsMembers();
	m_groupManager.activateUniqueGroups();

	SystemLogger::instance()->setStatus("idle");
	dbUpdateSystemInfo();

	LOG("SUCCESS\n\n");
}

void CoreSystem::clear()
{
    m_exploitCounter = 0;
    m_sampleCounter = 0;

	clearSamples();
}

QString CoreSystem::status()
{
    return SystemLogger::instance()->status();
}

QString CoreSystem::error()
{
    return SystemLogger::instance()->error();
}

int CoreSystem::progress()
{
	if(m_fileCounter == 0)
		return 0;

	return (m_processedCounter * 100) / m_fileCounter;
}

void CoreSystem::setLogFile(QString file)
{
    SystemLogger::instance()->setLogFile(file);
}

void CoreSystem::setLogLevel(int level)
{
    SystemLogger::instance()->setLogLevel(level);
}

void CoreSystem::setOutput(QString method)
{
    m_outputMethods.push_back(method);
}

int CoreSystem::filesNum()
{
	return m_fileCounter;
}

int CoreSystem::exploitsNum()
{
    return m_exploitCounter;
}

int CoreSystem::samplesNum()
{
    return m_sampleCounter;
}

int CoreSystem::errorsNum()
{
	return m_errorCounter;
}

QString CoreSystem::version()
{
    return VERSION;
}

bool CoreSystem::readOptions()
{
	if(!Options::instance()->readConfigXML()) {
		LOG_ERROR("failed to read XML configuration");
		return false;
	}

	/* logging */
	SystemLogger::instance()->setLogLevel(Options::instance()->logLevel);
	SystemLogger::instance()->setLogFile(Options::instance()->logFile);

	Options::instance()->listOptions();
	return true;
}

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
		updateQuery.addBindValue(error());
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

bool CoreSystem::dbRemovePendingFiles()
{
	/* remove all files */
	if(!DatabaseManager::instance()->clearTable("options_pendingfile"))
		return false;

	return true;
}

int CoreSystem::load(QString file)
{
    QList<ExploitSample *> q;
	ExploitSample *s;
	bool moduleFound = false;

	FileTypeAnalyzer fileAnalyzer;
	QString fileType = fileAnalyzer.analyze(file);
    LOG("fileType: [%s]\n", fileType.toStdString().c_str());

	InputMap::iterator it;
    for(it = m_inputMods->begin(); it != m_inputMods->end(); ++it) {
        if(fileType == it.value()->type()) {
			moduleFound = true;
            bool ok = it.value()->loadInput(file, &q);
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

bool CoreSystem::makeOutput(ExploitSample *s)
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

void CoreSystem::loadModules()
{
    m_inputMods = ModuleManager::instance()->input();
    m_outputMods = ModuleManager::instance()->output();
}

void CoreSystem::clearSamples()
{
    QList<ExploitSample *>::iterator it = m_samples.begin();
    while(it != m_samples.end()) {
        delete *it;
        it = m_samples.erase(it);
	}
}
