#ifndef _SHOWCASE_SIMPLE_FORMATTER_H_
#define _SHOWCASE_SIMPLE_FORMATTER_H_

#include "geber/cpp-client/GeberdCliApiClient.h"
#include "hiaux/network/HttpOutReqDisp.h"
#include "formatter.h"
#include "ShowcaseSimpleRequester.h"

class ShowcaseSimpleFormatter : public Formatter {
	HttpOutRequestDispPtr m_req_disp;
	boost::function<HttpSrv::ConnectionPtr(int)> m_getConnById;
	GeberdCliApiClientPtr m_geber_cli;
public:
	
	ShowcaseSimpleFormatter(HttpOutRequestDispPtr _req_disp,
							boost::function<HttpSrv::ConnectionPtr(int)> _getConnById,
							GeberdCliApiClientPtr _geber_cli);
							
	virtual void format(HttpSrv::ConnectionPtr _conn, HttpSrv::RequestPtr _req, const std::string &_args);
	
	void onCalledGeberOk (int _connid, const std::string &_resp);
	void onCalledGeberFail (int _connid);
};

typedef boost::shared_ptr<ShowcaseSimpleFormatter> ShowcaseSimpleFormatterPtr;

#endif
