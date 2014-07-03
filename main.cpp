#include "hiconfig.h"
#include "punktd.h"

int main (int argc, char** argv) {
	
	Punktd *pd;
	
	std::string conf_name;
	//std::cout << "nargs: " << argc << " confif: " << argv[1] << std::endl <<  std::endl;
	
	if (argc <= 1)
		conf_name = "punkt.conf";
	else
		conf_name = std::string(argv[1]);
	
	try {
		pd = new Punktd(conf_name);
	} catch (const char *s) {
		std::cout << "::main exception: " << s << std::endl;
		exit(0);
	}
	pd->join();
	
	return 0;
}
