#include <fstream>
#include <iostream>
#include <cstdlib>
#include <cstring>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <dirent.h>
#include <unistd.h>
#include <libgen.h>

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
vector<string> *ecru::listDirectory(string &path)
{
	DIR *dp;
	struct dirent *dirp;
	vector<string> *files = new vector<string>;

	if ((dp = opendir(path.c_str())) == NULL) {
		cerr << "error opening dir: " << path << endl;

		// XXX exception
	}

	while ((dirp = readdir(dp)) != NULL) {
		files->push_back(string(dirp->d_name));
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
	struct stat st;
	stat(path.c_str(), &st);

	if (st.st_mode & S_IXUSR) 
		return true;
	else 
		return false;
}

int ecru::executeCommand(string command, vector<string> args)
{
//	cout << "EXECUTING COMMAND " << command << endl;

	pid_t pid = fork();

	if (pid == 0) {
		int ret;
		unsigned int i;

		char **params = (char **)malloc(sizeof(char *) * (args.size() + 2));
		params[0] = basename((char *)command.c_str());
		for (i = 0; i < args.size(); i++) {
			params[i+1] = (char *)args[i].c_str();
		}
		params[i+1] = NULL;		

		ret = execve(command.c_str(), /*argv*/ params, NULL);
		if (ret == -1) {
			perror("execve");
			_exit(1);
		} else
			_exit(0);
	} else if (pid < 0) {
		cerr << "fork failed" << endl;
		exit(1);
	}

	int status;
	wait(&status);

//	cout << "EXEUCTE FINISHED OF COMMAND " << command << " (exit status = " << status << ")" << endl;

	return status;
}

string ecru::getBinary(int integer)
{	
	int length = sizeof(int) * 8;
	string result;

	for (int i = length - 1; i >= 0; i--) {
		if (((integer >> i) & 1) == 1) {
			result += '1';
		} else {
			result += '0';
		}
	}

	return result;
}

string ecru::format(string text, map<string,string> keywords)
{
	unsigned int i = 1;
	while (i < text.length() - 1) {
		if ((text[i] == '$') && (text[i-1] = ' ') &&
				isalpha(text[i+1])) {
			int start = i;
					       
			while (isalpha(text[i+1]) != 0)
				i++;
					        
			string keyword = text.substr(start + 1, i - start);
			map<string, string>::iterator iter = keywords.find(keyword);
			if (iter != keywords.end()) {
				text.replace(start, i - start + 1, keywords[keyword]);
			}
		}
							       
 		i++;
	}

	return text;
}

