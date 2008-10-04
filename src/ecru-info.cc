#include <iostream>
#include <map>

#include "LiveJournal.h"
#include "Event.h"
#include "ecru.h"

using namespace std;

int main(int argc, char** argv)
{
	int ch;
	int itemId;
	bool showUsejournals = false;

	while ((ch = getopt(argc, argv, "vu")) != -1) {
		switch (ch) {
			case 'v':
				ecru::version();
				exit(0);
			case 'u':
				showUsejournals = true;
				break;
			default:
				exit(0);
		}
	}

	argc -= optind;
	argv += optind;

	LiveJournal *livejournal = new LiveJournal();

	if (showUsejournals == true) {
		vector<string> journals = livejournal->getUsejournals();
		for (unsigned int i = 0; i < journals.size(); i++) {
			cout << journals[i] << endl;
		}
	} else {
		if (argc != 1) {
			cerr << "Missing itemid" << endl;
			exit(0);
		}

		Event *ljevent = new Event();

		itemId = atoi(argv[0]);

		ljevent = livejournal->getEvent(itemId);

		// print out subject
		string subject = ljevent->getSubject();

		if (subject.length() == 0)
			subject = "(no subject)";

		cout << "subject : " << subject;

		// print eventtime
		cout << " @ " << ljevent->getEventTime() << endl;

		// list keywords
		map<string, string> properties = ljevent->getProperties();

		for (map<string, string>::iterator i = properties.begin(); i != properties.end(); ++i) {
			if (i->first != "revnum" && i->first != "revtime")
				cout << i->first << " : " << i->second << endl;
		}

		cout << endl;
		
		// show the event
		cout << ljevent->getEvent() << endl;
	}

	return 0;
}	
