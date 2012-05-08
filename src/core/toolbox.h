/*
 *  Filename	: toolbox.h
 *  Author		: Kuba Sejdak
 *  Created on	: 08-05-2012
 */

#ifndef TOOLBOX_H_
#define TOOLBOX_H_

#include <iostream>
#include <iomanip>
using namespace std;

#define DEBUG
#define PLACE_IN_CODE			"FILE: " << __FILE__ << ", LINE: " << __LINE__ << ", "

#ifdef DEBUG
	#define	SHOWMSG(x)			cerr << "DEBUG: " << x << endl
	#define SHOWVAR(x)			cerr << "DEBUG: " << #x << " = " << x << endl
	#define SHOWVAR_HEX(x)		cerr << "DEBUG: " << #x << " = 0x" << hex << x << endl

	#define	SHOWMSG_L(x)		cerr << PLACE_IN_CODE << "DEBUG: " << x << endl
	#define SHOWVAR_L(x)		cerr << PLACE_IN_CODE << "DEBUG: " << #x << " = " << x << endl
	#define SHOWVAR_HEX_L(x)	cerr << PLACE_IN_CODE << "DEBUG: " << #x << " = 0x" << hex << x << endl
#else
	#define	SHOWMSG(x)			//
	#define SHOWVAR(x)			//
	#define SHOWVAR_HEX(x)		//

	#define	SHOWMSG_L(x)		//
	#define SHOWVAR_L(x)		//
	#define SHOWVAR_HEX_L(x)	//
#endif

typedef	unsigned char byte_t;

#endif /* TOOLBOX_H_ */
