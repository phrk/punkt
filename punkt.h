#ifndef _PUNKT_H_
#define _PUNKT_H_

#include "hiconfig.h"
#include "hiaux/network/HttpSrv.h"

#include "punkt_consts.h"
#include "ad.h"
#include "formatter.h"

class Punkt {
public:
	
	class PlaceInfo {
	public:
		std::vector<AdPtr> ads;
	};
	
	typedef boost::shared_ptr<PlaceInfo> PlaceInfoPtr;
	typedef boost::function<void(HttpSrv::ConnectionPtr, HttpSrv::RequestPtr, FormatterArgsPtr)> FormatterFun;

private:
	
	hiaux::hashtable<uint64_t, AdPtr> m_ads;
	hiaux::hashtable<uint64_t, PlaceInfoPtr> m_places;
	hiaux::hashtable<uint64_t, FormatterPtr> m_formatters;
	
	hAutoLock lock;

	void handleFormatEvent(HttpSrv::ConnectionPtr _conn, HttpSrv::RequestPtr _req);

public:
	
	void updateAd(AdPtr _ad);
	void updatePlaceTargets(uint64_t _pid, const std::vector<uint64_t> &_targets);
	void updateFormatter(uint64_t _fid, FormatterPtr _formatter);
	
	uint64_t getAdOwner(uint64_t _adid);
	
	void handleEvent(HttpSrv::ConnectionPtr _conn, HttpSrv::RequestPtr _req, const std::string &_evtype_str);
	void handleDemo(HttpSrv::ConnectionPtr _conn, HttpSrv::RequestPtr _req);
	void handlePlace(uint64_t _pid, HttpSrv::ConnectionPtr _conn, HttpSrv::RequestPtr _req);
	void connHandler(HttpSrv::ConnectionPtr _conn, HttpSrv::RequestPtr _req);
	
};

typedef boost::shared_ptr<Punkt> PunktPtr;

#endif
