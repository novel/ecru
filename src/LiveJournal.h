#ifndef __LIVEJOURNAL_H
#define __LIVEJOURNAL_H

#include <map>
#include <vector>
#include <string>

#include <libiqxmlrpc/libiqxmlrpc.h>
#include <libiqxmlrpc/client.h>
#include <libiqxmlrpc/http_client.h>

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

		iqxmlrpc::Client<iqxmlrpc::Http_client_connection> *client;

		std::string username;
		std::string passwd;

		void login();
		std::string decodeTextValue(const iqxmlrpc::Value *value);

		iqxmlrpc::Value convertPropertiesToStruct(map<string, string> properties);
};

#endif // __LIVEJOURNAL_H
