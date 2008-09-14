#include <fstream>
#include <iostream>
#include <cstdlib>

#include "LiveJournal.h"
#include "PostInfo.h"

using namespace std;

string invoke_editor()
{
	string editor;
	string result;

	try {
		editor = getenv("EDITOR");
	} catch (exception ex) {
		editor = "vi";
	}

	//cout << "using editor: " << editor << endl;
	
	char *tmp_filename;

	tmp_filename = tmpnam(NULL);

	cout << tmp_filename << endl;

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

	return result;
}

int main()
{

	LiveJournal *livejournal = new LiveJournal();
	string subject;
//	string postUrl = livejournal->postEvent("body", "subject");

//	cout << postUrl << endl;
//#endif
	//cout << invoke_editor() << endl;
	//exit(0);
	//string text = "subject: hello world\n\nsome text goes there\nanother line\nmore line\nthe end\n";
	string text = invoke_editor();

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
