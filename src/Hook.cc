#include <string>
#include <vector>
#include <iostream>

#include "Hook.h"
#include "ecru.h"

using namespace std;

Hook::Hook()
{
	string configDirectory = ecru::getConfigDirectory();

	this->preHooksDirectory = configDirectory + "hooks/pre/";
	this->postHooksDirectory = configDirectory + "hooks/post/";       			
}

vector<string> Hook::getPreHooks()
{
	vector<string> pathes;
	vector<string> files = ecru::listDirectory(this->preHooksDirectory);

	for (unsigned int i = 0; i < files.size(); i++) {
		string item = files[i];
		if ((item != ".") && (item != "..")) {
			string path = this->preHooksDirectory + files[i];
		
			if (ecru::isExecutable(path)) 
				pathes.push_back(path);
		}
	}

	return pathes;
}

vector<string> Hook::getPostHooks()
{
	vector<string> result = ecru::listDirectory(this->postHooksDirectory);

	return result;
}
