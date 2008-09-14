#ifndef __POSTINFO_H
#define __POSTINFO_H

#include <map>
#include <string>

using namespace std;

class PostInfo {
	public:
		PostInfo(string text);
		string getText() { return this->text; };
		string getProperty(string propertyName);
		bool hasProperty(string propertyName);
	private:
		string text;
		map<string, string> properties;
		void parseText(string text);
};

#endif // __POSTINFO_H
