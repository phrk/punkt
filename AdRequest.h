#ifndef _AD_REQUEST_H_
#define _AD_REQUEST_H_

#include "hiconfig.h"
#include "hiaux/network/HttpSrv.h"
#include <boost/shared_ptr.hpp>

class AdRequest {
public:
	HttpSrv::ConnectionPtr conn;
	HttpSrv::RequestPtr req;
	uint64_t pid;
	uint64_t adid;
	bool https;
	std::vector<std::string> search_queries;
	
	AdRequest(HttpSrv::ConnectionPtr _conn,
						HttpSrv::RequestPtr _req,
						uint64_t _pid,
						uint64_t _adid,
						bool _https);
};

typedef boost::shared_ptr<AdRequest> AdRequestPtr;

#endif
