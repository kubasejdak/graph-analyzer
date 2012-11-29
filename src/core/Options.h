/*
 * Filename		: Options.h
 * Author		: Kuba Sejdak
 * Created on	: 16-09-2012
 */

#ifndef OPTIONS_H_
#define OPTIONS_H_

#include <QString>

#define CONFIG_FILE         "/var/www/jsejdak/analyzer.conf"

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
extern bool PROTECT_AGAINST_BIG_FILES;
extern int MAX_INPUT_FILE_SIZE;

#endif /* OPTIONS_H_ */
