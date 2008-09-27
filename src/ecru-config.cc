#include <iostream>
#include <cstdlib>
#include <vector>

#include "Config.h"
#include "ecru.h"

using namespace std;

void usage()
{

	cerr << "usage: ecru-config variable_path" << endl;
	cerr << "usage: ecru-config [-l|-s path]" << endl;
	cerr << "usage: ecru-config [-v|-h]" << endl;
	exit(1);
}

void help()
{

	cout << "ecru-config" << endl << endl;
	cout << "\t-l -- lists current config profiles marking the active one with * (asteriks)";
	cout << endl;
	cout << "\t-s filename.conf -- switches current config profile to filename.conf" << endl;
	cout << "\t-v -- shows client version" << endl;
	exit(0);
}

void listConfigProfiles() {
	//cout << "hello world" << endl;
	Config *config = new Config();
	
	string currentConfigFilename = config->getCurrentConfigFilename();

	vector<string> configFiles = config->listConfigFiles();

	for (unsigned int i = 0; i < configFiles.size(); i++) {
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

	while ((ch = getopt(argc, argv, "hls:v")) != -1) {
		switch (ch) {
			case 'h':
				help();
			case 'l':
				listConfigProfiles();
				exit(0);
				break;
			case 's':
				setCurrentConfigFilename(optarg);
				exit(0);	
				break;
			case 'v':
				ecru::version();
				exit(0);
			default:
				usage();
			}
	}

	argc -= optind;
	argv += optind;

	Config *config = new Config(); //"/home/novel/.ecru/default.conf");

	cout << config->queryConfigProperty(argv[1]) << endl;

	return 0;
}

