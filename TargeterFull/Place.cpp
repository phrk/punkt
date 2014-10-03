#include "Place.h"

Place::Place(uint64_t _id, const std::string &_targeter_args, const PlaceTargets &_targeted_ads):
id(_id),
targeted_ads(_targeted_ads),
match_vk(false) {
	
	try {
		
		parseArgs(_targeter_args);
		
	} catch (...) {
	
		std::cout << "Place::parseArgs could not parse _targeter_args. Using default options\n"; 
	}
}

void Place::parseArgs(const std::string &_targeter_args) {
	
	json_error_t error;
	json_t *root = json_loads(_targeter_args.c_str(), 0, &error);
	
	int imatch_vk;
	
	json_t *j_match_vk = json_object_get(root, "match_vk");
	if (json_is_integer(j_match_vk)) {
		imatch_vk = json_integer_value(j_match_vk);
		
		if (imatch_vk == 1)
			match_vk = true;
		else if (imatch_vk == 0)
			match_vk = false;
			
	} else {
		std::cout << "Place::parseArgs could not parse match_vk\n";
		throw "Place::parseArgs could not parse match_vk";
	}
		
	json_decref(root);
}
