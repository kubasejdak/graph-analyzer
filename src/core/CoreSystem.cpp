/*
 *  Filename	: CoreSystem.cpp
 *  Author		: Kuba Sejdak
 *  Created on	: 01-05-2012
 */

#include "CoreSystem.h"

CoreSystem::CoreSystem()
{
    SystemLogger::instance()->setLogFile("/home/kuba/analyzer_log");
    SystemLogger::instance()->setLogLevel(2);

    ConfigFile::instance()->read();
	loadModules();

    SystemLogger::instance()->setStatus("idle");
    SystemLogger::instance()->clearError();

    m_exploitCounter = 0;
    m_sampleCounter = 0;

    LOG("created CoreSystem instance\n\n");
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

    /* check if directory */
    if(QDir(file).exists()) {
        QDirIterator it(file, QDirIterator::Subdirectories);
        while(it.hasNext()) {
            QString entryName = it.next();
            if(QDir(entryName).exists() || entryName == "." || entryName == "..")
                continue;

            m_pendingFiles.push_back(entryName);
        }
    }
    else
        m_pendingFiles.push_back(file);

    return m_pendingFiles.size();
}

void CoreSystem::run()
{
	/* parse all input files */
    LOG("parsing input files\n");
    while(!m_pendingFiles.isEmpty()) {
        QString currentFile = m_pendingFiles.front();
        m_pendingFiles.pop_front();

		/* load */
        LOG("loading\n");
        if(!load(currentFile)) {
            LOG_ERROR("opening file [%s] -> [%s]\n", currentFile.toStdString().c_str(), error().toStdString().c_str());
			continue;
		}

		ShellcodeSample *s;
        while(!m_samples.isEmpty()) {
            s = m_samples.front();
            m_samples.pop_front();

			/* emulate */
            LOG("emulating\n");
			if(!emulate(s)) {
                LOG_ERROR("emulating [%s] -> [%s]\n", currentFile.toStdString().c_str(), error().toStdString().c_str());
				delete s;
				continue;
			}

			/* analyze graph */
            LOG("analyzing\n");
			if(!analyze(s)) {
                LOG_ERROR("analyzing [%s] -> [%s]\n", currentFile.toStdString().c_str(), error().toStdString().c_str());
				delete s;
				continue;
			}

			/* make output */
            LOG("generating output\n");
			if(!makeOutput(s)) {
                LOG_ERROR("output for [%s] -> [%s]\n", currentFile.toStdString().c_str(), error().toStdString().c_str());
				delete s;
				continue;
			}

			/* clean up */
			delete s;
            LOG("sample processing finished\n");
		}
        LOG("file processing finished\n");
	} /* while */
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

int CoreSystem::exploitsNum()
{
    return m_exploitCounter;
}

int CoreSystem::samplesNum()
{
    return m_sampleCounter;
}

QString CoreSystem::version()
{
    return VERSION;
}

bool CoreSystem::load(QString file)
{
    SystemLogger::instance()->setStatus("loading");
    SystemLogger::instance()->clearError();

    QList<ShellcodeSample *> q;
	ShellcodeSample *s;

    FileAnalyser fileAnalyser;
    QString fileType = fileAnalyser.analyze(file);
    LOG("fileType: [%s]\n", fileType.toStdString().c_str());

    QMap<QString, AbstractInput *>::iterator it;
    for(it = m_inputMods->begin(); it != m_inputMods->end(); ++it) {
        if(fileType == it.value()->type()) {
            it.value()->loadInput(file, &q);

			/* process all returned samples */
            while(!q.isEmpty()) {
				s = q.front();
				q.pop_front();
                m_samples.push_back(s);
			}

			break;
		}
	}

    SystemLogger::instance()->setStatus("idle");
    if(!m_samples.isEmpty()) {
        ++m_sampleCounter;
        LOG("SUCCESS\n\n");
		return true;
	}

    SystemLogger::instance()->setError("no appropriate input module");
    LOG_ERROR("no appropriate input module\n");
    LOG_ERROR("FAILUR\n\n");
	return false;
}

bool CoreSystem::emulate(ShellcodeSample *s)
{
    SystemLogger::instance()->setStatus("emulating");
    SystemLogger::instance()->clearError();

    m_emuSystem.loadSample(s);
    bool ret = m_emuSystem.emulate();

    SystemLogger::instance()->setStatus("idle");
	if(!ret) {
        SystemLogger::instance()->setError("general emulation error");
        LOG_ERROR("general emulation error\n");
        LOG_ERROR("FAILURE\n\n");
		return false;
	}

    LOG("SUCCESS\n\n");
	return true;
}

bool CoreSystem::analyze(ShellcodeSample *s)
{
    SystemLogger::instance()->setStatus("analyzing");
    SystemLogger::instance()->clearError();

    m_anaSystem.loadSample(s);
    bool ret = m_anaSystem.analyze();

    SystemLogger::instance()->setStatus("idle");
	if(!ret) {
        SystemLogger::instance()->setError("general analyzing error");
        LOG_ERROR("general analyzing error\n");
        LOG_ERROR("FAILURE\n\n");
		return false;
	}

    if(s->info()->isShellcodePresent())
        ++m_exploitCounter;
    LOG("SUCCESS\n\n");
	return true;
}

bool CoreSystem::makeOutput(ShellcodeSample *s)
{
    SystemLogger::instance()->setStatus("generating output");
    SystemLogger::instance()->clearError();

    QList<QString>::iterator it;
    if(!s->info()->isShellcodePresent()) {
        LOG("no exploit found, returning\n");
        LOG("SUCCESS\n\n");
		return true;
    }

	bool ret;
    for(it = m_outputMethods.begin(); it != m_outputMethods.end(); ++it) {
        ret = (*m_outputMods)[*it]->generateOutput(s);

        SystemLogger::instance()->setStatus("idle");
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
    QList<ShellcodeSample *>::iterator it = m_samples.begin();
    while(it != m_samples.end()) {
        delete *it;
        it = m_samples.erase(it);
	}
}
