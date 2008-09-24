#include <cstdlib>
#include <ctime>
#include <clocale>
#include <iostream>
#include <vector>

#include <glibmm.h>
#include <libiqxmlrpc/libiqxmlrpc.h>
#include <libiqxmlrpc/client.h>
#include <libiqxmlrpc/http_client.h>

#include "LiveJournal.h"
#include "Config.h"
#include "Event.h"

using namespace iqxmlrpc;

LiveJournal::LiveJournal()
{
	setlocale(LC_ALL, "");
	
	this->config = new Config(); //std::string(getenv("HOME")) + "/.ecru/default.conf");
	this->client = new Client<Http_client_connection>(iqnet::Inet_addr("livejournal.com", 80), "/interface/xmlrpc");

	this->username = this->config->queryConfigProperty("config.account.login");
	this->passwd = this->config->queryConfigProperty("config.account.password");
}

/**
  *  Decodes textual objects from xmlrpc value to std::string
  *  @return std::string in current locale encoding
  */
std::string LiveJournal::decodeTextValue(const iqxmlrpc::Value *value)
{
	std::string result;

	if (value->is_string()) {
		result = value->get_string();
	} else if (value->is_binary()) {
		string raw = value->get_binary().get_data();
		Glib::ustring unicodeString(raw);

		try {
			result = (std::string)(Glib::locale_from_utf8(unicodeString));
		} catch (Glib::ConvertError ex) {
			result =  "error encoding string";
		}
	}

	return result;
}

iqxmlrpc::Value LiveJournal::convertPropertiesToStruct(map<string, string> properties)
{
	Struct propertiesStruct;

	for (map<string, string>::iterator i = properties.begin(); i != properties.end(); ++i) {
		propertiesStruct.insert(i->first, i->second);
	}

	return propertiesStruct;
}

string LiveJournal::postEvent(Event *ljevent)
{
	login();

	Client<Http_client_connection> client(iqnet::Inet_addr("livejournal.com", 80), "/interface/xmlrpc");
	Param_list param_list;
	param_list.push_back(Struct());

	unsigned int allowmask = 0;
	allowmask |= 1<<0;



	param_list[0].insert("username", this->username);
	param_list[0].insert("hpassword", this->passwd);
	param_list[0].insert("ver", "1");
	param_list[0].insert("event", ljevent->getEvent());
	param_list[0].insert("subject", ljevent->getSubject());
	param_list[0].insert("security", "usemask");
	param_list[0].insert("allowmask", (int)allowmask);
	param_list[0].insert("lineendings", "unix");
	param_list[0].insert("props", this->convertPropertiesToStruct(ljevent->getProperties()));

	/* time stuff */
	time_t rawtime;
        struct tm * timeinfo;

	time ( &rawtime );
	timeinfo = localtime ( &rawtime );

	param_list[0].insert("year", timeinfo->tm_year + 1900);
	param_list[0].insert("mon", timeinfo->tm_mon + 1);
	param_list[0].insert("day", timeinfo->tm_mday);
	param_list[0].insert("hour", timeinfo->tm_hour);
	param_list[0].insert("min", timeinfo->tm_min);
	
	Response response = client.execute("LJ.XMLRPC.postevent", param_list);

	Struct st = response.value().the_struct();
	
	return st["url"].get_string();
}

// deprecated
string LiveJournal::postEvent(string event, string subject)
{
	login();

	string login = this->config->queryConfigProperty("config.account.login");
	string passwd = this->config->queryConfigProperty("config.account.password");

	Client<Http_client_connection> client(iqnet::Inet_addr("livejournal.com", 80), "/interface/xmlrpc");
	Param_list param_list;
        param_list.push_back(Struct());

	unsigned int allowmask = 0;
	allowmask |= 1<<0;	

        param_list[0].insert("username", login);
        param_list[0].insert("hpassword", passwd);
	param_list[0].insert("ver", "1");
	param_list[0].insert("event", event);
	param_list[0].insert("subject", subject);
	param_list[0].insert("security", "usemask");
	param_list[0].insert("allowmask", (int)allowmask);
	param_list[0].insert("lineendings", "unix");

	/* time stuff */
	time_t rawtime;
        struct tm * timeinfo;

	time ( &rawtime );
	timeinfo = localtime ( &rawtime );

	//strftime (buffer,80,"Now is %I:%M%p.",timeinfo);
	//	    puts (buffer);
	param_list[0].insert("year", timeinfo->tm_year + 1900);
	param_list[0].insert("mon", timeinfo->tm_mon + 1);
	param_list[0].insert("day", timeinfo->tm_mday);
	param_list[0].insert("hour", timeinfo->tm_hour);
	param_list[0].insert("min", timeinfo->tm_min);
	
	Response response = client.execute("LJ.XMLRPC.postevent", param_list);

	Struct st = response.value().the_struct();
	//std::cout << st["itemid"].type_name() << std::endl;
	//std::cout << st["url"].type_name() << std::endl;
	return st["url"].get_string();
}

