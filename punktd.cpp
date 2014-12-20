#include "punktd.h"

void Punktd::onFinished() {

}

void Punktd::fallDown(std::string _s) {
	std::cout << _s << std::endl;
	exit(0);
}

void Punktd::setParamsList(std::vector<std::string> &required_params, std::vector<std::string> &_optional_params) {
	
	required_params.push_back("systemid");
	required_params.push_back("replid");
	
	required_params.push_back("listen_port");
	required_params.push_back("punkt_url");
	required_params.push_back("punkt_rsrc_url");
	
	required_params.push_back("geberd_ip");
	required_params.push_back("geberd_port");
	required_params.push_back("geberd_nconns");
	
	required_params.push_back("zeit_ip");
	required_params.push_back("zeit_port");
	required_params.push_back("zeit_nconns");
	
	required_params.push_back("hashd_url");
	
	required_params.push_back("reload_period");
	
	required_params.push_back("jsdir");
	
	required_params.push_back("pg_ip");
	required_params.push_back("pg_port");
	required_params.push_back("pg_user");
	required_params.push_back("pg_pass");
	required_params.push_back("pg_db");
}

/*
hiaux::hashtable<std::string,std::string> Punktd::parseConfig(const std::string &m_config_file) {
	hiaux::hashtable<std::string,std::string> ret;
	
	std::vector<std::string> required_params;
	std::vector<std::string> optional_params;
	
		
	try {
		ret = LoadConf::load(m_config_file, required_params, optional_params);
	}
	catch (const std::string s) {
		fallDown(s);
	} catch (const char *s) {
		fallDown(s);
	} catch (...) {
		fallDown("Geberd::parseConfig: Could not parse config file: " + m_config_file);
	}
		
	return ret;
}*/

void Punktd::bindFormatters(const std::string &_punkt_url,
							const std::string &_punkt_rsrc_url) {
	{
//		FormatterPtr f(new ShowcaseSimpleFormatter(m_req_disp, boost::bind(&HttpSrv::getHttpConnConst, m_srv.get(), _1), m_geber_cli));
//		m_punkt->updateFormatter(SHOWCASE_SIMPLE_FORMATTER_ID, f);
//		m_formatters.insert(std::pair<uint64_t, FormatterPtr>(SHOWCASE_SIMPLE_FORMATTER_ID, f));
	}
	{
		
		FormatterPtr showcase(new ShowcaseSliderFormatter(//m_req_disp,
													m_files_cache,
													m_geber_clia,
													m_zeit_acli,
													boost::bind(&Punkt::getAdOwner, m_punkt.get(), _1) ));
		
		FormatterPtr static_image (new StaticImageFormatter(m_files_cache,
															boost::bind(&Punkt::getAdOwner, m_punkt.get(), _1)));
																					
		m_punkt->updateFormatter(SHOWCASE_SLIDER_FORMATTER_ID, showcase);
		m_punkt->updateFormatter(SHOWCASE_SLIDER_304_224_FORMATTER_ID, showcase);
		m_punkt->updateFormatter(SHOWCASE_SLIDER_520_250_4_ITEMS_FORMATTER_ID, showcase);
		m_punkt->updateFormatter(STATIC_IMAGE_FORMATTER_ID, static_image);
		
		m_formatters.insert(std::pair<uint64_t, FormatterPtr>(SHOWCASE_SLIDER_FORMATTER_ID, showcase));
		m_formatters.insert(std::pair<uint64_t, FormatterPtr>(SHOWCASE_SLIDER_304_224_FORMATTER_ID, showcase));
		m_formatters.insert(std::pair<uint64_t, FormatterPtr>(SHOWCASE_SLIDER_520_250_4_ITEMS_FORMATTER_ID, showcase));
		m_formatters.insert(std::pair<uint64_t, FormatterPtr>(STATIC_IMAGE_FORMATTER_ID, static_image));
	}
}


/*
void Punktd::getClickTemplates(const hiaux::hashtable<std::string,std::string> &m_config) {
	
	hiaux::hashtable<std::string, std::string>::const_iterator it = m_config.find("wikimart_click_template");
	if (it != m_config.end())
		_click_templates[WIKIMART_ADVID] = it->second;
	
	it = m_config.find("ozon_click_template");
	if (it != m_config.end())
		_click_templates[OZON_ADVID] = it->second;
}*/

