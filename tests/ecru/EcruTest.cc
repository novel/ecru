#include <string>

#include <cppunit/extensions/HelperMacros.h>

#include "ecru.h"

using namespace std;

class EcruTest : public CPPUNIT_NS::TestFixture
{
	private:
		CPPUNIT_TEST_SUITE( EcruTest );
		CPPUNIT_TEST( testStripString );
		CPPUNIT_TEST( testStripNewLines );
		CPPUNIT_TEST_SUITE_END();
	
	public:
		void testStripString();
		void testStripNewLines();

};

CPPUNIT_TEST_SUITE_REGISTRATION( EcruTest );

void EcruTest::testStripString()
{
	// typical usage
	string unstripped = "   somestr ";
	string stripped = ecru::stripString(unstripped);

	CPPUNIT_ASSERT_EQUAL(unstripped.size(), stripped.size() + 4);
	CPPUNIT_ASSERT(stripped == "somestr");

	// find out what's going on if we pass empty string
	string empty = ecru::stripString("");

	CPPUNIT_ASSERT(empty == "");

	// check if it doesn't modify a string that doesn't need 
	// to be stripped
	string alreadyStripped;
	string doubleStrip = ecru::stripString(alreadyStripped);
	
	CPPUNIT_ASSERT(doubleStrip == alreadyStripped);
}

void EcruTest::testStripNewLines()
{
	// typical usage
	string stringWithNewLines = "hello\nhow are\nyou doing\n";
	string stripped = ecru::stripNewLines(stringWithNewLines);
	
	CPPUNIT_ASSERT(stripped.find("\n") == string::npos);

	// empty
	CPPUNIT_ASSERT("" == ecru::stripNewLines(""));

	// without new lines
	CPPUNIT_ASSERT("foo" == ecru::stripNewLines("foo"));
}
