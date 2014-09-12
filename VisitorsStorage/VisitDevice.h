#ifndef _VISIT_DEVICE_H_
#define _VISIT_DEVICE_H_

#include "hiconfig.h"

#include <boost/shared_ptr.hpp>
#include <string>
#include <vector>

#include "Visitor.pb.h"

class AdDisps {
public:
	
	AdDisps(const punkt::VisitDevice::AdDisps &_pb);
	void dump(punkt::VisitDevice::AdDisps *_pb);
	
	uint64_t adid;
	uint32_t n;
};

class AdClicks {
public:
	
	AdClicks(const punkt::VisitDevice::AdClicks &_pb);
	void dump(punkt::VisitDevice::AdClicks *_pb);
	
	uint64_t adid;
	uint32_t n;
};

class VisitorExt {
public:
	
	VisitorExt(const punkt::VisitorExt &_pb);
	void dump(punkt::VisitorExt *_pb);
	
	uint32_t extid;
	std::string data;
};

class VisitDevice {
public:
	
	VisitDevice(const std::string &_vdid, const std::string &_user_agent, uint64_t _create_ts);
	VisitDevice(const punkt::VisitDevice &_pb);
	void dump(punkt::VisitDevice *_pb);
	
	std::string vdid;
	uint64_t create_ts;
	std::string user_agent;
	
	std::vector<std::string> search_queries;
	std::vector<std::string> domains_visited;
	
	std::vector<AdDisps> ads_disped;
	std::vector<AdClicks> ads_clicked;
	
	std::vector<VisitorExt> ext;
};

typedef boost::shared_ptr<VisitDevice> VisitDevicePtr;

#endif
