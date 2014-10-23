#ifndef _AD_REQUEST_H_
#define _AD_REQUEST_H_

#include "hiconfig.h"
#include "hiaux/network/HttpServer/HttpServer.h"
#include <boost/shared_ptr.hpp>

class AdRequest {
public:
	HttpConnectionPtr conn;
	HttpRequestPtr req;
	std::string location;
	std::string domain;
	uint64_t pid;
	uint64_t adid;
	bool https;
	std::vector<std::string> search_queries;
	
	AdRequest(HttpConnectionPtr _conn, HttpRequestPtr _req,
						const std::string &_location,
						const std::string &_domain,
						uint64_t _pid,
						bool _https);
};

typedef boost::shared_ptr<AdRequest> AdRequestPtr;

#endif
