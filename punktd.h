#ifndef _PUNKTD_H_
#define _PUNKTD_H_

#include "hiconfig.h"
#include "hiaux/network/HttpServer/HttpServer.h"
#include "hiaux/network/HttpOutReqDisp.h"
#include "hiaux/loadconf/loadconf.h"

#include "hiaux/network/HttpApi/BinClient/BinClientA.h"

#include "geber/cpp-client/GeberdCliApiClient.h"
#include "geber/cpp-client/GeberCliApiClientA.h"

#include "punkt_consts.h"

#ifdef PUNKT_TARGETER_COOKIE_ONLY
	#include "TargeterCookieOnly.h"
#endif

#ifdef PUNKT_TARGETER_FULL
	#include "TargeterFull.h"
	#include "VisitorsStorage/VisitorsStorage.h"
#endif

#include "punkt.h"
#include "formatter.h"
#include "FileCache.h"
#include "ShowcaseSimpleFormatter.h"
#include "ShowcaseSliderFormatter.h"
#include "StaticImageFormatter.h"

#include "libpq-fe.h"

#define PUNKTD_NTHREADS 8

class Punktd {

private:
	hThreadPoolPtr m_pool;
	TaskLauncherPtr m_srv_tasklauncher;
	HttpServerPtr m_srv;
	
	hiapi::client::BinClientAPtr m_hiapi_bin_clienta_geber;
	
#ifdef PUNKT_TARGETER_FULL
	HashdClientAsyncPtr m_hashd_acli;
	VisitorsStoragePtr m_visitors_storage;
	TargeterFullPtr m_targeter;
#endif
		
#ifdef PUNKT_TARGETER_COOKIE_ONLY
	TargeterCookieOnlyPtr m_targeter;
#endif
	
	PunktPtr m_punkt;
	HttpOutRequestDispPtr m_req_disp;
	//GeberdCliApiClientAsyncPtr m_geber_acli;
	
	GeberCliApiClientAPtr m_geber_clia;
	
	
	ZeitClientAsyncPtr m_zeit_acli;
	
	PGconn *m_pg;
	
	hAutoLock lock;
	
	FileCachePtr m_files_cache;
	
	hiaux::hashtable<uint64_t, FormatterPtr> m_formatters;
	uint64_t m_last_reload_ts;
	uint64_t m_reload_period;
	
	hiaux::hashtable<std::string,std::string> parseConfig(const std::string &_config_file);
	
	TaskLauncher::TaskRet clientLoop();
	
	void fallDown(std::string _s);
	void bindFormatters(const std::string &_punkt_url,
						const std::string &_punkt_rsrc_url);
	
	bool loadPlaceTarges(uint64_t _place, std::vector<uint64_t> &_ads_ids);
	void loadPlaces();
	bool loadAds();
	
	void checkReload();
	
	bool doCheckDbConn(size_t _attempt);
	bool checkDbConn();
public:
	
	FormatterArgsPtr parseFormatterArgs(uint64_t _format_id, const std::string &_args);
	
	void onFinished();
	Punktd(const std::string &_config_file);
	
	void connHandler(HttpConnectionPtr, HttpRequestPtr);
	void join();
};

#endif
