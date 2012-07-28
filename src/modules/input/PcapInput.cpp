/*
 * Filename	: PcapInput.cpp
 * Author	: Kuba Sejdak
 * Created on	: 26-07-2012
 */

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
	mkdir("pcap_tmp", S_IRWXU | S_IRWXG | S_IRWXO);
	string tcpflow_cmd = "tcpflow -r ";
	tcpflow_cmd += filename;
	chdir("pcap_tmp");
	stat = system(tcpflow_cmd.c_str());
	if(stat)
		return;

	chdir("..");
	unlink("pcap_tmp/report.xml");

	/* get all files stored in pcap_tmp */
	DIR *dp;
	dirent *de;

	dp = opendir("pcap_tmp");
	if(dp == NULL) {
		rmdir("pcap_tmp");
		return;
	}

	int i = 1;
	ShellcodeSample *s;
	string flow_name;
	while((de = readdir(dp))) {
		if(strcmp(de->d_name, ".") == 0 || strcmp(de->d_name, "..") == 0)
			continue;

		/* create flow name */
		flow_name = filename;
		string postfix = "_flow_";
		postfix += itos(i);
		postfix += ".bin";
		size_t pos = flow_name.find(".pcap");
		if(pos != string::npos)
			flow_name.erase(pos);

		flow_name += postfix;

		/* move file and change name */
		string mv_cmd = "mv pcap_tmp/";
		mv_cmd += de->d_name;
		mv_cmd += " ";
		mv_cmd += flow_name;
		stat = system(mv_cmd.c_str());
		if(stat)
			continue;

		/* read code */
		chmod(flow_name.c_str(), S_IRWXU | S_IRWXG | S_IRWXO);
		fstream file(flow_name.c_str(), fstream::in | fstream::binary);
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
		s->getInfo()->setName(flow_name);
		s->getInfo()->setBaseName(filename);
		s->getInfo()->setFileType(type);
		s->getInfo()->setSize(size);
		s->setCode((byte_t *) buffer);
		samples->push(s);

		/* delete file */
		if(DELETE_FLOW_FILES)
			unlink(flow_name.c_str());

		++i;
	}

	closedir(dp);
	rmdir("pcap_tmp");
}
