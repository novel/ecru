#include <ctime>
#include <iostream>

#include <libiqxmlrpc/libiqxmlrpc.h>
#include <libiqxmlrpc/client.h>
#include <libiqxmlrpc/http_client.h>

#include "LiveJournal.h"
#include "Config.h"

using namespace iqxmlrpc;

LiveJournal::LiveJournal()
{
	this->config = new Config("/home/novel/.ecru/default.conf");
}

string LiveJournal::postEvent(string event, string subject)
{
	login();

	string login = this->config->queryConfigProperty("config.account.login");
	string passwd = this->config->queryConfigProperty("config.account.password");

	Client<Http_client_connection> client(iqnet::Inet_addr("livejournal.com", 80), "/interface/xmlrpc");
	Param_list param_list;
        param_list.push_back(Struct());

	unsigned int allowmask = 0;
	allowmask |= 1<<0;	

        param_list[0].insert("username", login);
        param_list[0].insert("hpassword", passwd);
	param_list[0].insert("ver", "1");
	param_list[0].insert("event", event);
	param_list[0].insert("subject", subject);
	param_list[0].insert("security", "usemask");
	param_list[0].insert("allowmask", (int)allowmask);
	param_list[0].insert("lineendings", "unix");

	/* time stuff */
	time_t rawtime;
        struct tm * timeinfo;
	char buffer [80];

	time ( &rawtime );
	timeinfo = localtime ( &rawtime );

	//strftime (buffer,80,"Now is %I:%M%p.",timeinfo);
	//	    puts (buffer);
	param_list[0].insert("year", timeinfo->tm_year + 1900);
	param_list[0].insert("mon", timeinfo->tm_mon + 1);
	param_list[0].insert("day", timeinfo->tm_mday);
	param_list[0].insert("hour", timeinfo->tm_hour);
	param_list[0].insert("min", timeinfo->tm_min);
	
	Response response = client.execute("LJ.XMLRPC.postevent", param_list);

	Struct st = response.value().the_struct();
	//std::cout << st["itemid"].type_name() << std::endl;
	//std::cout << st["url"].type_name() << std::endl;
	return st["url"].get_string();
}

void LiveJournal::login() {
	string login = this->config->queryConfigProperty("config.account.login");
	string passwd = this->config->queryConfigProperty("config.account.password");

	Client<Http_client_connection> client(iqnet::Inet_addr("livejournal.com", 80), "/interface/xmlrpc");
	Param_list param_list;
	param_list.push_back(Struct());
	param_list[0].insert("username", login);
	param_list[0].insert("hpassword", passwd);

	Response response = client.execute("LJ.XMLRPC.login", param_list);
}
