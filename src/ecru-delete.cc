#include <cstdlib>
#include <iostream>

#include "LiveJournal.h"
#include "ecru.h"

void usage()
{

	std::cerr << "usage: ecru-delete id1 id2 ... idN" << std::endl;
	std::cerr << "usage: ecru-delete -v" << std::endl;
	exit(1);
}

int main(int argc, char** argv)
{
	int ch;

	while ((ch = getopt(argc, argv, "v")) != -1) {
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

	if (argc == 0) {
		usage();
	}	

	LiveJournal *livejournal = new LiveJournal();

	for (int i = 0; i < argc; i++) {
		livejournal->deleteEvent(atoi(argv[i]));
	}

	return 0;
}
