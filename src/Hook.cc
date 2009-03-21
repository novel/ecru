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

vector<string>* Hook::findExecutableFiles(string& directory)
{
	vector<string> *pathes = new vector<string>;
	vector<string> *files = ecru::listDirectory(directory);

	for (unsigned int i = 0; i < files->size(); i++) {
		string item = (*files)[i];
		if ((item != ".") && (item != "..")) {
			string path = directory + (*files)[i];

			if (ecru::isExecutable(path)) 
				pathes->push_back(path);
		}
	}

	return pathes;
}

vector<string>* Hook::getPreHooks()	
{
	return this->findExecutableFiles(this->preHooksDirectory);
}

vector<string>* Hook::getPostHooks()
{
	return this->findExecutableFiles(this->postHooksDirectory);
}

void Hook::execPreHooks(string& filename)
{
	this->execHooks(*(this->getPreHooks()), filename);
}

void Hook::execPostHooks(string filename)	
{
//	this->execHooks(this->getPostHooks(), filename);
}

void Hook::execHooks(vector<string> &hooks, string& filename)
{
#if 0
	for (unsigned int i = 0; i < hooks.size(); i++) {
		vector<string> args;
		args.push_back(filename);
		ecru::executeCommand(hooks[i], args);
	}
#endif
}