FormatterArgsPtr Punktd::parseFormatterArgs(uint64_t _format_id, const std::string &_args) {
		
	std::cout << "Punktd::parseFormatterArgs\n";
	
	FormatterArgsPtr ret;
	
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

Punktd::Punktd() {
	
	setDefaultSignalHandlers();
	//loadConfig(m_config_file);
}

Punktd::~Punktd() {
	
}

void Punktd::doStart() {
	
	m_last_reload_ts = 0;
	
	//hiaux::hashtable<std::string,std::string> m_config = parseConfig(m_config_file);
	
	std::cout << "Config loaded\n";
	
	m_reload_period = string_to_uint64(m_config["reload_period"]);
	
	m_pg  = PQsetdbLogin(m_config["pg_ip"].c_str(),
                      m_config ["pg_port"].c_str(),
                     "",
                     "",
                     m_config ["pg_user"].c_str(),
                     m_config ["pg_pass"].c_str(),
                     m_config ["pg_db"].c_str());
					 
	if (PQstatus(m_pg) != CONNECTION_OK) {
		std::cout << "Could not connect to db";
		exit(0);
	}
	
	std::cout << "Connected to db\n";
	
	std::string jsdir = m_config["jsdir"];
	
	m_files_cache.reset(new FileCache);
	m_files_cache->addFile("renderShowcaseSlider.js", jsdir + "renderShowcaseSlider.js");
	m_files_cache->addFile("mootools", jsdir + "mootools-core-1.5.0.js");
	m_files_cache->addFile("slider.js", jsdir + "buildSlider.js");
	m_files_cache->addFile("ShowcaseSliderEvents.js", jsdir + "ShowcaseSliderEvents.js");
	m_files_cache->addFile("static_image.js", jsdir + "static_image.js");
	
	/*
	m_files_cache->addFile("vkauth.html", "TargeterFull/vkauth.html");
	m_files_cache->apply("vkauth.html", boost::bind(&escape_symbol, _1, '\"'));
	m_files_cache->apply("vkauth.html", boost::bind(&add_newline_backslash, _1));
	m_files_cache->addFile("vkauth.js", "TargeterFull/vkauth.js");
	*/
	
	m_pool.reset(new hThreadPool(PUNKTD_NTHREADS));
	m_srv_tasklauncher.reset(new TaskLauncher(m_pool, PUNKTD_NTHREADS, boost::bind(&Punktd::onFinished, this)));
	
	//m_req_disp.reset(new HttpOutRequestDisp(m_srv_tasklauncher));

	//m_geber_acli.reset(new GeberdCliApiClientAsync(m_config["geberd_url"], m_req_disp));
	
	try {
	
	m_hiapi_bin_clienta_zeit.reset (new hiapi::client::BinClientA(hiapi::client::BinClientA::INTERNET,
							m_config["zeit_ip"], strtoint(m_config["zeit_port"]), strtoint(m_config["zeit_nconns"])));
	
	m_zeit_acli.reset(new ZeitClientA( m_hiapi_bin_clienta_zeit ));

	} catch (...) {
		std::cout << "zeit connect exception\n";
	}

	m_hiapi_bin_clienta_geber.reset (new hiapi::client::BinClientA(hiapi::client::BinClientA::INTERNET,
							m_config["geberd_ip"], strtoint(m_config["geberd_port"]), strtoint(m_config["geberd_nconns"])));
	
	m_geber_clia.reset(new GeberCliApiClientA( m_hiapi_bin_clienta_geber ));

	m_srv_tasklauncher->addTask(NEW_LAUNCHER_TASK2(&Punktd::clientLoop, this));

#ifdef PUNKT_TARGETER_FULL
	m_hashd_acli.reset(new HashdClientAsync(m_config["hashd_url"], m_req_disp));
	m_visitors_storage.reset(new VisitorsStorage(m_hashd_acli));	
	m_targeter.reset(new TargeterFull(m_config ["replid"],
							m_visitors_storage,
							m_zeit_acli,
							m_files_cache,
							boost::bind(&Punktd::parseFormatterArgs, this, _1, _2)));

#endif
	
#ifdef PUNKT_TARGETER_COOKIE_ONLY
	m_targeter.reset(new TargeterCookieOnly(m_config ["replid"], m_zeit_acli,
											boost::bind(&Punktd::parseFormatterArgs, this, _1, _2)));
#endif
	
	m_punkt.reset(new Punkt(m_targeter,
							m_config ["systemid"],
							m_config ["replid"],
							m_config ["punkt_rsrc_url"]));
	

	bindFormatters(m_config["punkt_url"], m_config ["punkt_rsrc_url"]);
	std::cout << "Formatters binded\n";
	loadAds();
	std::cout << "Ads loaded\n";
	loadPlaces();
	std::cout << "Places loaded\n";
	
	std::cout << "All loaded\n";

	m_srv.reset(new HttpServer(m_srv_tasklauncher,
							ResponseInfo("text/html; charset=utf-8", "punktd"),
							boost::bind(&Punktd::connHandler, this, _1, _2),
							strtoint(m_config["listen_port"])));

	m_pool->run();
	m_pool->join();
}


TaskLauncher::TaskRet Punktd::clientLoop() {
	
	while (true) {
		
		m_hiapi_bin_clienta_geber->handleEvents();
	}
}

void Punktd::connHandler(HttpConnectionPtr _conn, HttpRequestPtr _req) {
	
//	_conn->sendResponse("_finished_");
//	_conn->close();
//	return;
	
	checkReload();
	m_punkt->connHandler(_conn, _req);
}

