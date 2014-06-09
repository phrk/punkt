#ifndef _SHOWCASE_SIMPLE_REQUESTER_
#define _SHOWCASE_SIMPLE_REQUESTER_

#include "hiaux/network/HttpOutReqDisp.h"
#include "hiaux/network/HttpSrv.h"

class ShowcaseSimpleRequester : public HttpOutRequestDisp::Requester {
	
	std::string m_gebercall_url;
	boost::function<void(int, uint64_t, const std::string&)> m_onCalledGeberOk;
	boost::function<void(int)> m_onCalledGeberFail;
	uint64_t m_pid;

public:
	
	ShowcaseSimpleRequester(boost::function<void(int, int, const std::string&)> _onCall,
							boost::function<void(int)> _onFinished,
							int id,
							uint64_t _pid,
							const std::string &_gebercall_url,
							boost::function<void(int, uint64_t, const std::string&)> _onCalledGeberOk,
							boost::function<void(int)> _onCalledGeberFail);
	
	virtual void onCallDone (int _callid, bool _success, const std::string &_resp);
	virtual void start();
};

#endif
