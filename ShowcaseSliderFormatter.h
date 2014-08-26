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

#include "zeit_client_async.h"

class ShowcaseSliderFormatterArgs: public FormatterArgs {
public:
	uint64_t shid;
	int nitems;
	hiaux::hashtable<std::string, std::string> partner_ids;
	hiaux::hashtable<std::string, std::string> click_templates;
	std::string json_dump;
	
	ShowcaseSliderFormatterArgs(uint64_t _shid,
								int _nitems,
								const hiaux::hashtable<std::string, std::string> &_partner_ids,
								const hiaux::hashtable<std::string, std::string> &_click_templates,
								const std::string &_json_dump);
};

class ShowcaseSliderFormatter : public Formatter {
	HttpOutRequestDispPtr m_req_disp;
	FileCachePtr m_jscache;
	std::string m_punkt_url;
	boost::function<HttpSrv::ConnectionPtr(int)> m_getConnById;
	//GeberdCliApiClientPtr m_geber_cli;

	GeberdCliApiClientAsyncPtr m_geber_acli;
	ZeitClientAsyncPtr m_zeit_acli;
		
	boost::function<uint64_t(uint64_t)> m_getAdOwner;
	
	std::string m_punkt_rsrc_url;
	// Events
	void handleShowDispEvent(HttpSrv::ConnectionPtr _conn, HttpSrv::RequestPtr _req);
	void handleItemDispEvent(HttpSrv::ConnectionPtr _conn, HttpSrv::RequestPtr _req);
	void handleClickEvent(HttpSrv::ConnectionPtr _conn, HttpSrv::RequestPtr _req);
	void handleConvEvent(HttpSrv::ConnectionPtr _conn, HttpSrv::RequestPtr _req);
	
	void getPartnerParameter(const std::string &_advid,
							const hiaux::hashtable<std::string, std::string> &_partner_ids,
							std::string &_partner_param) const;
	
	void renderClickTemplate(const std::string &_advid,
														uint64_t _pid,
														uint64_t _adid,
														uint64_t _itemid,
														const std::string _itemurl,
														const hiaux::hashtable<std::string, std::string> &_click_templates,
														std::string &_target) const;
	
	void rebuildClickLinks(ShowcaseInstance &_show,
							uint64_t _pid,
							uint64_t _adid,
							const hiaux::hashtable<std::string, std::string> &_click_templates,
							const hiaux::hashtable<std::string, std::string> &_partner_ids);
	
public:
	
	ShowcaseSliderFormatter(HttpOutRequestDispPtr _req_disp,
							FileCachePtr _jscache,
							const std::string &_punkt_url,
							const std::string &_punkt_rsrc_url,
							boost::function<HttpSrv::ConnectionPtr(int)> _getConnById,
							GeberdCliApiClientAsyncPtr _geber_acli,
							ZeitClientAsyncPtr _zeit_acli,
							boost::function<uint64_t(uint64_t)> _getAdOwner);
	
	void onGotShowcase(bool _success, ShowcaseInstance &_show, AdRequestPtr _ad_req, FormatterArgsPtr _args);
	void onGotShowcaseDemo(bool _success, ShowcaseInstance &_show, AdRequestPtr _ad_req, FormatterArgsPtr _args);
	
	virtual FormatterArgsPtr parseArgs(const std::string &_args_js);
	virtual void format(AdRequestPtr _ad_req, FormatterArgsPtr _args);
	virtual void formatDemo(AdRequestPtr _ad_req, FormatterArgsPtr _args);
	
	virtual void handleFormatEvent(HttpSrv::ConnectionPtr _conn, HttpSrv::RequestPtr _req);
	
	void onCalledZeit (bool _success);
	
};

typedef boost::shared_ptr<ShowcaseSliderFormatter> ShowcaseSliderFormatterPtr;

#endif // _SHOWCASE_SLIDER_FORMATTER_H_
