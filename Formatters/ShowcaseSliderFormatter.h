#ifndef _SHOWCASE_SLIDER_FORMATTER_H_
#define _SHOWCASE_SLIDER_FORMATTER_H_

#include "geber/cpp-client/GeberCliApiClientA.h"

#include "hiaux/network/HttpOutReqDisp.h"
#include "formatter.h"

#include "FileCache.h"

#include <jansson.h>

#include "showcase.h"

#include "punkt_consts.h"

#include "ZeitClientA.h"

class ShowcaseSliderFormatterArgs: public FormatterArgs {
public:
	uint64_t shid;
	int nitems;
	hiaux::hashtable<std::string, std::string> partner_ids;
	hiaux::hashtable<std::string, std::string> click_templates;
	
	std::string system_depot_url_prefix;
	std::string json_dump;
	std::string type;
	
	ShowcaseSliderFormatterArgs(uint64_t _shid,
								int _nitems,
								const hiaux::hashtable<std::string, std::string> &_partner_ids,
								const hiaux::hashtable<std::string, std::string> &_click_templates,
								const std::string &_system_url,
								const std::string &_system_rsrc_url,
								const std::string &_system_depot_url_prefix,
								const std::string &_type,
								const std::string &_json_dump);
								
	virtual ~ShowcaseSliderFormatterArgs();
};

class ShowcaseSliderFormatter : public Formatter {
	//HttpOutRequestDispPtr m_req_disp;
	FileCachePtr m_jscache;
	//std::string m_punkt_url;
//	boost::function<HttpConnectionPtr(int)> m_getConnById;
	//GeberdCliApiClientPtr m_geber_cli;

	GeberCliApiClientAPtr m_geber_clia;
	//GeberdCliApiClientAsyncPtr m_geber_acli;
	ZeitClientAPtr m_zeit_acli;
		
	boost::function<uint64_t(uint64_t)> m_getAdOwner;
	
	//std::string m_punkt_rsrc_url;
	// Events
	void handleShowDispEvent(HttpConnectionPtr _conn, HttpRequestPtr _req);
	void handleItemDispEvent(HttpConnectionPtr _conn, HttpRequestPtr _req);
	void handleClickEvent(HttpConnectionPtr _conn, HttpRequestPtr _req);
	void handleConvEvent(HttpConnectionPtr _conn, HttpRequestPtr _req);
	
	void getPartnerParameter(const std::string &_advid,
							const hiaux::hashtable<std::string, std::string> &_partner_ids,
							std::string &_partner_param) const;
	
	void renderClickTemplate(const std::string &_advid,
														uint64_t _pid,
														uint64_t _adid,
														uint64_t _itemid,
														const std::string _itemurl,
														const hiaux::hashtable<std::string, std::string> &_click_templates,
														const std::string &_system_url,
														std::string &_target) const;
	
	void rebuildClickLinks(ShowcaseInstance &_show,
							uint64_t _pid,
							uint64_t _adid,
							const hiaux::hashtable<std::string, std::string> &_click_templates,
							const hiaux::hashtable<std::string, std::string> &_partner_ids,
							const std::string &_system_url);
							
	void rebuildImageLinks(ShowcaseInstance &_show, const std::string &_system_depot_url_prefix);
	
public:
	
	ShowcaseSliderFormatter(//HttpOutRequestDispPtr _req_disp,
							FileCachePtr _jscache,
							GeberCliApiClientAPtr _geber_clia,
							ZeitClientAPtr _zeit_acli,
							boost::function<uint64_t(uint64_t)> _getAdOwner);
	
	void onGotShowcase(bool _success, ShowcaseInstance &_show, AdRequestPtr _ad_req, FormatterArgsPtr _args, const std::string &_exthtml,
											const std::string &_extjs);
	void onGotShowcaseDemo(bool _success, ShowcaseInstance &_show, AdRequestPtr _ad_req, FormatterArgsPtr _args);
	
	virtual FormatterArgsPtr parseArgs(const std::string &_args_js);
	virtual void format(AdRequestPtr _ad_req, FormatterArgsPtr _args, const std::string &_exthtml, const std::string &_extjs);
	virtual void formatDemo(AdRequestPtr _ad_req, FormatterArgsPtr _args);
	
	virtual void handleFormatEvent(HttpConnectionPtr _conn, HttpRequestPtr _req);
	virtual void handleTargeterEvent(const std::string &_method,
									uint64_t _pid,
									uint64_t _adid,
									const std::map<std::string, std::string> &_params,
									HttpConnectionPtr _conn,
									HttpRequestPtr _req);
	
	void onCalledZeit (bool _success);
	virtual ~ShowcaseSliderFormatter();
};

typedef boost::shared_ptr<ShowcaseSliderFormatter> ShowcaseSliderFormatterPtr;

#endif // _SHOWCASE_SLIDER_FORMATTER_H_
