#include <string>

#include <cppunit/extensions/HelperMacros.h>
#include <libconfig.h++>

#include "Config.h"
#include "ecru.h"

using namespace std;

class ConfigTest : public CPPUNIT_NS::TestFixture
{
	private:
		CPPUNIT_TEST_SUITE( ConfigTest );
		CPPUNIT_TEST( ConfigTest::testThatQueryConfigPropertyWorksOkForStrings );
		CPPUNIT_TEST_SUITE_END();
	
	public:
		void setUp();

		void testThatQueryConfigPropertyWorksOkForStrings();
};

CPPUNIT_TEST_SUITE_REGISTRATION( ConfigTest );

void ConfigTest::setUp()
{
	
}

void ConfigTest::testThatQueryConfigPropertyWorksOkForStrings()
{
	libconfig::Config *cfg = new libconfig::Config();
	libconfig::Setting& setting = cfg->getRoot();
	setting.add("config", libconfig::Setting::TypeGroup);
	libconfig::Setting& configSetting = cfg->lookup("config");
	configSetting.add("test", libconfig::Setting::TypeString);
	configSetting.add("true", libconfig::Setting::TypeBoolean);
	configSetting.add("false", libconfig::Setting::TypeBoolean);
	configSetting["test"] = "foobar";
	configSetting["true"] = true;
	configSetting["false"] = false;

	Config *config = new Config(cfg);

	string result = config->queryConfigProperty("config.test");
	CPPUNIT_ASSERT(result == "foobar");
	CPPUNIT_ASSERT("true" == config->queryConfigProperty("config.true"));
	CPPUNIT_ASSERT("false" == config->queryConfigProperty("config.false"));
}
