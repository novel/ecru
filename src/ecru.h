#ifndef __ECRU_H
#define __ECRU_H

#include <string>
#include <vector>

using namespace std;

namespace ecru {
	const std::string appname = "ecru";
	const std::string appversion = "0.1";

	void version();

	vector<string> listDirectory(string path);
};

#endif // __ECRU_H
