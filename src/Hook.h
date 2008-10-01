#ifndef __HOOK_H
#define __HOOK_H

#include <string>
#include <vector>

using namespace std;

class Hook {
	private:
		string preHooksDirectory;
		string postHooksDirectory;

	public:
		Hook();

		vector<string> getPreHooks();
		vector<string> getPostHooks();
};

#endif 
