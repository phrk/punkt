#ifndef _VISITOR_HASHD_H_
#define _VISITOR_HASHD_H_

#include "hiconfig.h"
#include "VisitDevice.h"
#include "VkProfile.h"
#include "Visitor.pb.h"

#include <boost/function.hpp>

#include "Visitor.h"
#include "Targeter.h"

#include "hiaux/crypt/base64.h"

class VisitorHashd : public Visitor {
	
	boost::function<void(VisitorHashd*)> m_onSave;
	
public:
	VkProfilePtr vk_profile;
	std::vector<VisitDevice> devices;
	std::vector<VisitorExt> ext;
	
	bool tried_vk_matching;
	
	bool m_saving;
	
	bool newdevice;
	std::string cur_vdid;
	VisitDevice *cur_device;
	
	uint64_t ttl;
	uint64_t ttl_inc;
	
	VisitorHashd(const std::string &_vid,
				const std::string &_vdid,
				uint64_t _ttl,
				boost::function<void(VisitorHashd*)> _onSave,
				bool _saving);
	
	void initCurDevice(const std::string &_vdid, const std::string &_user_agent);
	void parseProtobuf(const std::string &_dump);
	void dump(std::string &_dump);
	
	std::string& getId(std::string &_vid);
	std::string getId();
	
	virtual void addQuery(const std::string &_q);
	virtual void getQueries(std::vector<std::string> &_q);
	virtual void save();
	virtual ~VisitorHashd();
};

typedef boost::shared_ptr<VisitorHashd> VisitorHashdPtr;

#endif
