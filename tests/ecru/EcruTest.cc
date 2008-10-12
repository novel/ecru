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
	string unstripped = "   somestr ";
	string stripped = ecru::stripString(unstripped);

	CPPUNIT_ASSERT_EQUAL(unstripped.size(), stripped.size() + 4);
}

void EcruTest::testStripNewLines()
{
	CPPUNIT_ASSERT_EQUAL(1, 1);
}
