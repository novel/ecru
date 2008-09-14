#ifndef __EVENT_H
#define __EVENT_H

#include <string>

class Event {
	private:
		int itemid;
		std::string subject;
		std::string event;
		std::string url;
		std::string eventTime;

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
};

#endif // __EVENT_H
