#include "VisitorHashd.h"

VisitorHashd::VisitorHashd(const std::string &_vid,
					boost::function<void(VisitorHashd*)> _onSave):
	m_onSave(_onSave),
	Visitor(_vid) {
	
}

void VisitorHashd::getId(std::string &_vid) {
	
	_vid = m_vid;
}

void VisitorHashd::dump(std::string &_dump) {
	
}

void VisitorHashd::parseProtobuf(const std::string &_dump) {
	
}

void VisitorHashd::addQuery(const std::string &_q) {
	
}

void VisitorHashd::getQueries(std::vector<std::string> &_q) {
	
}

void VisitorHashd::save() {
	
	m_onSave(this);
}

VisitorHashd::~VisitorHashd() {
	
}
