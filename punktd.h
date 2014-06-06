#ifndef _PUNKTD_H_
#define _PUNKTD_H_

#include "hiconfig.h"
#include "hiaux/network/HttpSrv.h"
#include "hiaux/loadconf/loadconf.h"

#include "punkt.h"

#define PUNKTD_NTHREADS 4

class Punktd {
	hThreadPoolPtr m_pool;
	TaskLauncherPtr m_srv_tasklauncher;
	HttpSrvPtr m_srv;

	hiaux::hashtable<std::string,std::string> parseConfig(const std::string &_config_file);
	void fallDown(std::string _s);
public:
	
	Punktd(const std::string &_config_file);
	
	void connHandler(HttpSrv::ConnectionPtr, HttpSrv::RequestPtr);
	void join();
};

#endif
