/*
 * Filename	: debug.h
 * Author	: Kuba Sejdak
 * Created on	: 09-07-2012
 */

#ifndef DEBUG_H_
#define DEBUG_H_

#include <iostream>
#include <iomanip>
#include <cstdio>
using namespace std;

#define GLOBAL_DEBUG

#define PLACE_IN_CODE	"file: " << __FILE__ << ", line: " << __LINE__ << " "

#if defined(GLOBAL_DEBUG) && defined(LOCAL_DEBUG)
	#define	SHOWMSG(x)		clog << "DEBUG: " << x << endl
	#define PRINTMSG(fmt, ...)	fprintf(stderr, "DEBUG: " fmt "\n", __VA_ARGS__)
	#define SHOWERR(x)		cerr << "ERROR: " << x << endl
	#define PRINTERR(fmt, ...)	fprintf(stderr, "ERROR: " fmt "\n", __VA_ARGS__)
	#define SHOWVAR(x)		clog << "DEBUG: " << #x << " = " << x << endl
	#define SHOWVAR_HEX(x)		clog << "DEBUG: " << #x << " = 0x" << hex << x << endl

	#define	SHOWMSG_L(x)		clog << PLACE_IN_CODE << "DEBUG: " << x << endl
	#define	SHOWERR_L(x)		cerr << PLACE_IN_CODE << "ERROR: " << x << endl
	#define SHOWVAR_L(x)		clog << PLACE_IN_CODE << "DEBUG: " << #x << " = " << x << endl
	#define SHOWVAR_HEX_L(x)	clog << PLACE_IN_CODE << "DEBUG: " << #x << " = 0x" << hex << x << endl
#else
	#define	SHOWMSG(x)		;
	#define PRINTMSG(fmt, ...)	;
	#define	SHOWERR(x)		;
	#define PRINTERR(fmt, ...)	;
	#define PRINTMSG(fmt, ...)	;
	#define SHOWVAR(x)		;
	#define SHOWVAR_HEX(x)		;

	#define	SHOWMSG_L(x)		;
	#define	SHOWERR_L(x)		;
	#define SHOWVAR_L(x)		;
	#define SHOWVAR_HEX_L(x)	;
#endif

#endif /* DEBUG_H_ */
