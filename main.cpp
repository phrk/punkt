#include "hiconfig.h"
#include "punktd.h"

#include <fstream>
#include <iostream>

int main (int argc, char** argv) {
	
	Punktd *pd;
	
	std::string conf_name;
	//std::cout << "nargs: " << argc << " confif: " << argv[1] << std::endl <<  std::endl;
	
	pid_t pid = getpid();
	
	std::ofstream pidfile("punkt.pid");
	pidfile << pid << std::endl;
	pidfile.close();
	
	if (argc <= 1)
		conf_name = "punkt.conf";
	else
		conf_name = std::string(argv[1]);
	
	try {
		pd = new Punktd(conf_name);
	} catch (const char *s) {
		std::cout << "::main exception: " << s << std::endl;
		exit(0);
	} catch (std::string &_s) {
		std::cout << "::main exception: " << _s << std::endl;
		exit(0);
	} catch (std::string *_s) {
		std::cout << "::main exception: " << *_s << std::endl;
		exit(0);
	}
	pd->join();
	
	return 0;
}
