#include <syslog.h>
#include <stdarg.h>

class Logger {
	public:
		static Logger* instance();
	protected:
		Logger();
	private:
		static Logger* _instance;	
};
