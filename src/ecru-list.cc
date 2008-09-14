#include <iostream>
#include <vector>
#include <cstdlib>

#include <stdio.h>

#include "LiveJournal.h"
#include "Event.h"

using namespace std;

int main(int argc, char** argv)
{
	int ch;
	int count = 20;

	while ((ch = getopt(argc, argv, "c:")) != -1) {
		switch (ch) {
			case 'c':
				count = atoi(optarg);
				break;
			default:
				exit(0);
		}
	}

	argc -= optind;
	argv += optind;

	LiveJournal *livejournal = new LiveJournal();

	vector<Event*> events = livejournal->list(count);
	
	for (int i = 0; i < events.size(); i++) {
		cout << events[i]->getItemId() << " | ";
		cout.width(40);
		cout.setf(ios::left);
		cout << events[i]->getEvent();
		cout << endl;
	}
}
