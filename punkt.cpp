#include "punkt.h"

Punkt::Punkt(TargeterPtr _targeter,
	const std::string &_systemid,
	const std::string &_replid,
	const std::string &_punkt_rsrc_url):
	m_targeter(_targeter),
	m_systemid(_systemid),
	m_replid(_replid),
	m_punkt_rsrc_url(_punkt_rsrc_url) {
		
}

void Punkt::updateAd(AdPtr _ad) {
	
	hLockTicketPtr ticket = lock.lock();
		
	hiaux::hashtable<uint64_t, FormatterPtr>::iterator f_it = m_formatters.find(_ad->format_id);
	
	if (f_it == m_formatters.end()) {
		std::cout << "Punkt::updateAd cannot load Ad. Unknown format " << _ad->format_id << std::endl;
		return;
	}
	
	try {
		_ad->args = f_it->second->parseArgs(_ad->formatter_args_str);
	} catch (...) {
		std::cout << "Punkt::updateAd parseArgs exception\n";
	}

	m_targeter->updateAd(_ad);
}

void Punkt::updatePlaceTargets(uint64_t _pid, const std::vector<uint64_t> &_targets) {
	//std::cout << "Punkt::updatePlace " << _pid << std::endl;
/*	hLockTicketPtr ticket = lock.lock();
	
	PlaceTargetsPtr place = m_places[_pid];
	if (!place) {
		m_places[_pid].reset(new PlaceTargets);
		place = m_places[_pid];
	}

	place->ads.clear();
	for (int i = 0; i<_targets.size(); i++) {
		
		AdPtr ad = m_ads[ _targets[i] ];
		if (ad)
			place->ads.push_back(ad);
	}*/
	m_targeter->updatePlaceTargets(_pid, _targets);
}

void Punkt::updateFormatter(uint64_t _fid, FormatterPtr _formatter) {
	hLockTicketPtr ticket = lock.lock();
	m_formatters[_fid] = _formatter;
}

void Punkt::handleDemo(HttpSrv::ConnectionPtr _conn, HttpSrv::RequestPtr _req) {
	
	m_targeter->getVisitor(_conn, _req, boost::bind(&Punkt::handleDemoGotVisitor, this, _1, _conn, _req));
}

void Punkt::handleDemoGotVisitor(VisitorPtr _visitor, HttpSrv::ConnectionPtr _conn, HttpSrv::RequestPtr _req) {
	
	std::string ad_id_str;
	
	if (!_req->getField("adid", ad_id_str)) {
		_conn->sendResponse("{ \"status\" : \"wrong params\" }");
		_conn->close();
		return;
	}
	
	uint64_t adid = string_to_uint64(ad_id_str);	
	bool https;
	
	FormatterPtr formatter;
	FormatterArgsPtr formatter_args;
	
	{
		hLockTicketPtr ticket = lock.lock();
		
		AdPtr ad = m_targeter->getAd(adid);
		if (!ad) {
		
			std::cout << "Punkt::handleDemo unknown ad " << adid << std::endl;
			_conn->sendResponse("{ \"status\" : \"internal error\" }");
			_conn->close();
			return;
		}
		
		hiaux::hashtable<uint64_t, FormatterPtr>::iterator f_it = m_formatters.find(ad->format_id);
		
		if (f_it == m_formatters.end()) {
		
			std::cout << "Punkt::handleDemo unknown format " << ad->format_id << std::endl;
			_conn->sendResponse("{ \"status\" : \"internal error\" }");
			_conn->close();
			return;
		}
		
		formatter = f_it->second;
		formatter_args = ad->args;
	}
	
	std::string bf;
	if (_req->getField("https", bf))
		https = true;
	
	AdRequestPtr ad_req (new AdRequest(_conn, _req, 0, adid, https));
	
	if (_req->getField("search", bf)) {
		
		unescapeUrl(bf);
		toLowerUtf8(bf);
		std::cout << "Punkt::handleDemoGotVisitor query: " << bf << std::endl;
		ad_req->search_queries.push_back(bf); 
		_visitor->addQuery(bf);
	} else {
		_visitor->getQueries(ad_req->search_queries);
	}
	
	_visitor->save();
	formatter->formatDemo(ad_req, formatter_args);
}

void Punkt::handlePlace(uint64_t _pid, HttpSrv::ConnectionPtr _conn, HttpSrv::RequestPtr _req) {
	
	m_targeter->getVisitor(_conn, _req, boost::bind(&Punkt::handlePlaceGotVisitor, this, _pid, _1, _conn, _req));
}

