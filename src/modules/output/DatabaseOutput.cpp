/*
 * Filename		: DatabaseOutput.cpp
 * Author		: Kuba Sejdak
 * Created on	: 22-08-2012
 */

/* debug */
#define LOCAL_DEBUG
#include <debug.h>

#include "DatabaseOutput.h"

DatabaseOutput::DatabaseOutput()
{
	id = getNextID();
	name = "DatabaseOutput";
	description = "Inserts info about samples into database.";

	db = QSqlDatabase::addDatabase(DB_QT_DRIVER);
	db.setHostName(DB_HOST);
	db.setDatabaseName(DB_NAME);
	db.setUserName(DB_USER);
	db.setPassword(DB_PASS);
}

DatabaseOutput::~DatabaseOutput()
{
}

bool DatabaseOutput::generateOutput(ShellcodeSample *sample)
{
	if(!sample->getInfo()->isShellcodePresent() && SKIP_NONEXPLOIT_OUTPUT)
		return false;

	bool ok = db.open();
	if(ok) {
		QSqlQuery query;
		QSqlQuery seq_query;
		ShellcodeInfo *info = sample->getInfo();

		/* get next id number */
		seq_query.prepare("SELECT nextval('graph_analyzer_sample_id_seq')");
		if(!seq_query.exec()) {
			SHOWERR(seq_query.lastError().databaseText().toStdString());
			return false;
		}
		seq_query.next();
		int id = seq_query.record().value("nextval").toInt();

		/* general sample data */
		query.prepare("INSERT INTO graph_analyzer_sample VALUES (?, ?, ?, ?, ?, ?, ?)");
		query.bindValue(0, id);
		query.bindValue(1, info->getName().c_str());
		query.bindValue(2, info->getExtractedFrom().c_str());
		query.bindValue(3, info->getGraphName().c_str());
		query.bindValue(4, info->getFileType().c_str());
		query.bindValue(5, itos(info->getSize()).c_str());
		query.bindValue(6, itos(info->getCodeOffset()).c_str());
		if(!query.exec()) {
			SHOWERR(query.lastError().databaseText().toStdString());
			return false;
		}

		/* analyze modules specific data */
		multimap<string, map<string, string> *> *t = sample->getInfo()->getTraits();
		multimap<string, map<string, string> *>::iterator it;
		QSqlQuery q;
		string table;
		for(it = t->begin(); it != t->end(); ++it) {
			table = TABLE_PREFIX;
			table += (*it).first;

			q = traitQuery(table, (*it).second, id);
			if(!q.exec()) {
				SHOWERR(q.lastError().databaseText().toStdString());
				return false;
			}
		}
	}

	db.close();
	return true;
}

QSqlQuery DatabaseOutput::traitQuery(string table, map<string, string> *v, int sample_id)
{
	QSqlQuery q;
	string str_q;
	map<string, string>::iterator it;

	/* prepare query */
	str_q = "INSERT INTO ";
	str_q += table;
	str_q += " VALUES (DEFAULT, ";
	for(unsigned i = 0; i < v->size() + 1; ++i) { // +1 because sample_id
		str_q += "?";
		if(i < v->size())
			str_q += ", ";
	}
	str_q += ")";

	/* bind values */
	q.prepare(str_q.c_str());
	q.bindValue(0, sample_id);
	int i = 1;
	for(it = v->begin(); it != v->end(); ++it) {
		q.bindValue(i, (*it).second.c_str());
		++i;
	}

	return q;
}
