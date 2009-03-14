#include <string>
#include <iostream>

#include <cppunit/extensions/HelperMacros.h>

#include "Event.h"
#include "ecru.h"

using namespace std;

class EventTest : public CPPUNIT_NS::TestFixture
{
	private:
		CPPUNIT_TEST_SUITE( EventTest );
		CPPUNIT_TEST( testParseSimpleText );
		CPPUNIT_TEST( testParseTextWithKeywordsAndProps );
		CPPUNIT_TEST( testSecurityFieldBehaviour );
		CPPUNIT_TEST( testThatLinesStartingWithEcruAreSkipped );
		CPPUNIT_TEST( testThatToStringOutputFormatIsSane );
		CPPUNIT_TEST_SUITE_END();
	
	public:
		void testParseSimpleText();
		void testParseTextWithKeywordsAndProps();
		void testSecurityFieldBehaviour();
		void testThatLinesStartingWithEcruAreSkipped();
		void testThatToStringOutputFormatIsSane();
};

CPPUNIT_TEST_SUITE_REGISTRATION( EventTest );

void EventTest::testParseSimpleText()
{
	const string text = "hello world";
	Event *event = new Event(text);
	string eventText = event->getEvent();

	CPPUNIT_ASSERT(event->getEvent().length() > 0);	
}

void EventTest::testParseTextWithKeywordsAndProps()
{
	const string text = "subject: hello\nsecurity: public\ncurrent_music: foobar\n\nhello world";

	Event *event = new Event(text);
	string eventText = event->getEvent();

	CPPUNIT_ASSERT(event->getEvent().length() > 0);
	CPPUNIT_ASSERT(event->getSubject() == "hello");
	CPPUNIT_ASSERT(event->getSecurity() == "public");
	CPPUNIT_ASSERT(event->getProperty("current_music") == "foobar");
}

void EventTest::testSecurityFieldBehaviour()
{
	/* public, friendsonly, private security are obvious,
	 * if security field is wrong, roll back to public */
	const string securityPublic = "security: public\n\nbody";
	const string securityFriendsOnly = "security: friendsonly\n\nbody";
	const string securityPrivate = "security: private\n\nbody";
	const string securityWrong = "security: b00b00dh44\n\nbody";

	Event *eventPublic = new Event(securityPublic);
	Event *eventFriendsOnly = new Event(securityFriendsOnly);
	Event *eventPrivate = new Event(securityPrivate);
	Event *eventUnknown = new Event(securityWrong);

	CPPUNIT_ASSERT(eventPublic->getSecurity() == "public");
	CPPUNIT_ASSERT(eventFriendsOnly->getSecurity() == "friendsonly");
	CPPUNIT_ASSERT(eventPrivate->getSecurity() == "private");
	CPPUNIT_ASSERT(eventUnknown->getSecurity() == "public");
}

void EventTest::testThatLinesStartingWithEcruAreSkipped()
{
	const string text = "subject: hello\n\nhello world\nwasup?\necru:test\nsomething again\n";

	Event *event = new Event(text);
	string eventText = event->getEvent();

	cout << eventText << endl;
}

void EventTest::testThatToStringOutputFormatIsSane()
{
	const string text = "subject: hello\nsecurity: public\ncurrent_music: foobar\n\nhello world";

	Event *event = new Event(text);

	CPPUNIT_ASSERT(event->to_string() == text);
}
