#include "punkt.h"

Punkt::Punkt(TargeterPtr _targeter,
	const std::string &_systemid,
	const std::string &_replid,
	const std::string &_punkt_rsrc_url):
	m_targeter(_targeter),
	m_systemid(_systemid),
	m_replid(_replid),
	m_punkt_rsrc_url(_punkt_rsrc_url) {
	
	ETN *targeter_custom_methods = m_targeter->getCustomMethodsRouter();
	
		/*
											"cus",
											new ETN ("pid",
													new ETN ("adid",
																new ETN(boost::bind(&Punkt::handleTargeterEvent, this, "tm", _1, _2, _3))))
		*/
	
	ETN *evtype = 
			new ETN(EVENT_EQUALS,
					"fev",
						new ETN ("fid",
								new ETN (boost::bind(&Punkt::handleFormatEvent, this, _1, _2, _3))),
					"tev",
						new ETN ("fid",
							new ETN ("ev",
								new ETN (EVENT_EQUALS,
											"disp",
										 	new ETN ("pid",
													new ETN ("adid",
																new ETN (boost::bind(&Punkt::handleTargeterEvent, this, "disp", _1, _2, _3)))),
											"click",
											new ETN ("pid",
													new ETN ("adid",
																new ETN(boost::bind(&Punkt::handleTargeterEvent, this, "click", _1, _2, _3)))),
											"tm",						
											targeter_custom_methods	))));

	ETN *demo = new ETN(boost::bind(&Punkt::handleDemo, this, _1, _2, _3));

	ETN *place = new ETN("pid",
						new ETN("loc",
							new ETN("d",
								new ETN(boost::bind(&Punkt::handlePlace, this, _1, _2, _3)))));

	m_event_router = new ETN ("demo", demo,
								"evtype", evtype,
								"pid", place);
}

void Punkt::updateFormatter(uint64_t _fid, FormatterPtr _formatter) {
	
	hLockTicketPtr ticket = lock.lock();
	m_formatters[_fid] = _formatter;
}

void Punkt::handleDemo(const std::map<std::string, std::string> &_params, HttpConnectionPtr _conn, HttpRequestPtr _req) {
	
	m_targeter->getVisitor(_conn, _req, boost::bind(&Punkt::handleDemoGotVisitor, this, _1, _conn, _req));
}

void Punkt::handleDemoGotVisitor(VisitorPtr _visitor, HttpConnectionPtr _conn, HttpRequestPtr _req) {
	
	std::string ad_id_str;
	std::string location;
	std::string domain;
	
	_req->getField("loc", location);
	_req->getField("d", domain);
	
	if (!_req->getField("adid", ad_id_str)) {

		_conn->sendResponse(HttpResponse(200, "{ \"status\" : \"wrong params\" }"));
		return;
	}
	
	uint64_t adid = string_to_uint64(ad_id_str);	
	bool https;
	
	FormatterPtr formatter;
	FormatterArgsPtr formatter_args;
	
	std::string bf;
	if (_req->getField("https", bf))
		https = true;
	
	AdRequestPtr ad_req (new AdRequest(_conn, _req, location, domain, 0, https));
	
	{
		hLockTicketPtr ticket = lock.lock();
		
		AdPtr ad = m_targeter->getAd(adid);
		if (!ad) {
		
			std::cout << "Punkt::handleDemo unknown ad " << adid << std::endl;
			_conn->sendResponse(HttpResponse(200, "{ \"status\" : \"internal error\" }"));
			return;
		}
		
		hiaux::hashtable<uint64_t, FormatterPtr>::iterator f_it = m_formatters.find(ad->format_id);
		
		if (f_it == m_formatters.end()) {
		
			std::cout << "Punkt::handleDemo unknown format " << ad->format_id << std::endl;
			_conn->sendResponse(HttpResponse(200, "{ \"status\" : \"internal error\" }"));
			return;
		}
		
		formatter = f_it->second;
		formatter_args = ad->args;
	}

	ad_req->adid = adid;
	
	if (_req->getField("search", bf)) {
		
		unescapeUrl(bf);
		toLowerUtf8(bf);
		//std::cout << "Punkt::handleDemoGotVisitor query: " << bf << std::endl;
		ad_req->search_queries.push_back(bf); 
		_visitor->addQuery(bf);
	} else {
		_visitor->getQueries(ad_req->search_queries);
	}
	
	_visitor->save();
	formatter->formatDemo(ad_req, formatter_args);
}

