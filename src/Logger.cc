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
	openlog("ecru", LOG_NDELAY, LOG_USER);
	syslog(LOG_DEBUG, "ecru logger initialized");
}

void Logger::debug(string message)
{
	syslog(LOG_DEBUG, message.c_str());
}
