/*
 * Filename		: PcapInput.h
 * Author		: Kuba Sejdak
 * Created on	: 26-07-2012
 */

#ifndef PCAPINPUT_H_
#define PCAPINPUT_H_

/* standard headers */
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <ftw.h>

/* project headers */
#include "AbstractInput.h"
#include <core/SystemLogger.h>
#include <toolbox.h>
#include <options.h>

int ftw_remove_dir(const char *fpath, const struct stat *sb, int typeflag);

class PcapInput: public AbstractInput {
public:
	PcapInput();
	virtual ~PcapInput();

	virtual void loadInput(string filename, list<ShellcodeSample *> *samples);
};

#endif /* PCAPINPUT_H_ */
