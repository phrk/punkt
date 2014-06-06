#include "punkt.h"

Punkt::Punkt(HttpSrvPtr _srv):
	 m_srv(_srv) {
	
}

// connid, response 
cl_object Punkt::onLispHandlerFinished(cl_narg narg, ...) {
	
}

void Punkt::addHandler(const std::string &_path, const std::string &_lispcode) {
	
}

void Punkt::connHandler(HttpSrv::ConnectionPtr _conn, HttpSrv::RequestPtr _req) {
	
}
