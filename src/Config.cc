#include "Config.h"
#include "ecru.h"

#include <cstdlib>
#include <iostream>
#include <fstream>

#include <libgen.h>
#include <sys/stat.h>

#include <libconfig.h++>

using namespace std;

Config* Config::_instance = 0;

Config* Config::instance()
{
	if (_instance == 0) {
		_instance = new Config;
	}

	return _instance;
}

Config::Config(libconfig::Config *cfg)
{
	this->config = cfg;
}

Config::Config()
{
	this->configDirectory = std::string(getenv("HOME")) + "/.ecru/";

	string filename = getCurrentConfigFilename();

	libconfig::Config *cfg = new libconfig::Config();
	try {	
		cfg->readFile(filename.c_str());
	} catch (libconfig::FileIOException& ex) {
		cerr << "Configuration wasn't found, run 'ecru-config -g' to generate new configuration." << endl;
		exit(1);
	} catch (libconfig::ParseException& ex) {
		cerr << "Parse exception on line = " << ex.getLine() << ", error: " << ex.getError() << endl;
		exit(1);
	}

	this->config = cfg;
}

string Config::queryConfigProperty(string property)
{	
	cout << property << endl;

//	try {
//
		this->config->writeFile( string("dump" + property + "_before.txt").c_str() );

		string result;
		libconfig::Setting& setting = this->config->lookup(property);

		switch (setting.getType()) {
			case libconfig::Setting::TypeString:
				result.assign((const char*)setting);
				break;
			case libconfig::Setting::TypeBoolean:
				result = ((bool)setting) ? "true" : "false";
				break;
			default:
				// XXX I guess it should be an exception
				result = "unknown type.";
				break;
		}

		this->config->writeFile( string("dump" + property + "_after.txt").c_str());

		return result;
/*	} catch (libconfig::SettingNotFoundException& ex) {
		cerr << "Setting \"" << property << "\" was not found!" << endl;
		this->config->writeFile("error.txt");
		exit(1);
	}*/
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
	string configFilename = string(basename((char*)filename.c_str()));

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
	string hooksDir = configDir + "/hooks";

	string dirs[] = { configDir, templateDir, hooksDir, hooksDir + "/pre", hooksDir + "/post" };
	unsigned int dirsCount = sizeof(dirs) / sizeof(dirs[0]);

	for (unsigned int i = 0; i < dirsCount; i++) {
		/* TODO split into separate function somewhere in ecru:: */
		if ((mkdir(dirs[i].c_str(), S_IRWXU)) != 0) {
			perror(dirs[i].c_str());
			exit(1);
		}
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
