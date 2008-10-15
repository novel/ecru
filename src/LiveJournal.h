#ifndef __LIVEJOURNAL_H
#define __LIVEJOURNAL_H

#include <map>
#include <vector>
#include <string>

#include <xmlrpc-c/base.hpp>

#include "Config.h"
#include "Event.h"

class LiveJournal {
	public:
		LiveJournal();

		string postEvent(string event, string subject);
		string postEvent(Event *event);

		// returns latest count entries (events)
		vector<Event*> list(int count = 20);

		// returns event with the given itemid
		Event* getEvent(int itemId);

		/**
		  * Delete event
		  */
		void deleteEvent(int itemId);

		/**
		  * Update event
		  */
		string editEvent(Event *event);

		/**
		  * @return list of journals user allowed to post to
		  */
		vector<string> getUsejournals() { login(); return this->usejournals; };

	private:
		Config *config;
		vector<string> usejournals;
		bool logged;

		std::string username;
		std::string passwd;

		//std::string getChallenge();
		void login();
		std::string decodeTextValue(const xmlrpc_c::value value);

		xmlrpc_c::value_struct convertPropertiesToStruct(map<string, string> properties);
};

#endif // __LIVEJOURNAL_H
