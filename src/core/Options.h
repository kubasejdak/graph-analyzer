/*
 * Filename		: Options.h
 * Author		: Kuba Sejdak
 * Created on	: 16-09-2012
 */

#ifndef OPTIONS_H_
#define OPTIONS_H_

#include <QString>

#ifdef HOME_CONFIG
    #define CONFIG_FILE		"/home/kuba/Projects/graph-analyzer/conf/analyzer.conf"
#else
    #define CONFIG_FILE		"/home/jsejdak/graph-analyzer/conf/analyzer.conf"
#endif

extern QString APP_ROOT_PATH;
extern int EMULATION_STEPS;
extern QString GRAPHS_DIR;
extern bool DELETE_CODE_INSTANTLY;
extern QString DB_QT_DRIVER;
extern QString DB_HOST;
extern QString DB_NAME;
extern QString DB_USER;
extern QString DB_PASS;
extern QString ANA_TABLE_PREFIX;
extern QString OPT_TABLE_PREFIX;
extern QString ID_SEQ_POSTFIX;
extern bool SKIP_NONEXPLOIT_OUTPUT;

#endif /* OPTIONS_H_ */
