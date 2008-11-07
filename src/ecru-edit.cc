#include <iostream>
#include <cstdlib>
#include <cstdio>

#include "LiveJournal.h"
#include "ecru.h"

using namespace std;

void usage()
{

	cout << "usage: ecru-edit itemId" << endl;
	cout << "usage: ecru-edit -v" << endl;
	exit(0);
}

/* not very good we have a function with the same name for
 * ecru-post, but their behaviour differs, though probably
 * it needs to be splitted in a smaller parts */
string invoke_editor(Event *ljevent)
{
	string editor;
	string result;

	try {
		editor = getenv("EDITOR");
	} catch (exception ex) {
		editor = "vi";
	}

	char *tmpFilename;

	tmpFilename = ecru::generateTmpFile();

	ofstream fout(tmpFilename);
	ljevent->dump(fout);
	fout.close();

	int ret = system( (editor + " " + tmpFilename).c_str() );

	if (ret != 0) {
		cerr << "error" << endl;
		return NULL;
	}
	
	//result = ecru::readFile(tmpFilename);

	/* XXX should use ecru::readFile instead! But it doesn't work, blah */
	ifstream mystream;
	mystream.open(tmpFilename);

	string line;
	while (getline(mystream, line)) {
		 result += line + "\n";
	}

	mystream.close();

	if (remove(tmpFilename) != 0) {
		perror(tmpFilename);
	} 

	return result;
}

int main(int argc, char** argv)
{
	int ch;

	while ((ch = getopt(argc, argv, "c:vs")) != -1) {
		switch (ch) {
			case 'v':
				ecru::version();
				exit(0);
			default:
				usage();
		}
	}

	argc -= optind;
	argv += optind;
		
	if (argc != 1) {
		usage();
	}

	int itemId = atoi(argv[0]);

	LiveJournal *livejournal = new LiveJournal();
	Event *ljevent = new Event();

	ljevent = livejournal->getEvent(itemId);

	string text = invoke_editor(ljevent);

	Event *updatedEvent = new Event(text);
	updatedEvent->setItemId(ljevent->getItemId());

	string postUrl = livejournal->editEvent(updatedEvent);
	cout << "Location of your post is: " << postUrl << endl;
	
	return 0;
}
