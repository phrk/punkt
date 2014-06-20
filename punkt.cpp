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
	
	std::string formatter_id_str;
	std::string formatter_args_str;
	
	if (!_req->getField("fid", formatter_id_str)) {
		_conn->sendResponse("{ \"status\" : \"wrong params\" }");
		_conn->close();
		return;
	}
	if (!_req->getField("fargs", formatter_args_str)) {
		_conn->sendResponse("{ \"status\" : \"wrong params\" }");
		_conn->close();
		return;
	}
	
	uint64_t formatter_id;
	FormatterPtr formatter;
	/*
	{
		hLockTicketPtr ticket = lock.lock();
	
		formatter_id = string_to_uint64(formatter_id_str);
		/
		hiaux::hashtable<uint64_t, FormatterPtr>::iterator f_it = m_formatters.find(formatter_id);
	
		if (f_it == m_formatters.end()) {
		
			std::cout << "Punkt::handleDemo unknown formatter " << formatter_id << std::endl;
			_conn->sendResponse("{ \"status\" : \"internal error\" }");
			_conn->close();
			return;
		}
		
		formatter = f_it->second;
	}
	
	FormatterArgsPtr formatter_args;
	
	try {
		formatter_args = formatter->parseArgs(EMPTY_PLACE, formatter_args_str);
	}
	catch (...) {
		std::cout << "Punkt::handleDemo wrong args " << formatter_id << "args: " << formatter_args_str << std::endl;
		_conn->sendResponse("{ \"status\" : \"wrong args\" }");
		_conn->close();
		return;
	}
	
	//std::cout << "format demo\n";
	formatter->formatDemo(_conn, _req, formatter_args);
	*/
}

void Punkt::handlePlace(uint64_t _pid, HttpSrv::ConnectionPtr _conn, HttpSrv::RequestPtr _req) {
	
	FormatterPtr formatter;
	FormatterArgsPtr args;
	
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
	}
	
	formatter->format(_conn, _req, _pid, args);
}

void Punkt::connHandler(HttpSrv::ConnectionPtr _conn, HttpSrv::RequestPtr _req) {
	
	std::string demo_str;
	if (_req->getField("demo", demo_str)) {
		
		handleDemo(_conn, _req);
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
