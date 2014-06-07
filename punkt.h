#ifndef _PUNKT_H_
#define _PUNKT_H_

#include <ecl/ecl.h>
#include <boost/function.hpp>
#include "hiaux/network/HttpSrv.h"

class Punkt {
	
	// path / lisp fun name
	hiaux::hashtable<std::string, std::string> m_handlers_names;
	boost::function<HttpSrv::ConnectionPtr(int)> m_getConnById;
public:
	
	static Punkt* single;
	
	Punkt(boost::function<HttpSrv::ConnectionPtr(int)> _getConnById);
	// connid, response 
	void onLispFormatterFinished(int _connid, const std::string &_resp);
	void onLispSourceUpdate(const std::string &_lispcode);
	void connHandler(HttpSrv::ConnectionPtr _conn, HttpSrv::RequestPtr _req);
};

typedef boost::shared_ptr<Punkt> PunktPtr;

#endif
