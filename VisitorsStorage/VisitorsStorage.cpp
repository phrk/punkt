#include "VisitorsStorage.h"

VisitorsStorage::VisitorsStorage(ZeitClientAsyncPtr _zeit_acli):
	m_zeit_acli(_zeit_acli) {
	
}

void VisitorsStorage::getVisitor(const std::string &_vdid, boost::function<void(VisitorPtr)> _onVisitor) {
		
}

void VisitorsStorage::saveVisitor(VisitorPtr _visitor) {
		
}
