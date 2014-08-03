#ifndef _VISITORS_STORAGE_H_
#define _VISITORS_STORAGE_H_

#include "hiconfig.h"
#include "VkProfile.h"
#include "VisitDevice.h"
#include "Visitor.h"

class VisitorsStorage {
	ZeitClientAsyncPtr m_zeit_acli;
public:
	
	VisitorsStorage(ZeitClientAsyncPtr _zeit_acli);
	void getVisitor(const std::string &_vdid, boost::function<void(VisitorPtr)> _onVisitor);
	void saveVisitor(VisitorPtr _visitor);
};

typedef boost::shared_ptr<VisitorsStorage> VisitorsStoragePtr;

#endif
