#ifndef _PUNKTD_H_
#define _PUNKTD_H_

#include "hiconfig.h"
#include "hiaux/network/HttpSrv.h"
#include "hiaux/network/HttpOutReqDisp.h"
#include "hiaux/loadconf/loadconf.h"

#include "geber/cpp-client/GeberdCliApiClient.h"

#include "punkt_consts.h"

#include "punkt.h"
#include "formatter.h"
#include "FileCache.h"
#include "ShowcaseSimpleFormatter.h"
#include "ShowcaseSliderFormatter.h"

#include "libpq-fe.h"

#define PUNKTD_NTHREADS 4

class Punktd {	
	
private:
	hThreadPoolPtr m_pool;
	TaskLauncherPtr m_srv_tasklauncher;
	HttpSrvPtr m_srv;
	PunktPtr m_punkt;
	HttpOutRequestDispPtr m_req_disp;
	//GeberdCliApiClientPtr m_geber_cli;
	GeberdCliApiClientAsyncPtr m_geber_acli;
	PGconn *m_pg;
	
	FileCachePtr m_jscache;
	
	hiaux::hashtable<uint64_t, FormatterPtr> m_formatters;
	uint64_t m_last_reload_ts;
	uint64_t m_reload_period;
	
	hiaux::hashtable<std::string,std::string> parseConfig(const std::string &_config_file);
	void fallDown(std::string _s);
	void bindFormatters(const std::string &_punkt_url);
	
	void loadPlaceTarges(uint64_t _place, std::vector<uint64_t> &_ads_ids);
	void loadPlaces();
	void loadAds();
	
	void checkReload();
public:
	
	void onFinished();
	Punktd(const std::string &_config_file);
	
	void connHandler(HttpSrv::ConnectionPtr, HttpSrv::RequestPtr);
	void join();
};

#endif
