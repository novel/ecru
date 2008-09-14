#include "Config.h"

#include <iostream>

//#include <xmlwrapp/xmlwrapp.h>
#include <libconfig.h++>

using namespace std;


Config::Config(string filename)
{
	libconfig::Config *cfg = new libconfig::Config();
	try {	
		cfg->readFile(filename.c_str());
	} catch (libconfig::ParseException& ex) {
		cout << "parse exception on line = " << ex.getLine() << ", error: " << ex.getError() << endl;
	}

	this->config = cfg;
}

string Config::queryConfigProperty(string property)
{
	string result = this->config->lookup(property);

	return result;
}
