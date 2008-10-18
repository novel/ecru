#include <cstdlib>
#include <ctime>
#include <clocale>
#include <iostream>
#include <vector>

#include <glibmm.h>
#include <xmlrpc-c/base.hpp>
#include <xmlrpc-c/client_simple.hpp>

#include "LiveJournal.h"
#include "Config.h"
#include "Event.h"
#include "ecru.h"

LiveJournal::LiveJournal()
{
	setlocale(LC_ALL, "");
	
	this->config = new Config(); //std::string(getenv("HOME")) + "/.ecru/default.conf");

	this->username = this->config->queryConfigProperty("config.account.login");
	this->passwd = this->config->queryConfigProperty("config.account.password");

	this->logged = false;
}

/**
  *  Decodes textual objects from xmlrpc value to std::string
  *  @return std::string in current locale encoding
  */
std::string LiveJournal::decodeTextValue(const xmlrpc_c::value value)
{
	string result;

	if (value.type() == xmlrpc_c::value::TYPE_STRING) {
		result = xmlrpc_c::value_string(value);
	} else if (value.type() == xmlrpc_c::value::TYPE_BYTESTRING) {
		vector<unsigned char> v = xmlrpc_c::value_bytestring(value).vectorUcharValue();
		string raw;

		for (unsigned int i = 0; i < v.size(); i++) {
			raw.push_back(v[i]);
		}

		Glib::ustring unicodeString(raw);

		try {
			result = (std::string)(Glib::locale_from_utf8(unicodeString));
		} catch (Glib::ConvertError ex) {
			result =  "error encoding string";
		}
	}

	return result;
}

xmlrpc_c::value_struct LiveJournal::convertPropertiesToStruct(map<string, string> properties)
{
	map<string, xmlrpc_c::value> propsMap;

	for (map<string, string>::iterator i = properties.begin(); i != properties.end(); ++i) {
		propsMap[i->first] = xmlrpc_c::value_string(i->second);
	}

	return xmlrpc_c::value_struct(propsMap);
}

string LiveJournal::postEvent(Event *ljevent)
{
	login();

	string const serverUrl("http://livejournal.com:80/interface/xmlrpc");
	string const methodName("LJ.XMLRPC.postevent");

	xmlrpc_c::clientSimple xmlrpcClient;
	xmlrpc_c::value result;
	xmlrpc_c::paramList params;

	map<string, xmlrpc_c::value> paramsMap;

	paramsMap["username"] = xmlrpc_c::value_string(username);
	paramsMap["hpassword"] = xmlrpc_c::value_string(passwd);
	paramsMap["ver"] = xmlrpc_c::value_int(1);
	paramsMap["event"] = xmlrpc_c::value_string(ljevent->getEvent());
	paramsMap["subject"] = xmlrpc_c::value_string(ljevent->getSubject());
	paramsMap["lineendings"] = xmlrpc_c::value_string("unix");
	paramsMap["props"] = convertPropertiesToStruct(ljevent->getProperties());

	string usejournal = ljevent->getUsejournal();
	if (usejournal.length() > 0) {
		paramsMap["usejournal"] = xmlrpc_c::value_string(usejournal);
	}

	string security = ljevent->getSecurity();

	if (security == "public") {
		paramsMap["security"] = xmlrpc_c::value_string("public");
	} else if (security == "friendsonly") {
		unsigned int allowmask = 0;
		allowmask |= 1<<0;

		paramsMap["security"] = xmlrpc_c::value_string("usemask");
		paramsMap["allowmask"] = xmlrpc_c::value_int(allowmask);
	} else /* assuming private */ {
		paramsMap["security"] = xmlrpc_c::value_string("private");
	}

	/* time stuff */
	time_t rawtime;
        struct tm *timeinfo;

	time (&rawtime);
	timeinfo = localtime ( &rawtime );

	paramsMap["year"] = xmlrpc_c::value_int(timeinfo->tm_year + 1900);
	paramsMap["mon"] = xmlrpc_c::value_int(timeinfo->tm_mon + 1);
	paramsMap["day"] = xmlrpc_c::value_int(timeinfo->tm_mday);
	paramsMap["hour"] = xmlrpc_c::value_int(timeinfo->tm_hour);
	paramsMap["min"] = xmlrpc_c::value_int(timeinfo->tm_min);

	params.add(xmlrpc_c::value_struct(paramsMap));

	xmlrpcClient.call(serverUrl, methodName, params, &result);

	map<string, xmlrpc_c::value> const resultStruct = xmlrpc_c::value_struct(result);
	
	map<string, xmlrpc_c::value>::const_iterator iter = resultStruct.find("url");

	return (string)xmlrpc_c::value_string(iter->second);
}

