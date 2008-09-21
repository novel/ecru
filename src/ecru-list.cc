#include <iostream>
#include <vector>
#include <cstdlib>

#include <stdio.h>

#include "LiveJournal.h"
#include "Event.h"
#include "ecru.h"

using namespace std;

int main(int argc, char** argv)
{
	int ch;
	int count = 20;
	bool showURL = false;

	while ((ch = getopt(argc, argv, "c:vs")) != -1) {
		switch (ch) {
			case 'c':
				count = atoi(optarg);
				break;
			case 's':
				showURL = true;
				break;
			case 'v':
				ecru::version();
				exit(0);
			default:
				exit(0);
		}
	}

	argc -= optind;
	argv += optind;

	LiveJournal *livejournal = new LiveJournal();

	vector<Event*> events = livejournal->list(count);
	
	for (unsigned int i = 0; i < events.size(); i++) {
		cout << events[i]->getItemId() << " | ";
		cout.width(40);
		cout.setf(ios::left);
		cout << events[i]->getEvent();
		cout << "| " << events[i]->getEventTime();
		if (showURL)
			cout << " | " << events[i]->getURL();
		cout << endl;
	}
}
