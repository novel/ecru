#ifndef __CONFIG_H
#define __CONFIG_H

#include <string>

#include <libconfig.h++>
//#include <libxml++/libxml++.h>

using namespace std;

class Config {
	private:
		libconfig::Config *config;
	public:
		Config(string filename);
		~Config();

		string queryConfigProperty(string property);
};

#endif // __CONFIG_H
