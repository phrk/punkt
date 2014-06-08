#include "ShowcaseSimpleRequester.h"

ShowcaseSimpleRequester::ShowcaseSimpleRequester(boost::function<void(int, int, const std::string&)> _onCall,
									boost::function<void(int)> _onFinished,
									int _id,
									const std::string &_gebercall_url,
									boost::function<void(int, const std::string&)> _onCalledGeberOk,
									boost::function<void(int)> _onCalledGeberFail):

		HttpOutRequestDisp::Requester(_onCall, _onFinished, _id),
		m_gebercall_url(_gebercall_url),
		m_onCalledGeberOk(_onCalledGeberOk),
		m_onCalledGeberFail(_onCalledGeberFail) {

}

void ShowcaseSimpleRequester::onCallDone (int _callid, bool _success, const std::string &_resp) {
	
	if (!_success) {
		m_onCalledGeberFail(m_id);
		finished();
	}
	else {
		m_onCalledGeberOk(m_id, _resp);
		finished();
	}
}

void ShowcaseSimpleRequester::start() {
		
	call(0, m_gebercall_url);
}
