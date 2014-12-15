#include "hiconfig.h"
#include "punktd.h"

#include <fstream>
#include <iostream>

#include "signal.h"
#include <sys/types.h> 
#include <sys/wait.h> 

void empty_handler(int signal) {
	
	//std::cout << "empty handler\n";
}


int main (int argc, char** argv) {
	
/*	std::string ref("http%3A//www.sputnik.ru/search%3Fq%3D%25D0%2592%25D0%25B8%25D0%25BB%25D1%258E%25D1%2587%25D0%25B8%25D0%25BD%25D1%2581%25D0%25BA%25D0%25B8%25D0%25B9+%25D0%25B2%25D1%2583%25D0%25BB%25D0%25BA%25D0%25B0%25D0%25BD%26PID%3D2003");
	std::string q;
	RefParser::parse(ref, q);
	
	std::cout << ref << std::endl;
	std::cout << q << std::endl;
	
	
	return 0;
*/	
	/*
	signal(SIGPIPE, empty_handler);
	
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
	pd->join();*/
	
	Punktd* pd = new Punktd("punkt.conf");
	pd->start(false);
	
	return 0;
}