#if 0
string LiveJournal::getChallenge() {
	string const serverUrl("http://livejournal.com:80/interface/xmlrpc");
	string const methodName("LJ.XMLRPC.getchallenge");

	xmlrpc_c::clientSimple xmlrpcClient;
	xmlrpc_c::value result;

	xmlrpcClient.call(serverUrl, methodName, &result);

	map<string, xmlrpc_c::value> const resultStruct = xmlrpc_c::value_struct(result);

	map<string, xmlrpc_c::value>::const_iterator iter = resultStruct.find("challenge");

//	return iter->first;
	return (string)xmlrpc_c::value_string(iter->second);
}
#endif

void LiveJournal::login() {
	if (logged == true) {
		return;
	}

	string login = this->config->queryConfigProperty("config.account.login");
	string passwd = this->config->queryConfigProperty("config.account.password");

	string const serverUrl("http://livejournal.com:80/interface/xmlrpc");
	string const methodName("LJ.XMLRPC.login");

	xmlrpc_c::clientSimple xmlrpcClient;
	xmlrpc_c::value result;
	xmlrpc_c::paramList loginParams;

	map<string, xmlrpc_c::value> loginMap;
	loginMap["username"] = xmlrpc_c::value_string(login);
	loginMap["hpassword"] = xmlrpc_c::value_string(passwd);
	loginMap["clientversion"] = xmlrpc_c::value_string(ecru::clientversion);

	loginParams.add(xmlrpc_c::value_struct(loginMap));

	xmlrpcClient.call(serverUrl, methodName, loginParams, &result);

	map<string, xmlrpc_c::value> const resultStruct = xmlrpc_c::value_struct(result);

	// check if we got "usejournals" key
	map<string, xmlrpc_c::value>::const_iterator iter = resultStruct.find("usejournals");
	if (iter != resultStruct.end()) {
		vector<xmlrpc_c::value> const values = 
			(xmlrpc_c::value_array(iter->second)).vectorValueValue();

		for (unsigned int i = 0; i < values.size(); i++) {
			this->usejournals.push_back((string)xmlrpc_c::value_string(values[i]));
		}
	}

	this->logged = true;
}

vector<Event*> LiveJournal::list(int count) {
	login();

	string const serverUrl("http://livejournal.com:80/interface/xmlrpc");
	string const methodName("LJ.XMLRPC.getevents");

	xmlrpc_c::clientSimple xmlrpcClient;
	xmlrpc_c::value result;
	xmlrpc_c::paramList params;

	map<string, xmlrpc_c::value> paramsMap;
	paramsMap["username"] = xmlrpc_c::value_string(username);
	paramsMap["hpassword"] = xmlrpc_c::value_string(passwd);
	paramsMap["ver"] = xmlrpc_c::value_int(1);
	paramsMap["truncate"] = xmlrpc_c::value_int(40);
	paramsMap["selecttype"] = xmlrpc_c::value_string("lastn");
	paramsMap["howmany"] = xmlrpc_c::value_int(count);
	paramsMap["lineendings"] = xmlrpc_c::value_string("unix");

	params.add(xmlrpc_c::value_struct(paramsMap));

	xmlrpcClient.call(serverUrl, methodName, params, &result);

	map<string, xmlrpc_c::value> const resultStruct = xmlrpc_c::value_struct(result);

	vector<Event*> eventsVector;

	map<string, xmlrpc_c::value>::const_iterator iter = resultStruct.find("events");
	if (iter != resultStruct.end()) {
		vector<xmlrpc_c::value> const values = 
			(xmlrpc_c::value_array(iter->second)).vectorValueValue();

		for (unsigned int i = 0; i < values.size(); i++) {
			Event *ljevent = new Event();

			map<string, xmlrpc_c::value> event = xmlrpc_c::value_struct(values[i]);

			ljevent->setItemId((int)xmlrpc_c::value_int(event["itemid"]));
			ljevent->setURL((string)xmlrpc_c::value_string(event["url"]));
			ljevent->setEventTime((string)xmlrpc_c::value_string(event["eventtime"]));
			ljevent->setEvent(decodeTextValue(event["event"]));

			eventsVector.push_back(ljevent);
		}
	}
	
	return eventsVector;
}

