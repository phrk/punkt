#ifndef _PUNKT_H_
#define _PUNKT_H_

#include "hiconfig.h"
#include "hiaux/network/HttpSrv.h"

#include "punkt_consts.h"
#include "formatter.h"

class Punkt {
public:
	
	class PlaceInfo {
	public:
		uint64_t formatter_id;
		std::vector<FormatterArgsPtr> formatter_args;
		
		PlaceInfo (uint64_t _formatter_id, std::vector<FormatterArgsPtr> &_formatter_args);
	};
	
	typedef boost::shared_ptr<PlaceInfo> PlaceInfoPtr;
	
	typedef boost::function<void(HttpSrv::ConnectionPtr, HttpSrv::RequestPtr, FormatterArgsPtr)> FormatterFun;

private:	
	hiaux::hashtable<uint64_t, PlaceInfoPtr> m_places;
	hiaux::hashtable<uint64_t, FormatterPtr> m_formatters;
	// place -> flights_ids []
	// hiaux::hashtable<uint64_t, std::vector<uin64_t> > m_targets;
	// flight 
	//hiaux::hashtable<uint64_t, FormatterArgsPtr> m_formatter_args;
	
	hAutoLock lock;

public:
		
	void updatePlace(uint64_t _pid, PlaceInfoPtr _placeinfo);
	void updateFormatter(uint64_t _fid, FormatterPtr _formatter);
	
	void handleDemo(HttpSrv::ConnectionPtr _conn, HttpSrv::RequestPtr _req);
	void handlePlace(uint64_t _pid, HttpSrv::ConnectionPtr _conn, HttpSrv::RequestPtr _req);
	void connHandler(HttpSrv::ConnectionPtr _conn, HttpSrv::RequestPtr _req);
	
};

typedef boost::shared_ptr<Punkt> PunktPtr;

#endif
