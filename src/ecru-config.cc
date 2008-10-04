#include <iostream>
#include <cstdlib>
#include <vector>

#include "Hook.h"
#include "Config.h"
#include "ecru.h"

using namespace std;

void usage()
{

	cerr << "usage: ecru-config variable_path" << endl;
	cerr << "usage: ecru-config [-l|-s path]" << endl;
	cerr << "usage: ecru-config [-v|-h]" << endl;
	cerr << "usage: ecru-config -h [pre|post]" << endl;
	exit(1);
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
	bool generate = false;
	string username;
	string hpassword;
	string hookType;

	while ((ch = getopt(argc, argv, "gh:lp:s:u:v")) != -1) {
		switch (ch) {
			case 'g':
				generate = true;
				break;
			case 'h':
				hookType = string(optarg);
				if ((hookType != "post") && 
						(hookType != "pre")) {
					usage();
					exit(1);
				}
				break;
			case 'l':
				listConfigProfiles();
				exit(0);
				break;
			case 'p':
				hpassword = string(optarg);
				break;
			case 's':
				setCurrentConfigFilename(optarg);
				exit(0);	
				break;
			case 'u':
				username = string(optarg);
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
	
	Config *config = new Config();

	if (generate == true) {
		cout << "Generating ecru configuration..." << endl;
		if (username.length() == 0) {
			cout << "Enter username: ";
			cin >> username;
		}

		if (hpassword.length() == 0) {
			cout << "Enter md5 hash of the password: ";
			cin >> hpassword;
		}

		string configPath = Config::generate(username, hpassword);

		cout << "Config file placed to: " << configPath << endl;
		cout << "To install it, type: cp -r " << configPath << " ~/.ecru" << endl;
	} else if (hookType.length() > 0) {
		Hook *hook = new Hook();
		vector<string> hooks;

		if (hookType == "pre") {
			hooks = hook->getPreHooks();
		} else {
			hooks = hook->getPostHooks();
		}
		
		for (unsigned int i = 0; i < hooks.size(); i++) {
			cout << hooks[i] << endl;
		}
	} else {
		cout << config->queryConfigProperty(argv[1]) << endl;
	}

	return 0;
}

