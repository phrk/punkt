#include "punktd.h"

void Punktd::onFinished() {

}

void Punktd::fallDown(std::string _s) {
	std::cout << _s << std::endl;
	exit(0);
}

hiaux::hashtable<std::string,std::string> Punktd::parseConfig(const std::string &_config_file) {
	hiaux::hashtable<std::string,std::string> ret;
	
	std::vector<std::string> required_params;
	std::vector<std::string> optional_params;
	required_params.push_back("systemid");
	required_params.push_back("replid");
	
	required_params.push_back("listen_port");
	required_params.push_back("punkt_url");
	required_params.push_back("punkt_rsrc_url");
	required_params.push_back("geberd_url");
	required_params.push_back("zeit_url");
	required_params.push_back("hashd_url");
	
	required_params.push_back("reload_period");
	
	required_params.push_back("pg_ip");
	required_params.push_back("pg_port");
	required_params.push_back("pg_user");
	required_params.push_back("pg_pass");
	required_params.push_back("pg_db");
		
	try {
		ret = LoadConf::load(_config_file, required_params, optional_params);
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

void Punktd::bindFormatters(const std::string &_punkt_url,
							const std::string &_punkt_rsrc_url) {
	{
//		FormatterPtr f(new ShowcaseSimpleFormatter(m_req_disp, boost::bind(&HttpSrv::getHttpConnConst, m_srv.get(), _1), m_geber_cli));
//		m_punkt->updateFormatter(SHOWCASE_SIMPLE_FORMATTER_ID, f);
//		m_formatters.insert(std::pair<uint64_t, FormatterPtr>(SHOWCASE_SIMPLE_FORMATTER_ID, f));
	}
	{
		
		FormatterPtr f(new ShowcaseSliderFormatter(m_req_disp,
													m_files_cache,
													_punkt_url,
													_punkt_rsrc_url,
													m_geber_acli,
													m_zeit_acli,
													boost::bind(&Punkt::getAdOwner, m_punkt.get(), _1) ));
																								
		m_punkt->updateFormatter(SHOWCASE_SLIDER_FORMATTER_ID, f);
		m_punkt->updateFormatter(SHOWCASE_SLIDER_304_224_FORMATTER_ID, f);
		m_punkt->updateFormatter(SHOWCASE_SLIDER_520_250_4_ITEMS_FORMATTER_ID, f);
		
		m_formatters.insert(std::pair<uint64_t, FormatterPtr>(SHOWCASE_SLIDER_FORMATTER_ID, f));
		m_formatters.insert(std::pair<uint64_t, FormatterPtr>(SHOWCASE_SLIDER_304_224_FORMATTER_ID, f));
		m_formatters.insert(std::pair<uint64_t, FormatterPtr>(SHOWCASE_SLIDER_520_250_4_ITEMS_FORMATTER_ID, f));
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

bool Punktd::doCheckDbConn(size_t _attempt) {
	
	if (_attempt > 10)
		return false;
	
	if (PQstatus(m_pg) != CONNECTION_OK) {
		
		std::cout << "Reseting connection to PostgreSQL\n;";
		PQreset(m_pg);
		return doCheckDbConn(_attempt + 1);
	}
	return true;
}

bool Punktd::checkDbConn() {
	
	return doCheckDbConn(0);
}

void Punktd::loadAds() {
	std::string query("SELECT * FROM punkt.ads");
	PGresult *res = PQexec(m_pg, query.c_str());
	
	if (PQresultStatus(res) != PGRES_TUPLES_OK) {
		std::cout << "Punktd::loadAds !PGRES_TUPLES_OK\n";
		
		//checkDbConn();
		return;
	}
	
	int ntuples = PQntuples(res);
	int ncols = PQnfields(res);
	
	uint64_t id;
	uint64_t format_id;
	std::string formatter_args;
	std::string targeter_args;
	uint64_t ad_owner;
	
	for (int t = 0; t<ntuples; t++) {	
		for (int c = 0; c < ncols; c++) {
		
			if (std::string("id") == PQfname(res, c)) {
				id = string_to_uint64( PQgetvalue(res, t, c) );
			}
			
			if (std::string("ad_owner") == PQfname(res, c)) {
				ad_owner = string_to_uint64( PQgetvalue(res, t, c) );
			}
			
			if (std::string("format_id") == PQfname(res, c)) {
				format_id = string_to_uint64( PQgetvalue(res, t, c) );
			}
			if (std::string("formatter_args") == PQfname(res, c)) {
				formatter_args = PQgetvalue(res, t, c);
			}
			
			if (std::string("targeter_args") == PQfname(res, c)) {
				targeter_args = PQgetvalue(res, t, c);
			}
		}
		//AdPtr ad (new Ad(id, format_id, ad_owner, formatter_args, targeter_args));
		
		m_targeter->updateAd(id, format_id, ad_owner, formatter_args, targeter_args);
	}
	
}

bool Punktd::loadPlaceTarges(uint64_t _place, std::vector<uint64_t> &_ads_ids) {
	
	std::string query("SELECT ad_id FROM punkt.targets WHERE place_id=" + uint64_to_string(_place));
	PGresult *res = PQexec(m_pg, query.c_str());
	
	if (PQresultStatus(res) != PGRES_TUPLES_OK) {
		std::cout << "Punktd::loadPlaceTarges !PGRES_TUPLES_OK _place: " << _place << std::endl;
		return false;
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
	return true;
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
	std::string targeter_args;
	//bool readok = false;
	
	for (int t = 0; t<ntuples; t++) {	
		//readok = false;
		for (int c = 0; c < ncols; c++) {
			if (std::string("id") == PQfname(res, c)) {
				id = string_to_uint64(PQgetvalue(res, t, c));
				//readok = true;
			}
			
			if (std::string("targeter_args") == PQfname(res, c)) {
				
				targeter_args = std::string(PQgetvalue(res, t, c));
			}
			
		}
		
//		if (!readok) {
//			std::cout << "Punktd::loadPlaces could not load place id\n";
//			continue;
//		}
		
		std::vector<uint64_t> ads_ids;
		if (loadPlaceTarges(id, ads_ids)) {
			m_targeter->updatePlace(id, targeter_args, ads_ids);
		}
	}
	
	PQclear(res);
}
/*
void Punktd::getClickTemplates(const hiaux::hashtable<std::string,std::string> &_config) {
	
	hiaux::hashtable<std::string, std::string>::const_iterator it = _config.find("wikimart_click_template");
	if (it != _config.end())
		_click_templates[WIKIMART_ADVID] = it->second;
	
	it = _config.find("ozon_click_template");
	if (it != _config.end())
		_click_templates[OZON_ADVID] = it->second;
}*/

FormatterArgsPtr Punktd::parseFormatterArgs(uint64_t _format_id, const std::string &_args) {
		
	std::cout << "Punktd::parseFormatterArgs\n";
	
	FormatterArgsPtr ret;
	
	hLockTicketPtr ticket = lock.lock();
	
	hiaux::hashtable<uint64_t, FormatterPtr>::iterator f_it = m_formatters.find(_format_id);
	
	if (f_it == m_formatters.end()) {
		std::cout << "Punkt::parseFormatterArgs Unknown format " << _format_id << std::endl;
		return ret;
	}
	
	try {
		ret = f_it->second->parseArgs(_args);
	} catch (...) {
		std::cout << "Punkt::parseFormatterArgs parseArgs exception format " << _format_id << std::endl;
	}
	
	std::cout << "Punktd::parseFormatterArgs finished\n";
	
	return ret;
}

Punktd::Punktd(const std::string &_config_file) {
	
	m_last_reload_ts = 0;
	
	hiaux::hashtable<std::string,std::string> _config = parseConfig(_config_file);
	
	std::cout << "Config loaded\n";
	
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
	
	std::cout << "Connected to db\n";
	
	m_files_cache.reset(new FileCache);
	m_files_cache->addFile("renderShowcaseSlider.js", "js/renderShowcaseSlider.js");
	m_files_cache->addFile("mootools", "js/mootools-core-1.5.0.js");
	m_files_cache->addFile("slider.js", "js/buildSlider.js");
	m_files_cache->addFile("ShowcaseSliderEvents.js", "js/ShowcaseSliderEvents.js");
	m_files_cache->addFile("vkauth.html", "TargeterFull/vkauth.html");
	m_files_cache->apply("vkauth.html", boost::bind(&escape_symbol, _1, '\"'));
	m_files_cache->apply("vkauth.html", boost::bind(&add_newline_backslash, _1));
	m_files_cache->addFile("vkauth.js", "TargeterFull/vkauth.js");
	
	m_pool.reset(new hThreadPool(PUNKTD_NTHREADS));
	m_srv_tasklauncher.reset(new TaskLauncher(m_pool, PUNKTD_NTHREADS, boost::bind(&Punktd::onFinished, this)));
	
	m_req_disp.reset(new HttpOutRequestDisp(m_srv_tasklauncher));

	m_geber_acli.reset(new GeberdCliApiClientAsync(_config["geberd_url"], m_req_disp));
	m_zeit_acli.reset(new ZeitClientAsync(_config["zeit_url"], "_user_", "_key_", m_req_disp));


#ifdef PUNKT_TARGETER_FULL
	m_hashd_acli.reset(new HashdClientAsync(_config["hashd_url"], m_req_disp));
	m_visitors_storage.reset(new VisitorsStorage(m_hashd_acli));	
	m_targeter.reset(new TargeterFull(_config ["replid"], m_visitors_storage, _config ["punkt_rsrc_url"],
							m_zeit_acli,
							m_files_cache,
							boost::bind(&Punktd::parseFormatterArgs, this, _1, _2)));

#endif
	
#ifdef PUNKT_TARGETER_COOKIE_ONLY
	m_targeter.reset(new TargeterCookieOnly(_config ["replid"], m_zeit_acli,
											boost::bind(&Punktd::parseFormatterArgs, this, _1, _2)));
#endif
	
	m_punkt.reset(new Punkt(m_targeter,
							_config ["systemid"],
							_config ["replid"],
							_config ["punkt_rsrc_url"]));
	
	m_srv.reset(new HttpServer(m_srv_tasklauncher,
							ResponseInfo("text/html; charset=utf-8", "punktd"),
							boost::bind(&Punktd::connHandler, this, _1, _2),
							strtoint(_config["listen_port"])));

	bindFormatters(_config["punkt_url"], _config ["punkt_rsrc_url"]);
	std::cout << "Formatters binded\n";
	loadAds();
	std::cout << "Ads loaded\n";
	loadPlaces();
	std::cout << "Places loaded\n";
	
	std::cout << "All loaded\n";

	m_pool->run();
}

void Punktd::connHandler(HttpConnectionPtr _conn, HttpRequestPtr _req) {
	
//	_conn->sendResponse("_finished_");
//	_conn->close();
//	return;
	
	checkReload();
	m_punkt->connHandler(_conn, _req);
}

void Punktd::join() {
	m_pool->join();
}
