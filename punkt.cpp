#include "punkt.h"

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
	m_ads[_ad->id] = _ad;
}

void Punkt::updatePlaceTargets(uint64_t _pid, const std::vector<uint64_t> &_targets) {
	//std::cout << "Punkt::updatePlace " << _pid << std::endl;
	hLockTicketPtr ticket = lock.lock();
	
	PlaceInfoPtr place = m_places[_pid];
	if (!place) {
		m_places[_pid].reset(new PlaceInfo);
		place = m_places[_pid];
	}

	place->ads.clear();
	for (int i = 0; i<_targets.size(); i++) {
		
		AdPtr ad = m_ads[ _targets[i] ];
		if (ad)
			place->ads.push_back(ad);
	}
}

void Punkt::updateFormatter(uint64_t _fid, FormatterPtr _formatter) {
	hLockTicketPtr ticket = lock.lock();
	m_formatters[_fid] = _formatter;
}

void Punkt::handleDemo(HttpSrv::ConnectionPtr _conn, HttpSrv::RequestPtr _req) {
	
	std::string ad_id_str;
	
	if (!_req->getField("adid", ad_id_str)) {
		_conn->sendResponse("{ \"status\" : \"wrong params\" }");
		_conn->close();
		return;
	}
	
	uint64_t adid = string_to_uint64(ad_id_str);	
	
	FormatterPtr formatter;
	FormatterArgsPtr formatter_args;
	
	{
		hLockTicketPtr ticket = lock.lock();
		hiaux::hashtable<uint64_t, AdPtr>::iterator ad_it = m_ads.find(adid);
		
		if (ad_it == m_ads.end()) {
		
			std::cout << "Punkt::handleDemo unknown ad " << adid << std::endl;
			_conn->sendResponse("{ \"status\" : \"internal error\" }");
			_conn->close();
			return;
		}
		
		hiaux::hashtable<uint64_t, FormatterPtr>::iterator f_it = m_formatters.find(ad_it->second->format_id);
		
		if (f_it == m_formatters.end()) {
		
			std::cout << "Punkt::handleDemo unknown format " << ad_it->second->format_id << std::endl;
			_conn->sendResponse("{ \"status\" : \"internal error\" }");
			_conn->close();
			return;
		}
		
		formatter = f_it->second;
		formatter_args = ad_it->second->args;
	}
	
	formatter->formatDemo(_conn, _req, 0, adid, formatter_args);
}

void Punkt::handlePlace(uint64_t _pid, HttpSrv::ConnectionPtr _conn, HttpSrv::RequestPtr _req) {
	
	FormatterPtr formatter;
	FormatterArgsPtr args;
	
	uint64_t adid;
	
	{
		hLockTicketPtr ticket = lock.lock();
	
		hiaux::hashtable<uint64_t, PlaceInfoPtr>::iterator pit = m_places.find(_pid);
		if (pit == m_places.end()) {
			_conn->sendResponse("{ \"status\" : \"unknown pid\" }");
			_conn->close();
			return;
		}
	
		AdPtr ad = pit->second->ads[ rand() % pit->second->ads.size() ];
	
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
	
	formatter->format(_conn, _req, _pid, adid, args);
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

void Punkt::handleEvent(HttpSrv::ConnectionPtr _conn, HttpSrv::RequestPtr _req, const std::string &_evtype_str) {
	
	// format event
	if (_evtype_str == "fev") {
		
		handleFormatEvent(_conn, _req);
		
	} else if (_evtype_str == "gev") {
	// general event
		
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
