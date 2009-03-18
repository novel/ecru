#ifndef __EVENT_H
#define __EVENT_H

#include <string>
#include <map>
#include <fstream>
#include <iostream>

using namespace std;

class Event {
	private:
		int itemid;
		std::string subject;
		std::string security;
		std::string event;
		std::string url;
		std::string eventTime;
		std::string usejournal;
		int allowmask;

		map<string, string> properties;

		void parseText(const string text);

		void setUpDefaultProps();

	public:		
		Event();
		Event(const string text);

		int getItemId() { return this->itemid; };
		void setItemId(int itemId) { this->itemid = itemId; };
		
		std::string getSubject() { return this->subject; };
		void setSubject(std::string subject) { this->subject = subject; };

		std::string getSecurity() { return this->security; };
		void setSecurity(std::string security) { this->security = security; };

		std::string getEvent() { return this->event; };
		void setEvent(std::string event) { this->event = event; };

		std::string getURL() { return this->url; };
		void setURL(std::string url) { this->url = url; };

		std::string getEventTime() { return this->eventTime; };
		void setEventTime(std::string eventTime) { this->eventTime = eventTime; };

		std::string getUsejournal() { return this->usejournal; };
		void setUsejournal(std::string usejournal) { this->usejournal = usejournal; };

		int getAllowmask() { return this->allowmask; }
		void setAllowmask(int allowmask) { this->allowmask = allowmask; }

		std::string getProperty(std::string key) { return this->properties[key]; };
		void setProperty(std::string key, std::string value) { this->properties[key] = value; };
		bool hasProperty(std::string key) { 
			return (this->properties.find(key) != this->properties.end()); };

		map<string, string> getProperties() { return this->properties; };

		friend ostream& operator <<(ostream &os, const Event &event);

		string to_string();
			
		string dump(std::ostream &stream, bool add_footer = false);

		/*
		 * Method that automatically figures out what kind of property
		 * it is - keyword or a real property */
		void smartSetProperty(std::string key, std::string value);
};

#endif // __EVENT_H
