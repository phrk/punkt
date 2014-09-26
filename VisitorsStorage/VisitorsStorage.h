#ifndef _VISITORS_STORAGE_H_
#define _VISITORS_STORAGE_H_

#include "hiconfig.h"
#include "VkProfile.h"
#include "VisitDevice.h"
#include "VisitorHashd.h"

#include "VisitorsStorageConsts.h"

#include "HashdClientAsync.h"

#include <boost/function.hpp>

class VisitorsStorage {
	HashdClientAsyncPtr m_hashd_acli;
	
	//uint64_t getUniquePropVid(const std::string &_prop);
	//void onGotUniquePropVid(bool _success, bool _exists, const std::string &_vid);
	
	//void mergeVisitors(uint64_t _vid0, uint64_t _vid1);
	
	void genVid(std::string &_vid);
	
	void getVisitor_onGotDeviceVid (int _err,
									const std::string &_vid,
									const std::string &_vdid,
									boost::function<void(VisitorPtr)> _onVisitor); 
	
	void saveVid(const std::string &_vdid, const std::string &_vid); 
	
public:
	
	VisitorsStorage(HashdClientAsyncPtr _hashd_acli);
	void getVisitor(const std::string &_vdid, boost::function<void(VisitorPtr)> _onVisitor);
	void saveVisitor(VisitorHashd *_visitor);
	
	void onSaved(int _err);
	void onGotVisitor(int _err,
						const std::string &_dump,
						bool _newdevice,
						const std::string &_vid,
						const std::string &_vdid,
						boost::function<void(VisitorPtr)> _onVisitor);
};

typedef boost::shared_ptr<VisitorsStorage> VisitorsStoragePtr;

#endif
