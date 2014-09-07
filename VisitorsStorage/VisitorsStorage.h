#ifndef _VISITORS_STORAGE_H_
#define _VISITORS_STORAGE_H_

#include "hiconfig.h"
#include "VkProfile.h"
#include "VisitDevice.h"
#include "VisitorHashd.h"

#include "HashdClientAsync.h"

#include <boost/function.hpp>

class VisitorsStorage {
	HashdClientAsyncPtr m_hashd_acli;
public:
	
	VisitorsStorage(HashdClientAsyncPtr _hashd_acli);
	void getVisitor(const std::string &_vdid, boost::function<void(bool,VisitorPtr)> _onVisitor);
	void saveVisitor(VisitorPtr _visitor);
};

typedef boost::shared_ptr<VisitorsStorage> VisitorsStoragePtr;

#endif
