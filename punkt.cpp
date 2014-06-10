#include "punkt.h"

Punkt::PlaceInfo::PlaceInfo (uint64_t _formatter_id, FormatterArgsPtr _formatter_args):
	formatter_id(_formatter_id),
	formatter_args(_formatter_args){
	
}

void Punkt::updatePlace(uint64_t _pid, PlaceInfoPtr _placeinfo) {
	//std::cout << "Punkt::updatePlace " << _pid << std::endl;
	hLockTicketPtr ticket = lock.lock();
	m_places[_pid] = _placeinfo;
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
	
	{
		hLockTicketPtr ticket = lock.lock();
	
		formatter_id = string_to_uint64(formatter_id_str);
	
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
		std::cout << "Punkt::handleDemo wrong args " << formatter_id << std::endl;
		_conn->sendResponse("{ \"status\" : \"wrong args\" }");
		_conn->close();
		return;
	}
	
	//std::cout << "format demo\n";
	formatter->formatDemo(_conn, _req, formatter_args);
}

void Punkt::handlePlace(uint64_t _pid, HttpSrv::ConnectionPtr _conn, HttpSrv::RequestPtr _req) {
	
	FormatterPtr formatter;
	FormatterArgsPtr args;
	
	{
		hLockTicketPtr ticket = lock.lock();
	
		hiaux::hashtable<uint64_t, PlaceInfoPtr>::iterator it = m_places.find(_pid);
		if (it == m_places.end()) {
			_conn->sendResponse("{ \"status\" : \"unknown pid\" }");
			_conn->close();
			return;
		}
	
		hiaux::hashtable<uint64_t, FormatterPtr>::iterator f_it = m_formatters.find(it->second->formatter_id);
	
		if (f_it == m_formatters.end()) {
			std::cout << "Punkt::connHandler unknown formatter " << it->second->formatter_id << " pid: " << _pid << std::endl;
			_conn->sendResponse("{ \"status\" : \"internal error\" }");
			_conn->close();
			return;
		}
	
		formatter = f_it->second;
		args = it->second->formatter_args;
	}
	
	formatter->format(_conn, _req, args);
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
