#include "Config.h"
#include "ecru.h"

#include <iostream>
#include <fstream>

#include <libgen.h>
#include <sys/stat.h>

#include <libconfig.h++>

using namespace std;

Config::Config()
{
	this->configDirectory = std::string(getenv("HOME")) + "/.ecru/";

	//string filename = std::string(getenv("HOME")) + "/.ecru/default.conf";
	//string filename = this->configDirectory + "default.conf";
	string filename = getCurrentConfigFilename();

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

	for (unsigned int i = 0; i < files.size(); i++) {
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

string Config::generate(string username, string hpassword)
{
	string configDir = "ecru.new";
	string templateDir = configDir + "/templates";

	/* TODO split into separate function somewhere in ecru:: */
	if ( (mkdir(configDir.c_str(), S_IRWXU))  != 0) {
		perror(configDir.c_str());
		exit(0);
	}

	if ( (mkdir(templateDir.c_str(), S_IRWXU)) != 0) {
		perror(templateDir.c_str());
		exit(0);
	}


	libconfig::Config *cfg = new libconfig::Config();

	libconfig::Setting& setting = cfg->getRoot();

	//cout << setting.isGroup() << endl;
	setting.add("config", libconfig::Setting::TypeGroup);

	libconfig::Setting& configSetting = cfg->lookup("config");
	configSetting.add("account", libconfig::Setting::TypeGroup);

	libconfig::Setting& accountSetting = cfg->lookup("config.account");
	accountSetting.add("login", libconfig::Setting::TypeString);	
	accountSetting.add("password", libconfig::Setting::TypeString);

	accountSetting["login"] = username;
	accountSetting["password"] = hpassword;
	
	cfg->writeFile( (configDir + "/default.conf").c_str() );
	
	ofstream outputStream;

	outputStream.open( (configDir + "/current").c_str() );
	outputStream << "default.conf" << endl;
	outputStream.close();

	outputStream.open( (templateDir + "/default").c_str() );
	outputStream << "subject: " << endl << endl;
	outputStream.close();

	return configDir;
}
