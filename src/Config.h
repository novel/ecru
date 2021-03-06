#ifndef __CONFIG_H
#define __CONFIG_H

#include <string>
#include <vector>

#include <libconfig.h++>
//#include <libxml++/libxml++.h>

using namespace std;

class Config {
	private:
		libconfig::Config *config;
		string configDirectory;

		static Config* _instance;
	protected:
		Config();
	public:
		static Config* instance();
		Config(libconfig::Config *cfg);
//		Config();
//		~Config();
		libconfig::Config* getConfig() { return this->config; };

		string queryConfigProperty(string property);

		string getCurrentConfigFilename();
		void setCurrentConfigFilename(string filename);

		vector<string> listConfigFiles();
		
		static string generate(string username, string hpassword);
};

#endif // __CONFIG_H
