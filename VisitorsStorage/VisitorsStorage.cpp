#include "VisitorsStorage.h"

VisitorsStorage::VisitorsStorage(HashdClientAsyncPtr _hashd_acli):
	m_hashd_acli(_hashd_acli) {
	
}

void VisitorsStorage::getVisitor(const std::string &_vdid, boost::function<void(VisitorPtr)> _onVisitor) {
	
	m_hashd_acli->get("visitors", _vdid, boost::bind(&VisitorsStorage::onGotVisitor, this, _1, _2, _3, _vdid, _onVisitor));
}

void VisitorsStorage::onGotVisitor(bool _success, bool _exists, const std::string &_dump, const std::string &_vid, boost::function<void(VisitorPtr)> _onVisitor) {
		
	VisitorHashdPtr v(new VisitorHashd(_vid, boost::bind(&VisitorsStorage::saveVisitor, this, _1)));
	
	if (!_success) {
		_onVisitor(v);
		return;
	}
	
	
	v->parseProtobuf(_dump);
	_onVisitor(v);
}

void VisitorsStorage::saveVisitor(VisitorHashd *_visitor) {
	
	std::string dump, vid;
	_visitor->dump(dump);
	_visitor->getId(vid);
		
	m_hashd_acli->set("visitors", vid, dump, boost::bind(&VisitorsStorage::onSaved, this, _1));
}

void VisitorsStorage::onSaved(bool _success) {
	
//	std::cout << "VisitorsStorage::onSaved " << _success << std::endl;
}
