#include <fstream>
#include <iostream>
#include <cstdlib>

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>

#include "ecru.h"

void ecru::version()
{
	std::cout << ecru::appname << " " << ecru::appversion << std::endl;
	std::cout << "clientversion (as reported to livejournal): " << ecru::clientversion << std::endl;
	std::cout << "Roman Bogorodskiy <bogorodskiy@gmail.com>" << std::endl << std::endl;
}

/**
  * Get directory listing.
  * @return a list of files in directory as vector of strings
  */
vector<string> ecru::listDirectory(string path)
{
	DIR *dp;
	struct dirent *dirp;
	vector<string> files;

	if ((dp = opendir(path.c_str())) == NULL) {
		cerr << "error opening dir: " << path << endl;
	}

	while ((dirp = readdir(dp)) != NULL) {
		files.push_back(string(dirp->d_name));
	}

	closedir(dp);

	return files;
}

/**
  * Removes whitespaces at the beginning and the end of the line
  * @return a stripped string
  */
string ecru::stripString(std::string const& str)
{
	std::string::size_type const first = str.find_first_not_of(' ');
	return (first == std::string::npos)
			? std::string()
			: str.substr(first, str.find_last_not_of(' ')-first+1);
}

string ecru::stripNewLines(std::string const& str)
{
	string result = str;
	size_t index;
	while ( (index = result.find("\n")) != string::npos) {
		result.replace(index, 1, " ");
	}

	return result;
}

/**
  * Read either file or stdin if filename is "-" to string
  * @return file content as a string
  */
string ecru::readFile(std::string const& filename)
{
	string line;
	string result;

	std::istream *stream;

	if (filename != "-") {
		std::cout << "here " << filename  << std::endl;
		std::ifstream inputStream(filename.c_str());

		if (!inputStream) {
			std::cerr << "omg" << std::endl;
		}

		stream = &inputStream;
	} else {
		stream = &std::cin;
	}
	
	while (getline(*stream, line)) {
		std::cout << "yo" << std::endl;
		result += line + "\n";
	}

	std::cout << stream->bad() << stream->fail() << stream->good() <<stream->eof() << std::endl;

	return result;
}

char* ecru::generateTmpFile()
{
	char *templ = strdup("/tmp/ecruXXXXXXXX");
	
	mkstemp(templ);

	return templ;
}

string ecru::getConfigDirectory()
{
	return string(getenv("HOME")) + "/.ecru/"; 
}

bool ecru::isExecutable(string path)
{
	struct stat *st = (struct stat*)malloc(sizeof(struct stat*));
	stat(path.c_str(), st);

	if (st->st_mode & S_IXUSR) 
		return true;
	else 
		return false;
}