void Punkt::handlePlaceGotVisitor(uint64_t _pid, VisitorPtr _visitor, HttpSrv::ConnectionPtr _conn, HttpSrv::RequestPtr _req) {
	
	FormatterPtr formatter;
	FormatterArgsPtr args;
	
	uint64_t adid;
	bool https = false;
	bool query_set = false;
	std::string bf;
	std::string query;
	std::vector<std::string> queries;
	
	if (_req->getField("ref", bf)) {
		
		if (RefParser::parse(bf, query))
			query_set = true;
	}
	
	{
				
		AdPtr ad = m_targeter->getAdToShow(_pid, _visitor, queries);
	
		if (!ad) {
			std::cout << "not got ad\n";
			return;
		}
		
		hLockTicketPtr ticket = lock.lock();
	
		hiaux::hashtable<uint64_t, FormatterPtr>::iterator f_it = m_formatters.find(ad->format_id);
	
		if (f_it == m_formatters.end()) {
			std::cout << "Punkt::connHandler unknown formatter " << ad->format_id << " fid: " << ad->format_id << std::endl;
			_conn->sendResponse("{ \"status\" : \"internal error\" }");
			_conn->close();
			return;
		}
	
		formatter = f_it->second;
		args = ad->args;
		adid = ad->id;
	}
	
	if (_req->getField("https", bf))
		https = true;
	
	AdRequestPtr ad_req (new AdRequest(_conn, _req, _pid, adid, https));
	
	if (query_set) {
		ad_req->search_queries.push_back(query);
		_visitor->addQuery(bf);
	}
	
	_visitor->getQueries(ad_req->search_queries);
	_visitor->save();
	formatter->format(ad_req, args);
}

void Punkt::handleFormatEvent(HttpSrv::ConnectionPtr _conn, HttpSrv::RequestPtr _req) {
	
	std::string format_id_str;
	
	if (!_req->getField("fid", format_id_str)) {
		_conn->sendResponse("{ \"status\" : \"fid not set\" }");
		_conn->close();
		return;
	}
	
	uint64_t format_id = string_to_uint64(format_id_str);
	
	FormatterPtr format;
	
	{
		hiaux::hashtable<uint64_t, FormatterPtr>::iterator it = m_formatters.find(format_id);
		if (it == m_formatters.end()) {
			_conn->sendResponse("{ \"status\" : \"unknown format\" }");
			_conn->close();
			return;
		}
		format = it->second;
	}
	format->handleFormatEvent(_conn, _req);
}

std::string Punkt::getVkAuthCode(const std::string &_domain) {
	return "\n"
	"xmlHttp = new XMLHttpRequest();\n"
	"xmlHttp.open(\"GET\", \""+ m_punkt_rsrc_url +"vkauth.html\", false);\n"
	"xmlHttp.send(null);\n"
	"var au = document.createElement('div');\n"
	"au.innerHTML = xmlHttp.responseText;\n"
	"place.appendChild(au);\n"
	"\n"
	"var head = document.getElementsByTagName('head')[0];\n"
	"var script = document.createElement('script');\n"
	"script.type = 'text/javascript';\n"
	"script.onload = function () {\n"
	"	authVk();\n"
	"}\n"
	"script.src =\"" + m_punkt_rsrc_url + "vkauth.js\";\n"
	"head.appendChild(script);\n";
}

void Punkt::handleEvent(HttpSrv::ConnectionPtr _conn, HttpSrv::RequestPtr _req, const std::string &_evtype_str) {
	
	// format event
	if (_evtype_str == "fev") {
		
		handleFormatEvent(_conn, _req);
		
	} else if (_evtype_str == "vkmatch") {
		
		
	}
}

void Punkt::connHandler(HttpSrv::ConnectionPtr _conn, HttpSrv::RequestPtr _req) {
	
	std::string demo_str;
	if (_req->getField("demo", demo_str)) {
		
		handleDemo(_conn, _req);
		return;
	}
	
	std::string evtype_str;
	if (_req->getField("evtype", evtype_str)) {
		
		handleEvent(_conn, _req, evtype_str);
		return;
	}
	
	std::string pid_str;
	if (!_req->getField("pid", pid_str)) {
		_conn->sendResponse("{ \"status\" : \"pid not set\" }");
		_conn->close();
		return;
	}
	
	handlePlace(string_to_uint64(pid_str), _conn, _req);
}

uint64_t Punkt::getAdOwner(uint64_t _adid) {
	
	return m_targeter->getAdOwner(_adid);
}
