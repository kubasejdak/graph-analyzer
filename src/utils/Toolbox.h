/*
 * Filename		: Toolbox.h
 * Author		: Kuba Sejdak
 * Created on	: 09-07-2012
 */

#ifndef TOOLBOX_H_
#define TOOLBOX_H_

#include <cstdint>
#include <string>

typedef uint8_t byte_t;

class Toolbox {
public:
	static std::string itos(int n, int base = 10);
	static std::string hash(std::string data);

	// paths
	static std::string makeRelative(std::string path);
	static std::string removeSlash(std::string path);
	static std::string pcapFlowBasename(std::string path);
	static bool removeDirectory(std::string path);
	static std::string formatToString(const std::string fmt...);
};

#endif /* TOOLBOX_H_ */
