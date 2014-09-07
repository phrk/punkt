#ifndef _TARGETER_HASHD_H_
#define _TARGETER_HASHD_H_

#include "Targeter.h"
#include "VisitorHashd.h"
#include "VisitorsStorage.h"

class TargeterHashd : public Targeter {
	
	hAutoLock lock;
	hiaux::hashtable<uint64_t, AdPtr> m_ads;
	hiaux::hashtable<uint64_t, PlaceTargetsPtr> m_places;
	
	VisitorsStoragePtr m_storage;
	
	void genVid(std::string &_vid) const;
public:
	
	TargeterHashd(const std::string &_repl_id, VisitorsStoragePtr _storage);
	
	virtual void getVisitor(HttpSrv::ConnectionPtr _conn, HttpSrv::RequestPtr _req, boost::function<void(VisitorPtr)> _onGot);
	
	virtual void updateAd(AdPtr _ad);
	virtual void updatePlaceTargets(uint64_t _pid, const std::vector<uint64_t> &_targets);

	virtual AdPtr getAd(uint64_t _adid);
	virtual uint64_t getAdOwner(uint64_t _adid);

	virtual AdPtr getAdToShow(uint64_t _pid, VisitorPtr _visitor, std::vector<std::string> &_queries);
	
	void saveVisitor(VisitorHashd *_v);
	
	virtual ~TargeterHashd();
};

typedef boost::shared_ptr<TargeterHashd> TargeterHashdPtr;

#endif
