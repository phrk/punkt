#include "Targeter.h"

Visitor::Visitor(const std::string &_vid):
	 m_vid(_vid) {
	
}

void Visitor::getId(std::string &_vid) {
	_vid = m_vid;
}

Visitor::~Visitor() {
	
}

Targeter::Targeter(const std::string &_repl_id):
	 m_repl_id(_repl_id) {
}

Targeter::~Targeter() {
	
}
