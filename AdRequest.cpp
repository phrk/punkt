#include "AdRequest.h"

AdRequest::AdRequest(HttpConnectionPtr _conn, HttpRequestPtr _req,
					uint64_t _pid,
					uint64_t _adid,
					bool _https):
	 conn(_conn),
	 req(_req),
	 pid(_pid),
	 adid(_adid),
	 https(_https) {
	
}
