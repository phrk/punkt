#include "punktd.h"

void onFinished() {

}

void Punktd::fallDown(std::string _s) {
	std::cout << _s << std::endl;
	exit(0);
}

hiaux::hashtable<std::string,std::string> Punktd::parseConfig(const std::string &_config_file) {
	hiaux::hashtable<std::string,std::string> ret;
	
	std::vector<std::string> required_params;
	required_params.push_back("listen_port");
	
	try {
		ret = LoadConf::load(_config_file, required_params);
	}
	catch (const std::string s) {
		fallDown(s);
	} catch (const char *s) {
		fallDown(s);
	} catch (...) {
		fallDown("Geberd::parseConfig: Could not parse config file: " + _config_file);
	}
		
	return ret;
}

Punktd::Punktd(const std::string &_config_file) {
	
	hiaux::hashtable<std::string,std::string> _config = parseConfig(_config_file);
	
	m_pool.reset(new hThreadPool(PUNKTD_NTHREADS));
	m_srv_tasklauncher.reset(new TaskLauncher(m_pool, PUNKTD_NTHREADS, boost::bind(&onFinished)));
	
	m_srv.reset(new HttpSrv(m_srv_tasklauncher,
							HttpSrv::ResponseInfo("text/html; charset=utf-8", "punktd"),
							boost::bind(&Punktd::connHandler, this, _1, _2)));

	m_punkt.reset(new Punkt(boost::bind(&HttpSrv::getHttpConnConst, m_srv.get(), _1)));

	m_srv->start(strtoint(_config["listen_port"]));
	m_pool->run();
}

void Punktd::connHandler(HttpSrv::ConnectionPtr _conn, HttpSrv::RequestPtr _req) {
	
	m_punkt->connHandler(_conn, _req);
}

void Punktd::join() {
	m_pool->join();
}
