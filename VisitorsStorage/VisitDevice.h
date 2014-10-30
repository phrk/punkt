#ifndef _VISIT_DEVICE_H_
#define _VISIT_DEVICE_H_

#include "hiconfig.h"

#include <boost/shared_ptr.hpp>
#include <string>
#include <vector>

#include "hiaux/structs/hashtable.h"

#include "Visitor.pb.h"

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
	
	uint64_t stats_day_begin_ts;
	
	hiaux::hashtable<uint64_t, int> ads_disped_today;
	hiaux::hashtable<uint64_t, int> ads_clicked_today;
	hiaux::hashtable<uint64_t, int> places_today;
	hiaux::hashtable<uint64_t, int> domains_today;
		
	std::vector<VisitorExt> ext;
};

typedef boost::shared_ptr<VisitDevice> VisitDevicePtr;

#endif
