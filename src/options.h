/*
 * Filename	: options.h
 * Author	: Kuba Sejdak
 * Created on	: 28-07-2012
 */

#ifndef OPTIONS_H_
#define OPTIONS_H_

/* ===== EmulationSystem ===== */

/* how many steps should EmulationUnit go */
#define	EMULATION_STEPS		1000000

/* where to put graphs by default */
#define USE_DEFAULT_GRAPH_DIR	1
#define DEFAULT_GRAPH_DIR 	"graphs/"

/* ===== PcapInput ===== */

/* should system delete generated flow files */
#define DELETE_FLOW_FILES	1

#endif /* OPTIONS_H_ */
