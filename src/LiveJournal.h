#ifndef __LIVEJOURNAL_H
#define __LIVEJOURNAL_H

#include "Config.h"

class LiveJournal {
	public:
		LiveJournal();
		string postEvent(string event, string subject);

	private:
		Config *config;	

		void login();
};

#endif // __LIVEJOURNAL_H
