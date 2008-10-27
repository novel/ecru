#include <iostream>

#include <syslog.h>
#include <stdarg.h>

#include "Logger.h"
#include "ecru_version.h"

using namespace std;

Logger* Logger::_instance = 0;

Logger* Logger::instance()
{
	if (_instance == 0) {
		_instance = new Logger;
	}

	return _instance;
}

Logger::Logger()
{
	openlog(APP_NAME, LOG_NDELAY, LOG_USER);
}
