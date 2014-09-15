#include "Visitor.h"

Visitor::Visitor(const std::string &_vid):
	 m_vid(_vid) {
	
}

void Visitor::getId(std::string &_vid) {
	_vid = m_vid;
}

void Visitor::addUniqueProp(const std::string &_prop) {
	
	std::set<std::string>::iterator it = m_unique_props.find(_prop);
	if (it == m_unique_props.end()) {

		m_new_unique_props.push_back(_prop);
		m_unique_props.insert(_prop);
	}
}

Visitor::~Visitor() {
	
}

