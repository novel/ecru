#include <fstream>
#include <iostream>
#include <cstdlib>
#include <map>

#include "LiveJournal.h"
#include "Hook.h"
#include "Event.h"
#include "Template.h"
#include "ecru.h"

using namespace std;

void help()
{

	cout << "ecru-post" << endl << endl;
	cout << "\t-D prop=value -- add property/value pair to the post" << endl;
	cout << "\t-s subject -- specify post subject" << endl;
	cout << "\t-u journal -- post to other journal" << endl;
	cout << "\t-t template -- specify template name to use" << endl;
	cout << "\t-f filename -- specifiy file to read text from, use '-' for stdin" << endl;
	cout << "\t-v -- show version end exit" << endl;
}

string invoke_editor(string templateName)
{
	string editor;
	string result;
	Config* config = Config::instance();
	char *env = 0;

	Template *templ = new Template();

	editor = (env = getenv("EDITOR")) ? env : "vi";	

	//cout << "using editor: " << editor << endl;
	
	char *tmp_filename;

	tmp_filename = ecru::generateTmpFile();

	string templateContent = templ->getTemplate(templateName);

	cout << templateContent << endl;

	delete templ;

	ofstream fout(tmp_filename);

	// apply templates
	if (templateContent.length() > 0) {
		fout << templateContent;
	}

	// apply footer
	if (config->queryConfigProperty("config.footer.append") == "true") {
		map<string, string> keywords;
		keywords["login"] = config->queryConfigProperty("config.account.login");

		fout << ecru::format(config->queryConfigProperty("config.footer.text"), keywords) << endl;
	}
	
	fout.close();

	// time to apply pre-hooks
	Hook *hook = new Hook();
	string tmp_str = string(tmp_filename);
	hook->execPreHooks(tmp_str);

	int ret = system( (editor + " " + tmp_filename).c_str() );

	if (ret != 0) {
		cout << "error" << endl;
		return NULL;
	}

	// post hooks time
	hook->execPostHooks(string(tmp_filename));

	ifstream mystream;
	mystream.open(tmp_filename);

	string line;
	//cout << "what we got:" << endl;
	while (getline(mystream, line)) {
		//cout << line << endl;
		result += line + "\n";
	}
	
	mystream.close();

	if (remove(tmp_filename) != 0) {
		perror(tmp_filename);
	}

//	cout << result << endl;
//	exit(0);

	return result;
}

int main(int argc, char** argv)
{
	int ch;
	string templateName = "default";
	string filename = "";
	string subject;
	string propertyPair;
	string usejournal;
	size_t index;
	map<string, string> properties;

	while ((ch = getopt(argc, argv, "D:f:hs:t:vu:")) != -1) {
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
			case 'v':
				ecru::version();
				exit(0);
			case 'u':
				usejournal = string(optarg);
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

	if (ecru::stripString(ecru::stripNewLines(event->getEvent())).length() == 0) {
		cerr << "Missing post body, not submitting." << endl;
		exit(0);
	}

	// command line properties doesn't override the ones defined in text 
	if ((event->getSubject().length() == 0) && (subject.length() > 0)) {
		event->setSubject(subject);
	}

	if ((event->getUsejournal().length() == 0) && (usejournal.length() > 0)) {
		event->setUsejournal(usejournal);
	}

	for (map<string,string>::iterator i = properties.begin(); i != properties.end(); i++) {
		if (event->hasProperty(i->first) == false) {
			event->smartSetProperty(i->first, i->second);
		}
	}

	try {
		string postUrl = livejournal->postEvent(event);
		cout << "Location of your post is: " << postUrl << endl;
	} catch (girerr::error& err) {
		cout << "Error: post wasn't submitted!" << endl;
		cout << "LiveJournal server reported the following error: " << err.what() << endl;
	}
}
