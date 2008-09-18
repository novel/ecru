#include <iostream>

#include "LiveJournal.h"
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

	itemId = atoi(argv[0]);

	cout << "itemId = " << itemId << endl;

	livejournal->getEvent(itemId);

	return 0;
}	
