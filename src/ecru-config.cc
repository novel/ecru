#include <iostream>

#include "Config.h"

using namespace std;

int main(int argc, char** argv) {
	Config *config = new Config("/home/novel/.ecru/default.conf");

	cout << config->queryConfigProperty(argv[1]) << endl;

	return 0;
}

