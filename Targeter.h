#ifndef _TARGETER_H_
#define _TARGETER_H_

#include "hiconfig.h"
#include "hiaux/network/HttpSrv.h"

#include "ad.h"
#include "PlaceTargets.h"

class Targeter {
	
	std::string m_repl_id;
	hiaux::hashtable<uint64_t, AdPtr> m_ads;
	hiaux::hashtable<uint64_t, PlaceTargetsPtr> m_places;
public:
	
	Targeter(const std::string &_repl_id);
	void getVisitorId(HttpSrv::ConnectionPtr _conn, HttpSrv::RequestPtr _req, std::string &_vid) = 0;
	
	void updateAd(AdPtr _ad);
	void updatePlaceTargets(uint64_t _pid, const std::vector<uint64_t> &_targets);

	void addSearchQuery(const std::string &_vid, const std::string &_query) = 0;
	AdPtr getAdToShow(uint64_t _pid, const std::string &_vid, std::vector<std::string> &_queries) = 0;
	
	virtual ~Targeter();
};

typedef boost::shared_ptr<Targeter> TargeterPtr;

#endif
