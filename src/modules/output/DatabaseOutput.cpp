/*
 * Filename		: DatabaseOutput.cpp
 * Author		: Kuba Sejdak
 * Created on	: 22-08-2012
 */

#include "DatabaseOutput.h"

DatabaseOutput::DatabaseOutput()
{
    m_name = "DatabaseOutput";
    m_description = "Inserts info about samples into database.";
}

bool DatabaseOutput::generateOutput(ShellcodeSample *sample)
{
    if(!sample->info()->isShellcodePresent() && SKIP_NONEXPLOIT_OUTPUT) {
        LOG("no exploit found, returning\n");
        LOG("SUCCESS\n\n");
        return true;
    }

    ShellcodeInfo *info = sample->info();

    /* ensure that sample is not a duplicate */
    bool duplicate = checkDuplicate(info);
    if(duplicate) {
        LOG("duplicate sample: skipping and removing duplicated graph file\n");
        QFile file(info->graphName());
        file.remove();
        LOG("SUCCESS\n\n");
        return true;
    }

    /* get next id number */
    QSqlQuery seq_query(DatabaseManager::instance()->database());
    seq_query.prepare("SELECT nextval('analyze_sample_id_seq')");
    if(!DatabaseManager::instance()->exec(&seq_query)) {
        SystemLogger::instance()->setError(DatabaseManager::instance()->lastError());
        LOG_ERROR("%s\n", DatabaseManager::instance()->lastError().toStdString().c_str());
        LOG_ERROR("FAILURE\n\n");
        return false;
    }

    seq_query.next();
    int id = seq_query.record().value("nextval").toInt();

    /* general sample data */
    QSqlQuery sample_query(DatabaseManager::instance()->database());
	sample_query.prepare("INSERT INTO analyze_sample VALUES (?, ?, ?, ?, ?, ?, ?, ?)");
    sample_query.bindValue(0, id);
    sample_query.bindValue(1, info->name());
    sample_query.bindValue(2, info->extractedFrom());
    sample_query.bindValue(3, info->graphName());
    sample_query.bindValue(4, info->fileType());
    sample_query.bindValue(5, QString().setNum(info->size()));
    sample_query.bindValue(6, QString().setNum(info->codeOffset()));
	sample_query.bindValue(7, "");
    if(!DatabaseManager::instance()->exec(&sample_query)) {
        SystemLogger::instance()->setError(DatabaseManager::instance()->lastError());
        LOG_ERROR("%s\n", DatabaseManager::instance()->lastError().toStdString().c_str());
        LOG_ERROR("FAILURE\n\n");
        return false;
    }

    /* analyze modules specific data */
    QMultiMap<QString, QMap<QString, QString> *> *t = sample->info()->traits();
    QMultiMap<QString, QMap<QString, QString> *>::iterator it;
    QSqlQuery mod_query(DatabaseManager::instance()->database());
    QString table;
    for(it = t->begin(); it != t->end(); ++it) {
        table = ANA_TABLE_PREFIX;
        table += it.key();

        traitQuery(&mod_query, table, it.value(), id);
        if(!DatabaseManager::instance()->exec(&mod_query)) {
            SystemLogger::instance()->setError(DatabaseManager::instance()->lastError());
            LOG_ERROR("%s\n", DatabaseManager::instance()->lastError().toStdString().c_str());
            LOG_ERROR("FAILURE\n\n");
            return false;
        }
    }

    LOG("SUCCESS\n\n");
	return true;
}

void DatabaseOutput::traitQuery(QSqlQuery *query, QString table, QMap<QString, QString> *v, int sample_id)
{
    QMap<QString, QString>::iterator it;

	/* prepare query */
    QString str_q = QString("INSERT INTO %1 VALUES (DEFAULT, ").arg(table);
    for(int i = 0; i < v->size() + 1; ++i) { // +1 because sample_id
		str_q += "?";
		if(i < v->size())
			str_q += ", ";
	}
	str_q += ")";

	/* bind values */
    query->prepare(str_q);
    query->bindValue(0, sample_id);
	int i = 1;
	for(it = v->begin(); it != v->end(); ++it) {
        query->bindValue(i, it.value());
		++i;
	}
}

bool DatabaseOutput::checkDuplicate(ShellcodeInfo *info)
{
    /* check sample */
    QSqlQuery select_query(DatabaseManager::instance()->database());
    select_query.prepare("SELECT * FROM analyze_sample WHERE name = ? AND extracted_from = ? AND file_size = ? AND shellcode_offset = ?");
    select_query.addBindValue(info->name());
    select_query.addBindValue(info->extractedFrom());
    select_query.addBindValue(info->size());
    select_query.addBindValue(info->codeOffset());
    if(!DatabaseManager::instance()->exec(&select_query)) {
        SystemLogger::instance()->setError(DatabaseManager::instance()->lastError());
        LOG_ERROR("%s\n", DatabaseManager::instance()->lastError().toStdString().c_str());
        LOG_ERROR("FAILURE\n\n");
        return false;
    }

    LOG("SUCCESS\n\n");
    return select_query.next();
}
