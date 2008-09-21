#include "Config.h"
#include "ecru.h"

#include <iostream>
#include <fstream>

#include <libgen.h>

#include <libconfig.h++>

using namespace std;

Config::Config()
{
	this->configDirectory = std::string(getenv("HOME")) + "/.ecru/";

	//string filename = std::string(getenv("HOME")) + "/.ecru/default.conf";
	string filename = this->configDirectory + "default.conf";

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

string Config::getCurrentConfigFilename()
{
	string schemaFilename = this->configDirectory + "current";

	ifstream schemaInputStream(schemaFilename.c_str());
	if (!schemaInputStream) {
		return this->configDirectory + "default.conf";
	}

	string currentConfigFilename;

	schemaInputStream >> currentConfigFilename;

	return this->configDirectory + currentConfigFilename;
}

void Config::setCurrentConfigFilename(string filename)
{
	string schemaFilename = this->configDirectory + "current";
	string configFilename = string(basename(filename.c_str()));

	ofstream schemaOutputStream(schemaFilename.c_str());
	
	schemaOutputStream << configFilename << endl;
}

vector<string> Config::listConfigFiles()
{
	vector<string> files = ecru::listDirectory(this->configDirectory);
	vector<string> configFiles;

	for (int i = 0; i < files.size(); i++) {
		string filename = files[i];

		if (filename.length() > 5) {
			// we're interested only in "*.conf$" files
			if (filename.substr(filename.length() - 5) == ".conf") {
				configFiles.push_back(this->configDirectory + filename);
			}
		}
	}

	return configFiles;
}
