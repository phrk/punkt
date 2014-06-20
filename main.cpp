#include "hiconfig.h"
#include "punktd.h"

int main (int argc, char** argv) {
	
	Punktd *pd;
	
	try {
		pd = new Punktd("punkt.conf");
	} catch (const char *s) {
		std::cout << "::main exception: " << s << std::endl;
		exit(0);
	}
	pd->join();
	
	return 0;
}
