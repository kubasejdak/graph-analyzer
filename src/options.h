/*
 * Filename		: options.h
 * Author		: Kuba Sejdak
 * Created on	: 28-07-2012
 */

#ifndef OPTIONS_H_
#define OPTIONS_H_

/* ===== EmulationSystem ===== */

/* how many steps should EmulationUnit go */
#define	EMULATION_STEPS		1000000

/* where to put graphs and samples */
#define GRAPHS_DIR 			"graphs/"

/* delete sample's code after analysis (memory management issue) */
#define DELETE_CODE_INSTANTLY	1

/* ===== DatabaseOutput ===== */

/* info to connect with database */
#define DB_QT_DRIVER		"QPSQL"
#define DB_HOST				"localhost"
#define DB_NAME				"graph_analyzer_db"
#define DB_USER				"jsejdak"
#define DB_PASS				"poliritiper"
#define TABLE_PREFIX		"graph_analyzer_"
#define ID_SEQ_POSTFIX		"_id_seq"

/* should system skip output for non-explit samples */
#define SKIP_NONEXPLOIT_OUTPUT	true

#endif /* OPTIONS_H_ */
