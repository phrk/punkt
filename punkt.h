#ifndef _PUNKT_H_
#define _PUNKT_H_

#include <ecl/ecl.h>

class Punkt {
	
	// path / lisp fun name
	hiaux::hashtable<std::string, std::string> m_handlers_names;
	
	HttpSrvPtr m_srv;
	
public:
	
	Punkt(HttpSrvPtr _http_srv);
	// connid, response 
	cl_object onLispHandlerFinished(cl_narg narg, ...);
	void addHandler(const std::string &_path, const std::string &_lispcode);
	void connHandler(HttpSrv::ConnectionPtr _conn, HttpSrv::RequestPtr _req);
};

typedef boost::shared_ptr<Punkt> PunktPtr;

#endif
