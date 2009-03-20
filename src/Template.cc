#include <cstdlib>
#include <fstream>
#include <string>
#include <vector>

#include "Template.h"
#include "ecru.h"

using namespace std;

Template::Template()
{
	this->templateDir = string(getenv("HOME")) + "/.ecru/templates/";
}

vector<string> Template::listTemplates()
{
	vector<string> files = ecru::listDirectory(this->templateDir);
	vector<string> templates;

	for (unsigned int i = 0; i < files.size(); i++) {
		string filename = files[i];

		if (filename[0] != '.') {
			templates.push_back(filename);
		}
	}

	return templates;
}

string Template::getTemplate(string templateName)
{
	ifstream fin((this->templateDir + templateName).c_str());

	if (!fin) {
		return "";
	}
	
	string result;
	string line;

	while (getline(fin, line)) {
		result += line + "\n";
	}

	fin.close();

	return result;
}
