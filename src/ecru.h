#ifndef __ECRU_H
#define __ECRU_H

#include <string>
#include <vector>

using namespace std;

namespace ecru {
	const std::string appname = "ecru";
	const std::string appversion = "0.1.0";
	const std::string clientversion = "CLI-ecru/" + appversion;

	void version();

	vector<string> listDirectory(string path);

	string stripString(std::string const& str);

	string stripNewLines(std::string const& str);

	string readFile(std::string const& filename);

	char* generateTmpFile();
};

#endif // __ECRU_H
