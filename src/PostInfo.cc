#include <string>
#include <sstream>
#include <iostream>

#include <glibmm.h>

#include "PostInfo.h"
#include "ecru.h"

using namespace std;

PostInfo::PostInfo(string text)
{
	parseText(text);
}

bool PostInfo::hasProperty(string propertyName)
{
	return (this->properties.find(propertyName) != this->properties.end());
}

string PostInfo::getProperty(string propertyName)
{
	return this->properties[propertyName];
}

void PostInfo::parseText(string text)
{
	stringstream stream(text);
	string line;
	//int emptyStringsSpottedCount = 0;
	int linesRead = 0;
	int propertiesSpottedCount = 0;
	string postText;
	bool postTextMode = false;

	while (getline(stream, line)) {
		if (postTextMode) {
			postText += line + "\n";
		} else {
			size_t index = line.find(":");
			if (index != string::npos) {
				string propName = line.substr(0, index);
				string propValue = line.substr(index + 1);

				//cout << "[" << strip_string(propName) << "] = [" << strip_string(propValue) << "]" << endl;
		
				this->properties[ecru::stripString(propName)] = (string)Glib::
						locale_to_utf8(ecru::stripString(propValue));

				propertiesSpottedCount++;
			} else if (linesRead == 0) {
				// if it was the first line and no options were given, assume
				// that thw complete text is a post text
				postText += line + "\n";
				postTextMode = true;
			} else if (line.length() == 0) {
				postTextMode = true;
			}
		}

		linesRead++;
	}

	this->text = (string)Glib::locale_to_utf8(postText);
}
