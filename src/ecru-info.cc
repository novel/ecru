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

	while ((ch = getopt(argc, argv, "v")) != -1) {
		switch (ch) {
			case 'v':
				ecru::version();
				exit(0);
			default:
				exit(0);
		}
	}

	argc -= optind;
	argv += optind;

	if (argc != 1) {
		cerr << "Missing itemid" << endl;
		exit(0);
	}

	LiveJournal *livejournal = new LiveJournal();
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

	return 0;
}	
