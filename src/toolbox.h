/*
 *  Filename	: toolbox.h
 *  Author	: Kuba Sejdak
 *  Created on	: 08-05-2012
 */

#ifndef TOOLBOX_H_
#define TOOLBOX_H_

#include <iostream>
#include <iomanip>
#include <stdint.h>
using namespace std;

#define DEBUG
#define PLACE_IN_CODE			"FILE: " << __FILE__ << ", LINE: " << __LINE__ << ", "

#ifdef DEBUG
	#define	SHOWMSG(x)		clog << "DEBUG: " << x << endl
	#define PRINTMSG(fmt, ...)	fprintf(stderr, "DEBUG: " fmt "\n", __VA_ARGS__)
	#define SHOWERR(x)		cerr << "ERROR: " << x << endl
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
	#define SHOWVAR(x)		;
	#define SHOWVAR_HEX(x)		;

	#define	SHOWMSG_L(x)		;
	#define	SHOWERR_L(x)		;
	#define SHOWVAR_L(x)		;
	#define SHOWVAR_HEX_L(x)	;
#endif

typedef	uint8_t byte_t;

#endif /* TOOLBOX_H_ */
