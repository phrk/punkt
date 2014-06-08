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
	required_params.push_back("geberd_url");
	
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

void Punktd::loadFormatters() {
	
	FormatterPtr f(new ShowcaseSimpleFormatter(m_req_disp, boost::bind(&HttpSrv::getHttpConnConst, m_srv.get(), _1), m_geber_cli));
	m_punkt->updateFormatter(0, (Punkt::FormatterFun)boost::bind(&Formatter::format, f.get(), _1, _2, _3));
	m_formatters.push_back(f);
	
	m_punkt->updatePlace(0, Punkt::PlaceInfoPtr(new Punkt::PlaceInfo(0, "{ \"shid\": \"2\"}")));
}

Punktd::Punktd(const std::string &_config_file) {
	
	hiaux::hashtable<std::string,std::string> _config = parseConfig(_config_file);
	
	m_geber_cli.reset(new GeberdCliApiClient(_config["geberd_url"]));
	
	m_punkt.reset(new Punkt());
	
	m_pool.reset(new hThreadPool(PUNKTD_NTHREADS));
	m_srv_tasklauncher.reset(new TaskLauncher(m_pool, PUNKTD_NTHREADS, boost::bind(&onFinished)));
	
	m_req_disp.reset(new HttpOutRequestDisp(m_srv_tasklauncher));
	
	m_srv.reset(new HttpSrv(m_srv_tasklauncher,
							HttpSrv::ResponseInfo("text/html; charset=utf-8", "punktd"),
							boost::bind(&Punktd::connHandler, this, _1, _2)));

	//m_punkt.reset(new Punkt(boost::bind(&HttpSrv::getHttpConnConst, m_srv.get(), _1)));
	
	loadFormatters();
	
	m_srv->start(strtoint(_config["listen_port"]));
	m_pool->run();
}

void Punktd::connHandler(HttpSrv::ConnectionPtr _conn, HttpSrv::RequestPtr _req) {
	
	m_punkt->connHandler(_conn, _req);
}

void Punktd::join() {
	m_pool->join();
}
