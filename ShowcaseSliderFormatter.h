#ifndef _SHOWCASE_SLIDER_FORMATTER_H_
#define _SHOWCASE_SLIDER_FORMATTER_H_

#include "geber/cpp-client/GeberdCliApiClient.h"

#include "geber/cpp-client/GeberdCliApiClientAsync.h"

#include "hiaux/network/HttpOutReqDisp.h"
#include "formatter.h"

#include "FileCache.h"

#include <jansson.h>

#include "showcase.h"

#include "punkt_consts.h"

#include "zeit_client.h"

class ShowcaseSliderFormatterArgs: public FormatterArgs {
public:
	uint64_t shid;
	int nres;
	
	ShowcaseSliderFormatterArgs(uint64_t _shid, int _nres);
};

class ShowcaseSliderFormatter : public Formatter {
	HttpOutRequestDispPtr m_req_disp;
	FileCachePtr m_jscache;
	std::string m_punkt_url;
	boost::function<HttpSrv::ConnectionPtr(int)> m_getConnById;
	//GeberdCliApiClientPtr m_geber_cli;

	GeberdCliApiClientAsyncPtr m_geber_acli;

	boost::function<uint64_t(uint64_t)> m_getAdOwner;

	void handleShowDispEvent(HttpSrv::ConnectionPtr _conn, HttpSrv::RequestPtr _req);
	void handleItemsShowEvent(HttpSrv::ConnectionPtr _conn, HttpSrv::RequestPtr _req);
	void handleClickEvent(HttpSrv::ConnectionPtr _conn, HttpSrv::RequestPtr _req);
	
	void rebuildClickLinks(ShowcaseInstance &_show, uint64_t _pid, uint64_t _adid);
	
public:
	
	ShowcaseSliderFormatter(HttpOutRequestDispPtr _req_disp,
							FileCachePtr _jscache,
							const std::string &_punkt_url,
							boost::function<HttpSrv::ConnectionPtr(int)> _getConnById,
							GeberdCliApiClientAsyncPtr _geber_acli,
							boost::function<uint64_t(uint64_t)> _getAdOwner);
	
	void onGotShowcase(bool _success, ShowcaseInstance &_show, HttpSrv::ConnectionPtr _conn, uint64_t _pid, uint64_t _adid);
	void onGotShowcaseDemo(bool _success, ShowcaseInstance &_show, HttpSrv::ConnectionPtr _conn, uint64_t _pid, uint64_t _adid);
	
	virtual FormatterArgsPtr parseArgs(const std::string &_args_js);
	virtual void format(HttpSrv::ConnectionPtr _conn, HttpSrv::RequestPtr _req, uint64_t _pid, uint64_t _adid, FormatterArgsPtr _args);
	virtual void formatDemo(HttpSrv::ConnectionPtr _conn, HttpSrv::RequestPtr _req, uint64_t _pid, uint64_t _adid, FormatterArgsPtr _args);
	virtual void handleFormatEvent(HttpSrv::ConnectionPtr _conn, HttpSrv::RequestPtr _req);
	
	void onCalledZeitOk (int _connid, uint64_t _pid, uint64_t _adid, const std::string &_resp);
	void onCalledZeitFail (int _connid);
	
	//void onCalledGeberOk (int _connid, uint64_t _pid, uint64_t _adid, const std::string &_resp);
	//void onCalledGeberOkDemo (int _connid, uint64_t _pid, uint64_t _adid, const std::string &_resp);
	//void onCalledGeberFail (int _connid);
};

typedef boost::shared_ptr<ShowcaseSliderFormatter> ShowcaseSliderFormatterPtr;

#endif // _SHOWCASE_SLIDER_FORMATTER_H_
