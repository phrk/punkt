#ifndef _VISITOR_H_
#define _VISITOR_H_

#include "hiconfig.h"
#include "VisitDevice.h"
#include "Visitor.pb.h"

class Visitor {
public:
	std::string id;
	VkProfilePtr vk_profile;
	std::vector<VisitDevice> devices;
	
	void parseProtobuf(const std::string &_dump);
};

typedef boost::shared_ptr<Visitor> VisitorPtr;

#endif
