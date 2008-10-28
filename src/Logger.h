#include <string>

#include <syslog.h>
#include <stdarg.h>

class Logger {
	public:
		static Logger* instance();

		void debug(std::string message);
	protected:
		Logger();
	private:
		static Logger* _instance;	
};
