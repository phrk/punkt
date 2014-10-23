#ifndef _PLACE_TARGETS_H_
#define _PLACE_TARGETS_H_

#include "hiconfig.h"
#include "ad.h"
#include <vector>

class PlaceTargets {
public:
	//std::vector<AdPtr> ads;
	std::vector<uint64_t> ads;
};

typedef boost::shared_ptr<PlaceTargets> PlaceTargetsPtr;

#endif
