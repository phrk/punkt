#include "VisitorHashd.h"

VisitorHashd::VisitorHashd(const std::string &_vid,
						const std::string &_vdid,
						boost::function<void(VisitorHashd*)> _onSave,
						bool _newdevice):
	cur_vdid(_vdid),
	m_onSave(_onSave),
	Visitor(_vid),
	newdevice(_newdevice) {
	
}

std::string& VisitorHashd::getId(std::string &_vid) {
	
	_vid = m_vid;
	return _vid;
}

std::string VisitorHashd::getId() {
	
	return m_vid;
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