Event* LiveJournal::getEvent(int itemId) 
{
	login();

	string const serverUrl("http://livejournal.com:80/interface/xmlrpc");
	string const methodName("LJ.XMLRPC.getevents");

	xmlrpc_c::clientSimple xmlrpcClient;
	xmlrpc_c::value result;
	xmlrpc_c::paramList params;

	map<string, xmlrpc_c::value> paramsMap;

	paramsMap["username"] = xmlrpc_c::value_string(username);
	paramsMap["hpassword"] = xmlrpc_c::value_string(passwd);
	paramsMap["ver"] = xmlrpc_c::value_int(1);
	paramsMap["itemid"] = xmlrpc_c::value_int(itemId);
	paramsMap["selecttype"] = xmlrpc_c::value_string("one");
	paramsMap["lineendings"] = xmlrpc_c::value_string("unix");

	params.add(xmlrpc_c::value_struct(paramsMap));

	xmlrpcClient.call(serverUrl, methodName, params, &result);

	map<string, xmlrpc_c::value> const resultStruct = xmlrpc_c::value_struct(result);
	map<string, xmlrpc_c::value>::const_iterator iter = resultStruct.find("events");

	map<string, xmlrpc_c::value> event = 
		xmlrpc_c::value_struct((xmlrpc_c::value_array(iter->second)).vectorValueValue()[0]);

	Event *ljevent = new Event();

	for (map<string, xmlrpc_c::value>::const_iterator i = event.begin(); i != event.end(); i++) {
		if (i->first == "subject") {
			ljevent->setSubject(decodeTextValue(i->second));
		} else if (i->first == "eventtime") {
			ljevent->setEventTime((string)xmlrpc_c::value_string(i->second));
		} else if (i->first == "event") {
			ljevent->setEvent(decodeTextValue(i->second));
		} else if (i->first == "allowmask") {
			//cout << i->second.type() << endl;
			//cout << (( (unsigned int)xmlrpc_c::value_int(i->second) >> 0) & 1) << endl;
		} else if (i->first == "props") {
			map<string, xmlrpc_c::value> props = xmlrpc_c::value_struct(i->second);

			for (map<string, xmlrpc_c::value>::const_iterator j = props.begin();
					j != props.end(); j++) {
				ljevent->setProperty(j->first, decodeTextValue(j->second));
			}
		}
	}
	
	return ljevent;
}

void LiveJournal::deleteEvent(int itemId)
{
	login();

	string const serverUrl("http://livejournal.com:80/interface/xmlrpc");
	string const methodName("LJ.XMLRPC.editevent");

	xmlrpc_c::clientSimple xmlrpcClient;
	xmlrpc_c::value result;
	xmlrpc_c::paramList params;

	map<string, xmlrpc_c::value> paramsMap;

	paramsMap["username"] = xmlrpc_c::value_string(username);
	paramsMap["hpassword"] = xmlrpc_c::value_string(passwd);
	paramsMap["ver"] = xmlrpc_c::value_int(1);
	paramsMap["itemid"] = xmlrpc_c::value_int(itemId);
	paramsMap["lineendings"] = xmlrpc_c::value_string("unix");
	paramsMap["event"] = xmlrpc_c::value_string("");
	paramsMap["subject"] = xmlrpc_c::value_string("");

	params.add(xmlrpc_c::value_struct(paramsMap));

	xmlrpcClient.call(serverUrl, methodName, params, &result);
}

string LiveJournal::editEvent(Event *event)
{		
	login();

	string const serverUrl("http://livejournal.com:80/interface/xmlrpc");
	string const methodName("LJ.XMLRPC.editevent");

	xmlrpc_c::clientSimple xmlrpcClient;
	xmlrpc_c::value result;
	xmlrpc_c::paramList params;

	map<string, xmlrpc_c::value> paramsMap;

	paramsMap["username"] = xmlrpc_c::value_string(username);
	paramsMap["hpassword"] = xmlrpc_c::value_string(passwd);
	paramsMap["ver"] = xmlrpc_c::value_int(1);
	paramsMap["itemid"] = xmlrpc_c::value_int(event->getItemId());
	paramsMap["event"] = xmlrpc_c::value_string(event->getEvent());
	paramsMap["subject"] = xmlrpc_c::value_string(event->getSubject());
	paramsMap["lineendings"] = xmlrpc_c::value_string("unix");
	paramsMap["props"] = convertPropertiesToStruct(event->getProperties());

	params.add(xmlrpc_c::value_struct(paramsMap));

	string security = event->getSecurity();

	if (security == "public") {
		paramsMap["security"] = xmlrpc_c::value_string("public");
	} else if (security == "friendsonly") {
		unsigned int allowmask = 0;
		allowmask |= 1<<0;

		paramsMap["security"] = xmlrpc_c::value_string("usemask");
		paramsMap["allowmask"] = xmlrpc_c::value_int((int)allowmask);
	} else /* assuming private */ {
		paramsMap["security"] = xmlrpc_c::value_string("private");
	}

	xmlrpcClient.call(serverUrl, methodName, params, &result);

	map<string, xmlrpc_c::value> const resultStruct = xmlrpc_c::value_struct(result);
	
	map<string, xmlrpc_c::value>::const_iterator iter = resultStruct.find("url");

	return (string)xmlrpc_c::value_string(iter->second);
}
