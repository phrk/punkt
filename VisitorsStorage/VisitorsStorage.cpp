#include "VisitorsStorage.h"

VisitorsStorage::VisitorsStorage(HashdClientAsyncPtr _hashd_acli):
	m_hashd_acli(_hashd_acli) {
	
}

void VisitorsStorage::getVisitor(const std::string &_vdid, boost::function<void(bool,VisitorPtr)> _onVisitor) {
		
}

void VisitorsStorage::saveVisitor(VisitorPtr _visitor) {
		
}
