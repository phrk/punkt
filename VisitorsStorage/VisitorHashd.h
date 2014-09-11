#ifndef _VISITOR_HASHD_H_
#define _VISITOR_HASHD_H_

#include "hiconfig.h"
#include "VisitDevice.h"
#include "VkProfile.h"
#include "Visitor.pb.h"

#include <boost/function.hpp>

#include "Visitor.h"
#include "Targeter.h"

class VisitorExt {
public:
	uint32_t extid;
	std::string data;
};

class VisitorHashd : public Visitor {
	boost::function<void(VisitorHashd*)> m_onSave;
public:
	VkProfilePtr vk_profile;
	std::vector<VisitDevice> devices;
	std::vector<VisitorExt> ext;
	
	bool newdevice;
	std::string cur_vdid;
	
	VisitorHashd(const std::string &_vid,
					const std::string &_vdid,
						boost::function<void(VisitorHashd*)> _onSave,
						bool _newdevice);
	
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
