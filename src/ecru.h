#ifndef __ECRU_H
#define __ECRU_H

#include <string>
#include <vector>
#include <map>

#include "ecru_version.h"

using namespace std;

namespace ecru {
	const std::string appname = APP_NAME;
	const std::string appversion = APP_VERSION;
	const std::string clientversion = "CLI-ecru/" + appversion;

	void version();

	vector<string> *listDirectory(string &path);

	string stripString(std::string const& str);

	string stripNewLines(std::string const& str);

	string readFile(std::string const& filename);

	/**
	  * Creates a temporary file
	  * @return path of the temporary file
	  */
	char* generateTmpFile();

	/**
	  * Returns path of the config difectory
	  */
	string getConfigDirectory();

	/**
	  * Check if file at the given path is executable
	  *
	  * @return true if executable, false otherwise
	  */
	bool isExecutable(string& path);

	/**
	  * Execute command
	  *
	  * @return exit status of the command
	  */
	int executeCommand(string command, vector<string> args);

	/**
	  * Represent integer as binary
	  */
	string getBinary(int integer);

	/**
	  * Replaces "$keywords" in text with the values from the map
	  */
	string format(string text, map<string,string> keywords);
}

#endif // __ECRU_H
