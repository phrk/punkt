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
	
	required_params.push_back("reload_period");
	
	required_params.push_back("pg_ip");
	required_params.push_back("pg_port");
	required_params.push_back("pg_user");
	required_params.push_back("pg_pass");
	required_params.push_back("pg_db");
	
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

void Punktd::bindFormatters() {
	{
		FormatterPtr f(new ShowcaseSimpleFormatter(m_req_disp, boost::bind(&HttpSrv::getHttpConnConst, m_srv.get(), _1), m_geber_cli));
		m_punkt->updateFormatter(SHOWCASE_SIMPLE_FORMATTER_ID, f);
		m_formatters.insert(std::pair<uint64_t, FormatterPtr>(SHOWCASE_SIMPLE_FORMATTER_ID, f));
	}
	{
		m_jscache->addFile("renderShowcaseSlider.js", "js/renderShowcaseSlider.js");
		m_jscache->addFile("mootools", "js/mootools-core-1.5.0.js");
		m_jscache->addFile("slider.js", "js/buildSlider.js");
		m_jscache->addFile("ShowcaseSliderEvents.js", "js/ShowcaseSliderEvents.js");
		
		FormatterPtr f(new ShowcaseSliderFormatter(m_req_disp, m_jscache, boost::bind(&HttpSrv::getHttpConnConst, m_srv.get(), _1), m_geber_cli));
		m_punkt->updateFormatter(SHOWCASE_SLIDER_FORMATTER_ID, f);
		m_formatters.insert(std::pair<uint64_t, FormatterPtr>(SHOWCASE_SLIDER_FORMATTER_ID, f));
	}
}

void Punktd::checkReload() {
	uint64_t now = time(0);
	if (now - m_last_reload_ts < m_reload_period)
		return;
	
	loadAds();
	loadPlaces();
	
	m_last_reload_ts = time(0);
}

void Punktd::loadAds() {
	std::string query("SELECT * FROM punkt.ads");
	PGresult *res = PQexec(m_pg, query.c_str());
	
	if (PQresultStatus(res) != PGRES_TUPLES_OK) {
		std::cout << "Punktd::loadAds !PGRES_TUPLES_OK\n";
		return;
	}
	
	int ntuples = PQntuples(res);
	int ncols = PQnfields(res);
	
	uint64_t id;
	uint64_t format_id;
	std::string formatter_args;
	
	for (int t = 0; t<ntuples; t++) {	
		for (int c = 0; c < ncols; c++) {
		
			if (std::string("id") == PQfname(res, c)) {
				id = string_to_uint64( PQgetvalue(res, t, c) );
			}
		
			if (std::string("format_id") == PQfname(res, c)) {
				format_id = string_to_uint64( PQgetvalue(res, t, c) );
			}
			if (std::string("formatter_args") == PQfname(res, c)) {
				formatter_args = PQgetvalue(res, t, c);
			}
		}
		AdPtr ad (new Ad(id, format_id, formatter_args));
		m_punkt->updateAd(ad);
	}
	
}

void Punktd::loadPlaceTarges(uint64_t _place, std::vector<uint64_t> &_ads_ids) {
	
	std::string query("SELECT ad_id FROM punkt.targets WHERE place_id=" + uint64_to_string(_place));
	PGresult *res = PQexec(m_pg, query.c_str());
	
	if (PQresultStatus(res) != PGRES_TUPLES_OK) {
		std::cout << "Punktd::loadPlaceTarges !PGRES_TUPLES_OK _place: " << _place << std::endl;
		return;
	}
	
	int ntuples = PQntuples(res);
	int ncols = PQnfields(res);
	
	for (int t = 0; t<ntuples; t++) {	
		for (int c = 0; c < ncols; c++) {
		
			if (std::string("ad_id") == PQfname(res, c))
				_ads_ids.push_back( string_to_uint64( PQgetvalue(res, t, c) ) );
		}
	}
	PQclear(res);
}

void Punktd::loadPlaces() {

	std::string query  ("SELECT * FROM punkt.places");
	PGresult *res = PQexec(m_pg, query.c_str());
	
	if (PQresultStatus(res) != PGRES_TUPLES_OK) {
		std::cout << "Punktd::loadPlaces !PGRES_TUPLES_OK\n";
		return;
	}
	
	int ntuples = PQntuples(res);
	int ncols = PQnfields(res);
	
	uint64_t id;
	bool readok = false;
	
	for (int t = 0; t<ntuples; t++) {	
		readok = false;
		for (int c = 0; c < ncols; c++) {
			if (std::string("id") == PQfname(res, c)) {
				id = string_to_uint64(PQgetvalue(res, t, c));
				readok = true;
			}		
		}
		
		if (!readok) {
			std::cout << "Punktd::loadPlaces could not load place id\n";
			continue;
		}
		
		
		std::vector<uint64_t> ads_ids;
		loadPlaceTarges(id, ads_ids);
		m_punkt->updatePlaceTargets(id, ads_ids);
	}
	
	PQclear(res);
}

Punktd::Punktd(const std::string &_config_file) {
	
	m_last_reload_ts = 0;
	
	hiaux::hashtable<std::string,std::string> _config = parseConfig(_config_file);
	
	m_reload_period = string_to_uint64(_config["reload_period"]);
	
	m_pg  = PQsetdbLogin(_config["pg_ip"].c_str(),
                      _config ["pg_port"].c_str(),
                     "",
                     "",
                     _config ["pg_user"].c_str(),
                     _config ["pg_pass"].c_str(),
                     _config ["pg_db"].c_str());
					 
	if (PQstatus(m_pg) != CONNECTION_OK) {
		std::cout << "Could not connect to db";
		exit(0);
	}
	
	m_jscache.reset(new FileCache);
	
	m_geber_cli.reset(new GeberdCliApiClient(_config["geberd_url"]));
	
	m_punkt.reset(new Punkt());
	
	m_pool.reset(new hThreadPool(PUNKTD_NTHREADS));
	m_srv_tasklauncher.reset(new TaskLauncher(m_pool, PUNKTD_NTHREADS, boost::bind(&onFinished)));
	
	m_req_disp.reset(new HttpOutRequestDisp(m_srv_tasklauncher));
	
	m_srv.reset(new HttpSrv(m_srv_tasklauncher,
							HttpSrv::ResponseInfo("text/html; charset=utf-8", "punktd"),
							boost::bind(&Punktd::connHandler, this, _1, _2)));

	//m_punkt.reset(new Punkt(boost::bind(&HttpSrv::getHttpConnConst, m_srv.get(), _1)));
							
	bindFormatters();
	std::cout << "Formatters binded\n";
	loadAds();
	std::cout << "Ads loaded\n";
	loadPlaces();
	std::cout << "Places loaded\n";
	
	std::cout << "All loaded\n";
	
	m_srv->start(strtoint(_config["listen_port"]));
	m_pool->run();
}

void Punktd::connHandler(HttpSrv::ConnectionPtr _conn, HttpSrv::RequestPtr _req) {
	
	checkReload();
	m_punkt->connHandler(_conn, _req);
}

void Punktd::join() {
	m_pool->join();
}
