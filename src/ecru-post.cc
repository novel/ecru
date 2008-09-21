#include <fstream>
#include <iostream>
#include <cstdlib>

#include "LiveJournal.h"
#include "PostInfo.h"
#include "Template.h"

using namespace std;

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

	cout << templateContent << endl;

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

	while ((ch = getopt(argc, argv, "t:")) != -1) {
		switch (ch) {
			case 't':
				templateName = string(optarg);
				break;
			default:
				exit(1);
		}
	}

	LiveJournal *livejournal = new LiveJournal();
	string subject;

//	string postUrl = livejournal->postEvent("body", "subject");

//	cout << postUrl << endl;
//#endif
	//cout << invoke_editor() << endl;
	//exit(0);
	//string text = "subject: hello world\n\nsome text goes there\nanother line\nmore line\nthe end\n";
	string text = invoke_editor(templateName);

	PostInfo *postInfo = new PostInfo(text);
	//cout << postInfo->getText() << endl;
	if (postInfo->hasProperty("subject") == true) {
		subject = postInfo->getProperty("subject");
	} else {
		cout << "subject is missing" << endl;
		exit(0);
	}		

	string postUrl = livejournal->postEvent(postInfo->getText(), subject);
	cout << "Location of your post is: " << postUrl << endl;
}
