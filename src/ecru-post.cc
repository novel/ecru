#include <fstream>
#include <iostream>
#include <cstdlib>
#include <map>

#include "LiveJournal.h"
#include "Event.h"
#include "Template.h"
#include "ecru.h"

using namespace std;

void help()
{

	cout << "ecru-post" << endl << endl;
	cout << "\t-D prop=value -- add property/value pair to the post" << endl;
	cout << "\t-s subject -- specify post subject" << endl;
	cout << "\t-t template -- specify template name to use" << endl;
	cout << "\t-f filename -- specifiy file to read text from, use '-' for stdin" << endl;
}

string invoke_editor(string templateName)
{
	string editor;
	string result;

	Template *templ = new Template();

	try {
		editor = getenv("EDITOR");
	} catch (exception ex) {
		editor = "vi";
	}

	//cout << "using editor: " << editor << endl;
	
	char *tmp_filename;

	tmp_filename = tmpnam(NULL);

	string templateContent = templ->getTemplate(templateName);

	//cout << templateContent << endl;

	delete templ;

	if (templateContent.length() > 0) {
		ofstream fout(tmp_filename);
		fout << templateContent;
		fout.close();
	}

//	cout << tmp_filename << endl;

	int ret = system( (editor + " " + tmp_filename).c_str() );

	if (ret != 0) {
		cout << "error" << endl;
		return NULL;
	}

	ifstream mystream;
	mystream.open(tmp_filename);

	string line;
	//cout << "what we got:" << endl;
	while (getline(mystream, line)) {
		//cout << line << endl;
		result += line + "\n";
	}
	
	mystream.close();

	return result;
}

int main(int argc, char** argv)
{
	int ch;
	string templateName = "default";
	string filename = "";
	string subject;
	string propertyPair;
	size_t index;
	map<string, string> properties;

	while ((ch = getopt(argc, argv, "D:f:hs:t:")) != -1) {
		switch (ch) {
			case 'D':
				propertyPair = string(optarg);
				index = propertyPair.find_first_of("=");
				if (index == string::npos) {
					cerr << "Wrong property pair (" << propertyPair;
					cerr << "): should look like: key=value" << endl;
				} else {			
					properties[propertyPair.substr(0, index)] = \
						propertyPair.substr(index+1, propertyPair.length());
				}
				break;				
			case 'f':
				filename = optarg;
				break;
			case 'h':
				help();
				exit(0);
			case 's':
				subject = string(optarg);
				break;
			case 't':
				templateName = string(optarg);
				break;
			default:
				exit(1);
		}
	}

	argc -= optind;
	argv += optind;

	string text;

	if (filename != "") {
		text = ecru::readFile(filename);	
	} else {
		text = invoke_editor(templateName);
	}

	LiveJournal *livejournal = new LiveJournal();

	Event *event = new Event(text);

	// command line properties doesn't override the ones defined in text 
	if ((event->getSubject().length() == 0) && (subject.length() > 0)) {
		event->setSubject(subject);
	}

	for (map<string,string>::iterator i = properties.begin(); i != properties.end(); i++) {
		if (event->hasProperty(i->first) == false) {
			event->setProperty(i->first, i->second);
		}
	}

	string postUrl = livejournal->postEvent(event);
	cout << "Location of your post is: " << postUrl << endl;
}
