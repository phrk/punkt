#ifndef _PUNKTD_H_
#define _PUNKTD_H_

#include "hiconfig.h"
#include "hiaux/network/HttpSrv.h"
#include "hiaux/network/HttpOutReqDisp.h"
#include "hiaux/loadconf/loadconf.h"

#include "geber/cpp-client/GeberdCliApiClient.h"

#include "punkt.h"
#include "formatter.h"
#include "ShowcaseSimpleFormatter.h"

#define PUNKTD_NTHREADS 4

class Punktd {	
	
private:
	hThreadPoolPtr m_pool;
	TaskLauncherPtr m_srv_tasklauncher;
	HttpSrvPtr m_srv;
	PunktPtr m_punkt;
	HttpOutRequestDispPtr m_req_disp;
	
	GeberdCliApiClientPtr m_geber_cli;
	
	std::vector<FormatterPtr> m_formatters;
	
	hiaux::hashtable<std::string,std::string> parseConfig(const std::string &_config_file);
	void fallDown(std::string _s);
	void loadFormatters();
public:
	
	Punktd(const std::string &_config_file);
	
	void connHandler(HttpSrv::ConnectionPtr, HttpSrv::RequestPtr);
	void join();
};

#endif
