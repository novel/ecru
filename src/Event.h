#ifndef __EVENT_H
#define __EVENT_H

#include <string>
#include <map>

using namespace std;

class Event {
	private:
		int itemid;
		std::string subject;
		std::string event;
		std::string url;
		std::string eventTime;

		map<string, string> properties;

	public:
		int getItemId() { return this->itemid; };
		void setItemId(int itemId) { this->itemid = itemId; };
		
		std::string getSubject() { return this->subject; };
		void setSubject(std::string subject) { this->subject = subject; };

		std::string getEvent() { return this->event; };
		void setEvent(std::string event) { this->event = event; };

		std::string getURL() { return this->url; };
		void setURL(std::string url) { this->url = url; };

		std::string getEventTime() { return this->eventTime; };
		void setEventTime(std::string eventTime) { this->eventTime = eventTime; };

		std::string getProperty(std::string key) { return this->properties[key]; };
		void setProperty(std::string key, std::string value) { this->properties[key] = value; };

		map<string, string> getProperties() { return this->properties; };
};

#endif // __EVENT_H