void Punkt::handlePlace(const std::map<std::string, std::string> &_params, HttpConnectionPtr _conn, HttpRequestPtr _req) {
	
	uint64_t _pid;
	std::string pid_str;
	if (!_req->getField("pid", pid_str)) {
		
	}
	
	m_targeter->getVisitor(_conn, _req, boost::bind(&Punkt::handlePlaceGotVisitor, this, string_to_uint64(pid_str), _1, _conn, _req));
}

void Punkt::handlePlaceGotVisitor(uint64_t _pid, VisitorPtr _visitor, HttpConnectionPtr _conn, HttpRequestPtr _req) {
	
	FormatterPtr formatter;
	FormatterArgsPtr args;
	
	uint64_t adid;
	bool https = false;
	bool query_set = false;
	std::string bf;
	std::string query;
	std::vector<std::string> queries;
	std::string exthtml, extjs;
	
	std::string location;
	std::string domain;
	
	_req->getField("loc", location);
	_req->getField("d", domain);
	
	if (_req->getField("ref", bf)) {
		
		if (RefParser::parse(bf, query))
			query_set = true;
	}
	
	if (_req->getField("https", bf))
		https = true;
	
	AdRequestPtr ad_req (new AdRequest(_conn, _req, location, domain, _pid, https));
	
	{
				
		AdPtr ad = m_targeter->getAdToShow(ad_req, _visitor, queries, exthtml, extjs);
	
		if (!ad) {
			std::cout << "not got ad\n";
			return;
		}
		
		//std::cout << "got ad\n";
		
		hLockTicketPtr ticket = lock.lock();
	
		hiaux::hashtable<uint64_t, FormatterPtr>::iterator f_it = m_formatters.find(ad->format_id);
	
		if (f_it == m_formatters.end()) {

			std::cout << "Punkt::connHandler unknown formatter " << ad->format_id << " fid: " << ad->format_id << std::endl;
			_conn->sendResponse(HttpResponse(200, "{ \"status\" : \"internal error\" }"));
			return;
		}
	
		formatter = f_it->second;
		args = ad->args;
		adid = ad->id;
	}
	
	ad_req->adid = adid;
	
	if (query_set) {
		ad_req->search_queries.push_back(query);
		_visitor->addQuery(bf);
	}

	_visitor->getQueries(ad_req->search_queries);
	_visitor->save();
	formatter->format(ad_req, args, exthtml, extjs);
}

void Punkt::handleFormatEvent(const std::map<std::string, std::string> &_params, HttpConnectionPtr _conn, HttpRequestPtr _req) {
		
	uint64_t format_id = string_to_uint64(_params.at("fid"));
	
	FormatterPtr format;
	
	{
		hiaux::hashtable<uint64_t, FormatterPtr>::iterator it = m_formatters.find(format_id);
		if (it == m_formatters.end()) {
			_conn->sendResponse(HttpResponse(200, "{ \"status\" : \"unknown format\" }"));
			return;
		}
		format = it->second;
	}
	format->handleFormatEvent(_conn, _req);
}

void Punkt::handleTargeterEvent(const std::string &_method,
								const std::map<std::string, std::string> &_params,
								HttpConnectionPtr _conn, HttpRequestPtr _req) {

	uint64_t pid = string_to_uint64(_params.at("pid"));
	uint64_t adid = string_to_uint64(_params.at("adid"));
	
	m_targeter->handleEvent(_method, pid, adid, _params, _conn, _req);
}

void Punkt::connHandler(HttpConnectionPtr _conn, HttpRequestPtr _req) {
	
	m_event_router->handleEvent(_conn, _req);
}

uint64_t Punkt::getAdOwner(uint64_t _adid) {
	
	return m_targeter->getAdOwner(_adid);
}
