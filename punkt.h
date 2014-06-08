#ifndef _PUNKT_H_
#define _PUNKT_H_

#include "hiconfig.h"
#include "hiaux/network/HttpSrv.h"

class Punkt {
public:
	class PlaceInfo {
	public:
		uint64_t formatter_id;
		std::string formatter_args;
		
		PlaceInfo (uint64_t _formatter_id, const std::string _formatter_args);
	};
	
	typedef boost::shared_ptr<PlaceInfo> PlaceInfoPtr;
	
	typedef boost::function<void(HttpSrv::ConnectionPtr, HttpSrv::RequestPtr, const std::string &)> FormatterFun;

private:	
	hiaux::hashtable<uint64_t, PlaceInfoPtr> m_places;
	hiaux::hashtable<uint64_t, FormatterFun> m_formatters;

	hAutoLock lock;

public:
		
	void updatePlace(uint64_t _pid, PlaceInfoPtr _placeinfo);
	void updateFormatter(uint64_t _fid, FormatterFun _formatter);
	
	void connHandler(HttpSrv::ConnectionPtr _conn, HttpSrv::RequestPtr _req);
	
};

typedef boost::shared_ptr<Punkt> PunktPtr;

#endif
