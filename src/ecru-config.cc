#include <iostream>
#include <cstdlib>
#include <vector>

#include "Config.h"

using namespace std;

void listConfigProfiles() {
	//cout << "hello world" << endl;
	Config *config = new Config();
	
	string currentConfigFilename = config->getCurrentConfigFilename();

	vector<string> configFiles = config->listConfigFiles();

	for (int i = 0; i < configFiles.size(); i++) {
		//cout.width(2);
		if (configFiles[i] == currentConfigFilename) {
			cout << "* ";
		} else {
			cout << "  ";
		}
		cout << configFiles[i];

		cout << endl;
	}

	delete config;
}

void setCurrentConfigFilename(string filename)
{
	Config *config = new Config();

	config->setCurrentConfigFilename(filename);
}

int main(int argc, char** argv) {
	int ch;

	while ((ch = getopt(argc, argv, "ls:")) != -1) {
		switch (ch) {
			case 'l':
				listConfigProfiles();
				exit(0);
				break;
			case 's':
				setCurrentConfigFilename(optarg);
				exit(0);	
				break;
			default:
				cout << "oops" << endl;
				exit(1);
			}
	}

	argc -= optind;
	argv += optind;

	Config *config = new Config(); //"/home/novel/.ecru/default.conf");

	cout << config->queryConfigProperty(argv[1]) << endl;

	return 0;
}