void LiveJournal::login() {
	string login = this->config->queryConfigProperty("config.account.login");
	string passwd = this->config->queryConfigProperty("config.account.password");

	Client<Http_client_connection> client(iqnet::Inet_addr("livejournal.com", 80), "/interface/xmlrpc");
	Param_list param_list;
	param_list.push_back(Struct());
	param_list[0].insert("username", login);
	param_list[0].insert("hpassword", passwd);

	Response response = client.execute("LJ.XMLRPC.login", param_list);
}

vector<Event*> LiveJournal::list(int count) {
	login();

	Param_list param_list;
	param_list.push_back(Struct());
	param_list[0].insert("username", username);
        param_list[0].insert("hpassword", passwd);	
	param_list[0].insert("ver", "1");
	param_list[0].insert("truncate", "40");
	param_list[0].insert("noprops", "1");
	param_list[0].insert("selecttype", "lastn");
	param_list[0].insert("howmany", count);
	param_list[0].insert("lineendings", "unix");

	Response response = client->execute("LJ.XMLRPC.getevents", param_list);

	Struct st = response.value().the_struct();
	
	Array events = st["events"].the_array();

	vector<Event*> events_vector;
	for (Array::const_iterator i = events.begin(); i != events.end(); ++i) {
		Struct event = i->the_struct();

		Event *ljevent = new Event();
		ljevent->setItemId(event["itemid"].get_int());
		ljevent->setURL(event["url"].get_string());
		ljevent->setEventTime(event["eventtime"].get_string());		
	/*	cout << "------------" << endl;
		cout << "itemid = " << event["itemid"].get_int() << endl;
		cout << "url = " << event["url"].get_string() << endl;
		cout << "event = " << event["event"].type_name() << endl;
		cout << "------------" << endl;*/

		//cout << "eventtime = " << event["eventtime"].get_string() << endl;

		ljevent->setSubject(decodeTextValue(&event["subject"]));
		ljevent->setEvent(decodeTextValue(&event["event"]));
#if 0		
		if (event["event"].is_string()) {
			ljevent->setEvent(event["event"].get_string());
		} else if (event["event"].is_binary()) {
			string strEvent = event["event"].get_binary().get_data();
			Glib::ustring uevent(strEvent);
		
			try {	
				ljevent->setEvent(Glib::locale_from_utf8(uevent));
			} catch (Glib::ConvertError ex) {
				//cout << Glib::convert_with_fallback(strEvent, "koi8-r", "utf-8") << endl;
				//ljevent->setEvent(ljevent->getURL());
				ljevent->setEvent("cannot convert text to your locale");
			}
		}
#endif

		events_vector.push_back(ljevent);
	}

	return events_vector;
}

Event* LiveJournal::getEvent(int itemId) 
{
	login();

        Param_list param_list;
        param_list.push_back(Struct());
        param_list[0].insert("username", username);
        param_list[0].insert("hpassword", passwd);
        param_list[0].insert("ver", "1");
	param_list[0].insert("itemid", itemId);
        param_list[0].insert("selecttype", "one");
        param_list[0].insert("lineendings", "unix");

        Response response = client->execute("LJ.XMLRPC.getevents", param_list);

        Struct st = response.value().the_struct();

        Array events = st["events"].the_array();

        vector<Event*> events_vector;

	Event *ljevent = new Event();
	//for (Array::const_iterator i = events.begin(); i != events.end(); ++i) {
		Value i = events[0];
		Struct eventStruct = i.the_struct();

//		std::cout << eventStruct["url"].get_string() << std::endl;
		//std::cout << decodeTextValue((Value)eventStruct["url"]) << std::endl;


		if (eventStruct.has_field("props") == true) {
			// process properties
			Struct propsStruct = eventStruct["props"].the_struct();
			
			for (Struct::const_iterator j = propsStruct.begin(); j != propsStruct.end(); j++) {
				std::string key = j->first;

				//std::cout << key << " : " << decodeTextValue(j->second) << std::endl;

				ljevent->setProperty(j->first, decodeTextValue(j->second));
			}
		}

		ljevent->setEventTime(eventStruct["eventtime"]);
		ljevent->setEvent(decodeTextValue(&eventStruct["event"]));
		//std::cout << ljevent->getEvent() << endl;
//	}

//	std::cout << events.size() << std::endl;

//	Event *event = new Event();

	return ljevent;

}
