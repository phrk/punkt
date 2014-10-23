#ifndef _PLACE_H_
#define _PLACE_H_

#include "hiconfig.h"
#include <string>
#include <boost/shared_ptr.hpp>

#include <jansson.h>
#include "PlaceTargets.h"

class Place {

	void parseArgs(const std::string &_targeter_args);

public:

	uint64_t id;
	PlaceTargets targeted_ads;
	bool vk_match;
	
	std::string m_targeter_args;
	
	Place(uint64_t _id, const std::string &_targeter_args, const PlaceTargets &_targeted_ads);
};

typedef boost::shared_ptr<Place> PlacePtr;

#endif
