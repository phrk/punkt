#include "VisitorHashd.h"

VisitorHashd::VisitorHashd(const std::string &_vid,
					boost::function<void(VisitorHashd*)> _onSave,
					HttpSrv::ConnectionPtr _conn):
	m_onSave(_onSave),
	Visitor(_vid) {
	
}

void VisitorHashd::getId(std::string &_vid) {
	
}

void VisitorHashd::addQuery(const std::string &_q) {
	
}

void VisitorHashd::getQueries(std::vector<std::string> &_q) {
	
}

void VisitorHashd::save() {
	
}

VisitorHashd::~VisitorHashd() {
	
}
