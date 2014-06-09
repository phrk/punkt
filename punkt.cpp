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

void Punkt::updateFormatter(uint64_t _fid, FormatterFun _formatter) {
	hLockTicketPtr ticket = lock.lock();
	m_formatters[_fid] = _formatter;
}

void Punkt::connHandler(HttpSrv::ConnectionPtr _conn, HttpSrv::RequestPtr _req) {
	
	std::string pid_str;
	if (!_req->getField("pid", pid_str)) {
		_conn->sendResponse("{ \"status\" : \"pid not set\" }");
		_conn->close();
		return;
	}
	
	uint64_t pid = string_to_uint64(pid_str);
	
	FormatterFun formatter;
	FormatterArgsPtr args;
	
	{
		hLockTicketPtr ticket = lock.lock();
	
		hiaux::hashtable<uint64_t, PlaceInfoPtr>::iterator it = m_places.find(pid);
		if (it == m_places.end()) {
			_conn->sendResponse("{ \"status\" : \"unknown pid\" }");
			_conn->close();
			return;
		}
	
		hiaux::hashtable<uint64_t, FormatterFun>::iterator f_it = m_formatters.find(it->second->formatter_id);
	
		if (f_it == m_formatters.end()) {
			std::cout << "Punkt::connHandler unknown formatter " << it->second->formatter_id << " pid: " << pid << std::endl;
			_conn->sendResponse("{ \"status\" : \"internal error\" }");
			_conn->close();
			return;
		}
	
		formatter = f_it->second;
		args = it->second->formatter_args;
	}
	
	formatter(_conn, _req, args);
}
