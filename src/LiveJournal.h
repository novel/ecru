#ifndef __LIVEJOURNAL_H
#define __LIVEJOURNAL_H

#include <libiqxmlrpc/libiqxmlrpc.h>
#include <libiqxmlrpc/client.h>
#include <libiqxmlrpc/http_client.h>

#include "Config.h"
#include "Event.h"

class LiveJournal {
	public:
		LiveJournal();
		string postEvent(string event, string subject);

		vector<Event*> list(int count = 20);

	private:
		Config *config;

		iqxmlrpc::Client<iqxmlrpc::Http_client_connection> *client;

		string username;
		string passwd;

		void login();
		std::string decodeTextValue(iqxmlrpc::Value *value);
};

#endif // __LIVEJOURNAL_H
