#include "AdRequest.h"

AdRequest::AdRequest(HttpConnectionPtr _conn, HttpRequestPtr _req,
					const std::string &_location,
					const std::string &_domain,
					uint64_t _pid,
					bool _https):
	 conn(_conn),
	 location(_location),
	 domain(_domain),
	 req(_req),
	 pid(_pid),
	 adid(0),
	 https(_https) {
	
}
