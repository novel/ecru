#ifndef __HOOK_H
#define __HOOK_H

#include <string>
#include <vector>

using namespace std;

class Hook {
	private:
		string preHooksDirectory;
		string postHooksDirectory;

		vector<string>* findExecutableFiles(string& directory);
		void execHooks(vector<string> &hooks, string &filename);
	public:
		Hook();

		vector<string> *getPreHooks();
		vector<string> *getPostHooks();

		void execPreHooks(string &filename);
		void execPostHooks(string filename);
};

#endif 
