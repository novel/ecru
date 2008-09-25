#include <iostream>

#include <glibmm.h>

#include "Event.h"
#include "ecru.h"

using namespace std;

Event::Event()
{
	this->setUpDefaultProps();
}

Event::Event(const string text)
{
	this->setUpDefaultProps();

	this->parseText(text);
}

void Event::setUpDefaultProps()
{
	// set come default values
	this->security = "public";
}

void Event::parseText(const string text)
{
	stringstream stream(text);
	string line;
	int linesRead = 0;
	int propertiesSpottedCount = 0;
	string postText;
	bool postTextMode = false;

	while (getline(stream, line)) {
		if (postTextMode) {
			postText += line + "\n";
		} else {
			size_t index = line.find(":");
			if (index != string::npos) {
				string propName = line.substr(0, index);
				string propValue = ecru::stripString(line.substr(index + 1));


				// subject is actually not a property
				if (propName == "subject") {
					this->subject = (string)Glib::locale_to_utf8(propValue);
				} else if (propName == "security") {
					// "security" is actually a property, but
					// requires some special handling
					string decodedPropValue = (string)Glib::locale_to_utf8(propValue);

					if ((decodedPropValue != "public") or
						(decodedPropValue != "friendsonly") or 
							(decodedPropValue != "private")) {
						this->security = "public";
					} else {
						this->security = decodedPropValue;
					}
				} else {
					this->properties[ecru::stripString(propName)] = (string)Glib::
						locale_to_utf8(propValue);
				}

				propertiesSpottedCount++;
			} else if (linesRead == 0) {
				// if it was the first line and no options were given, assume
				// that thw complete text is a post text
				postText += line + "\n";
				postTextMode = true;
			} else if (line.length() == 0) {
				postTextMode = true;
			}
		}

		linesRead++;
	}

	this->event = (string)Glib::locale_to_utf8(postText);
}

ostream& operator <<(ostream &os, const Event &event)
{
      os << event.subject;
      return os;
}
/*
friend std::stream& operator<<(std::stream& os, const Event& event)
{
	return os << event->getSubject;
} */
