#include <iostream>

#include "LiveJournal.h"

using namespace std;

int main(int argc, char** argv)
{
	LiveJournal *livejournal = new LiveJournal();

	livejournal->list(5);
}
