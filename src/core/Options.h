/*
 * Filename		: Options.h
 * Author		: Kuba Sejdak
 * Created on	: 16-09-2012
 */

#ifndef OPTIONS_H_
#define OPTIONS_H_

#include <string>
using namespace std;

#define CONFIG_FILE		"/home/kuba/Projects/graph-analyzer/conf/analyzer.conf"

extern string APP_ROOT_PATH;
extern int EMULATION_STEPS;
extern string GRAPHS_DIR;
extern bool DELETE_CODE_INSTANTLY;
extern string DB_QT_DRIVER;
extern string DB_HOST;
extern string DB_NAME;
extern string DB_USER;
extern string DB_PASS;
extern string TABLE_PREFIX;
extern string ID_SEQ_POSTFIX;
extern bool SKIP_NONEXPLOIT_OUTPUT;

#endif /* OPTIONS_H_ */
