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
	
	FormatterPtr f(new ShowcaseSimpleFormatter(m_req_disp, boost::bind(&HttpSrv::getHttpConnConst, m_srv.get(), _1), m_geber_cli));
	//(Punkt::FormatterFun)boost::bind(&Formatter::format, f.get(), _1, _2, _3)
	m_punkt->updateFormatter(SHOWCASE_SIMPLE_FORMATTER_ID, f);
	m_formatters.insert(std::pair<uint64_t, FormatterPtr>(SHOWCASE_SIMPLE_FORMATTER_ID, f));
}

void Punktd::checkReload() {
	uint64_t now = time(0);
	if (now - m_last_reload_ts < m_reload_period)
		return;

	loadPlaces();
	
	m_last_reload_ts = time(0);
}

void Punktd::loadPlaces() {
	//std::cout << "Punktd::loadPlaces\n";
	// INSERT INTO punkt.places (formatter_id, formatter_args, ownerid, caption) VALUES (0, '{ "shid" : "2", "n" : "6"}', 0, '');
	std::string query  ("SELECT * FROM punkt.places");
	PGresult *res = PQexec(m_pg, query.c_str());
	
	if (PQresultStatus(res) != PGRES_TUPLES_OK) {
		std::cout << "Punktd::loadPlaces !PGRES_TUPLES_OK\n";
		return;
	}
	
	int ntuples = PQntuples(res);
	int ncols = PQnfields(res);
	
	uint64_t id;
	uint64_t formatter_id;
	std::string formatter_args;
	uint64_t ownerid;
	std::string caption;
	
	for (int t = 0; t<ntuples; t++) {	
		for (int c = 0; c < ncols; c++) {
			if (std::string("id") == PQfname(res, c))
				id = string_to_uint64(PQgetvalue(res, t, c));
			
			if (std::string("formatter_id") == PQfname(res, c))
				formatter_id = string_to_uint64(PQgetvalue(res, t, c));
			
			if (std::string("formatter_args") == PQfname(res, c))
				formatter_args = PQgetvalue(res, t, c);
			
			if (std::string("ownerid") == PQfname(res, c))
				ownerid = string_to_uint64(PQgetvalue(res, t, c));
			
			if (std::string("caption") == PQfname(res, c))
				caption = PQgetvalue(res, t, c);
		}
		
		hiaux::hashtable<uint64_t, FormatterPtr>::iterator it = m_formatters.find(formatter_id);
		if (it == m_formatters.end()) {
			std::cout << "Punktd::loadPlaces formatter " << formatter_id << " dont exists. Place " << id << std::endl;
			continue;
		}
		
		FormatterArgsPtr args;
		try {
			args = it->second->parseArgs(id, formatter_args);
		} catch (...) {
			std::cout << "Punktd::loadPlaces could not parse args for place " << id << std::endl;
			continue;
		}
		m_punkt->updatePlace(id, Punkt::PlaceInfoPtr(new Punkt::PlaceInfo(formatter_id, args)));
	}
	
		// pid shid nres
	 // (new ShowcaseSimpleFormatterArgs (0, 2, 6) );
	
	/*
	{
		FormatterArgsPtr args (new ShowcaseSimpleFormatterArgs (1, 2, 6) );
		m_punkt->updatePlace(1, Punkt::PlaceInfoPtr(new Punkt::PlaceInfo(SHOWCASE_SIMPLE_FORMATTER_ID, args)));
	}*/
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
	
	m_geber_cli.reset(new GeberdCliApiClient(_config["geberd_url"]));
	
	m_punkt.reset(new Punkt());
	
	m_pool.reset(new hThreadPool(PUNKTD_NTHREADS));
	m_srv_tasklauncher.reset(new TaskLauncher(m_pool, PUNKTD_NTHREADS, boost::bind(&onFinished)));
	
	m_req_disp.reset(new HttpOutRequestDisp(m_srv_tasklauncher));
	
	m_srv.reset(new HttpSrv(m_srv_tasklauncher,
							HttpSrv::ResponseInfo("text/plain; charset=utf-8", "punktd"),
							boost::bind(&Punktd::connHandler, this, _1, _2)));

	//m_punkt.reset(new Punkt(boost::bind(&HttpSrv::getHttpConnConst, m_srv.get(), _1)));
	
	bindFormatters();
	loadPlaces();
	
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
