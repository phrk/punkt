#ifndef _VK_PROFILE_
#define _VK_PROFILE_

#include "hiconfig.h"
#include <boost/shared_ptr.hpp>
#include <string>
#include <vector>

#include "Visitor.pb.h"

class VkProfile {
public:
	std::string id;
	std::string first_name;
	std::string last_name;
	bool sex;
	std::string bdate;
	
	uint32_t city;
	uint32_t country;
	uint32_t timezone;
	std::string photo_url;
	
	std::string site;
	std::string university;
	std::string graduation;
	
	std::vector<std::string> friends;
	std::vector<std::string> groups;
	
	void parseProtobuf(const std::string &_dump);
	void dumpProtobuf(std::string &_dump);
};

typedef boost::shared_ptr<VkProfile> VkProfilePtr;

#endif
