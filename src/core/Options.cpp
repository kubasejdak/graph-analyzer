/*
 * Filename		: Options.cpp
 * Author		: Kuba Sejdak
 * Created on	: 16-09-2012
 */

#include <core/Options.h>

/* ===== CoreSystem ===== */

/* path to root folder of GraphAnalyzer resources */
string APP_ROOT_PATH;

/* ===== EmulationSystem ===== */

/* how many steps should EmulationUnit go */
int EMULATION_STEPS;

/* where to put graphs and samples */
string GRAPHS_DIR;

/* delete sample's code after analysis (memory management issue) */
bool DELETE_CODE_INSTANTLY;

/* ===== DatabaseOutput ===== */

/* info to connect with database */
string DB_QT_DRIVER;
string DB_HOST;
string DB_NAME;
string DB_USER;
string DB_PASS;
string TABLE_PREFIX;
string ID_SEQ_POSTFIX;

/* should system skip output for non-explit samples */
bool SKIP_NONEXPLOIT_OUTPUT;
