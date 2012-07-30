/*
 * Filename	: PcapInput.cpp
 * Author	: Kuba Sejdak
 * Created on	: 26-07-2012
 */

/* debug */
#define LOCAL_DEBUG
#include <debug.h>

#include "PcapInput.h"

PcapInput::PcapInput() {
	id = getNextID();
	name = "PcapInput";
	type = "pcap";
	description = "Loads shellcode from pcap files.";
}

PcapInput::~PcapInput() {
}

void PcapInput::loadInput(string filename, queue<ShellcodeSample *> *samples) {
	int stat;
	/* move to pcap_tmp */
	if(!nameExists("pcap_tmp"))
		mkdir("pcap_tmp", S_IRWXU | S_IRWXG | S_IRWXO);
	stat = chdir("pcap_tmp");
	if(stat) {
		SystemLogger::getInstance()->setError(CHANGE_DIR_FAILED);
		SHOWERR_L("chdir failed in PcapInput");
		return;
	}

	/* create flow files */
	string tcpflow_cmd = "tcpflow -r \"";
	if(isRelative(filename)) {
		tcpflow_cmd += "../";
		tcpflow_cmd += filename;
	}
	else
		tcpflow_cmd +=filename;
	tcpflow_cmd += "\"";
	stat = system(tcpflow_cmd.c_str());
	if(stat) {
		SystemLogger::getInstance()->setError(TCPFLOW_FAILED);
		SHOWERR_L("tcpflow cmd failed in PcapInput");
		chdir("..");
		ftw("pcap_tmp", ftw_remove_dir, 0);
		rmdir("pcap_tmp");
		return;
	}

	unlink("report.xml");

	/* get all flow files stored in pcap_tmp */
	DIR *dp;
	dirent *de;

	dp = opendir(".");
	if(dp == NULL) {
		SystemLogger::getInstance()->setError(TCPFLOW_FAILED);
		SHOWERR_L("opendir failed in PcapInput");
		return;
	}

	int i = 1;
	ShellcodeSample *s;
	while((de = readdir(dp))) {
		if(strcmp(de->d_name, ".") == 0 || strcmp(de->d_name, "..") == 0)
			continue;

		/* read code */
		chmod(de->d_name, S_IRWXU | S_IRWXG | S_IRWXO);
		fstream file(de->d_name, fstream::in | fstream::binary);
		if(!file.is_open())
			continue;

		file.seekg(0, ios::end);
		int size = file.tellg();
		file.seekg(0, ios::beg);
		char *buffer = new char[size];

		file.read(buffer, size);
		file.close();

		/* create new sample */
		s = new ShellcodeSample();
		s->getInfo()->setName(de->d_name);
		s->getInfo()->setBaseName(filename);
		s->getInfo()->setFileType(type);
		s->getInfo()->setSize(size);
		s->setCode((byte_t *) buffer);
		samples->push(s);

		unlink(de->d_name);
		++i;
	}

	/* clean and move to SAMPLES_DIR again */
	closedir(dp);
	stat = chdir("..");
	if(stat) {
		SystemLogger::getInstance()->setError(CHANGE_DIR_FAILED);
		SHOWERR_L("chdir failed in PcapInput");
		return;
	}
	rmdir("pcap_tmp");
}

int ftw_remove_dir(const char *fpath, const struct stat *sb, int typeflag) {
	unlink(fpath);

	return 0;
}
