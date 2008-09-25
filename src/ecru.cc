#include <sys/types.h>
#include <dirent.h>


#include "ecru.h"

#include <iostream>

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

	if((dp  = opendir(path.c_str())) == NULL) {
		         //   cout << "Error(" << errno << ") opening " << dir << endl;
			 cerr << "error opening dir: " << path << endl;
			      // return errno;
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

