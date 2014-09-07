#ifndef _VISIT_DEVICE_
#define _VISIT_DEVICE_

#include "hiconfig.h"

#include <boost/shared_ptr.hpp>
#include <string>
#include <vector>

#include "Visitor.pb.h"

class VisitDevice {
public:
	
	std::string id;
	uint64_t create_ts;
	
	std::string vkid;	
	std::string user_agent;
	
	std::vector<std::string> search_queries;
	std::vector<std::string> domains_visited;
	std::vector<uint64_t> ads_clicked;
	std::vector<uint64_t> items_clicked;
	std::vector<uint64_t> items_bought;
	
	bool ismobile;
};

typedef boost::shared_ptr<VisitDevice> VisitDevicePtr;

#endif
