#ifndef _SHOWCASE_SIMPLE_FORMATTER_H_
#define _SHOWCASE_SIMPLE_FORMATTER_H_

#include "geber/cpp-client/GeberdCliApiClient.h"
#include "hiaux/network/HttpOutReqDisp.h"
#include "formatter.h"
#include "ShowcaseSimpleRequester.h"
#include <jansson.h>

class ShowcaseSimpleFormatterArgs: public FormatterArgs {
public:
	uint64_t shid;
	int nres;
	
	ShowcaseSimpleFormatterArgs(uint64_t _shid, int _nres);
};

class ShowcaseSimpleFormatter : public Formatter {
	HttpOutRequestDispPtr m_req_disp;
	boost::function<HttpSrv::ConnectionPtr(int)> m_getConnById;
	GeberdCliApiClientPtr m_geber_cli;
public:
	
	ShowcaseSimpleFormatter(HttpOutRequestDispPtr _req_disp,
							boost::function<HttpSrv::ConnectionPtr(int)> _getConnById,
							GeberdCliApiClientPtr _geber_cli);
	
	virtual FormatterArgsPtr parseArgs(const std::string &_args_js);
	virtual void format(HttpSrv::ConnectionPtr _conn, HttpSrv::RequestPtr _req, uint64_t _pid, FormatterArgsPtr _args);
	virtual void formatDemo(HttpSrv::ConnectionPtr _conn, HttpSrv::RequestPtr _req, uint64_t _pid, FormatterArgsPtr _args);
	
	void onCalledGeberOk (int _connid, uint64_t _pid, const std::string &_resp);
	void onCalledGeberOkDemo (int _connid, uint64_t _pid, const std::string &_resp);
	void onCalledGeberFail (int _connid);
};

typedef boost::shared_ptr<ShowcaseSimpleFormatter> ShowcaseSimpleFormatterPtr;

#endif
