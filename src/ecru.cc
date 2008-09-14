#include "ecru.h"

#include <iostream>

void ecru::version()
{
	std::cout << ecru::appname << " " << ecru::appversion << std::endl;
	std::cout << "Roman Bogorodskiy <bogorodskiy@gmail.com>" << std::endl << std::endl;
}
