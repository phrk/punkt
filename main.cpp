#include "hiconfig.h"
#include "punktd.h"

int main (int argc, char** argv) {
	
	Punktd *pd = new Punktd("punkt.conf");
	pd->join();
	
	return 0;
}
