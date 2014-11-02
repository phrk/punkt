#include "Place.h"

Place::Place(uint64_t _id, const std::string &_targeter_args, const PlaceTargets &_targeted_ads):
id(_id),
targeted_ads(_targeted_ads),
vk_match(false) {
	
	try {
		
		parseArgs(_targeter_args);
		
	} catch (...) {
	
		std::cout << "Place::parseArgs could not parse _targeter_args. Using default options\n"; 
	}
}

void Place::parseArgs(const std::string &_targeter_args) {
	
	m_targeter_args = _targeter_args;
	
	json_error_t error;
	json_t *root = json_loads(_targeter_args.c_str(), 0, &error);
	
	json_t *j_domain = json_object_get(root, "domain");
	if (json_is_string(j_domain)) {
	
		domain = json_string_value(j_domain);	
	} else {
		std::cout << "Place::parseArgs could not parse domain\n";
		throw "Place::parseArgs could not parse domain";
	}
	
	json_t *j_vk_match = json_object_get(root, "vk_match");
	if (json_is_string(j_vk_match)) {
		
		std::string vk_match_str = json_string_value(j_vk_match);
		
		if (vk_match_str == "on")
			vk_match = true;
		else
			vk_match = false;
			
	} else {
		std::cout << "Place::parseArgs could not parse vk_match\n";
		throw "Place::parseArgs could not parse vk_match";
	}
	
	json_t *j_vk_app_id = json_object_get(root, "vk_app_id");
	if (json_is_string(j_vk_app_id)) {
	
		vk_app_id = json_string_value(j_vk_app_id);	
	} else {
		std::cout << "Place::parseArgs could not parse vk_app_id\n";
		throw "Place::parseArgs could not parse vk_app_id";
	}
	
	json_decref(root);
}
