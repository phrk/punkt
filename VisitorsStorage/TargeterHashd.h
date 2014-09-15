#ifndef _TARGETER_HASHD_H_
#define _TARGETER_HASHD_H_

#include "Targeter.h"
#include "VisitorHashd.h"
#include "VisitorsStorage.h"
#include "Visitor.h"

class TargeterHashd : public Targeter {
	
	hAutoLock lock;
	hiaux::hashtable<uint64_t, AdPtr> m_ads;
	hiaux::hashtable<uint64_t, PlaceTargetsPtr> m_places;
	
	VisitorsStoragePtr m_storage;
	
	std::string m_punkt_rsrc_url;
	
	void genVdid(std::string &_vdid) const;
public:
	
	TargeterHashd(const std::string &_repl_id, VisitorsStoragePtr _storage,
			const std::string &_punkt_rsrc_url);
	
	virtual void getVisitor(HttpSrv::ConnectionPtr _conn, HttpSrv::RequestPtr _req, boost::function<void(VisitorPtr)> _onGot);
//	void onGotVisitor(const std::string &_vid, boost::function<void(VisitorPtr)> _onGot, bool _success, VisitorPtr _v);
	
	virtual void updateAd(AdPtr _ad);
	virtual void updatePlaceTargets(uint64_t _pid, const std::vector<uint64_t> &_targets);

	virtual AdPtr getAd(uint64_t _adid);
	virtual uint64_t getAdOwner(uint64_t _adid);

	virtual AdPtr getAdToShow(uint64_t _pid, VisitorPtr _visitor, std::vector<std::string> &_queries, std::string &_extcode);
	
	virtual void handleEvent(const std::string &_method, uint64_t _pid, uint64_t _adid, const std::map<std::string, std::string> &_params, HttpSrv::ConnectionPtr _conn, HttpSrv::RequestPtr _req);
	
	void saveVisitor(VisitorHashd *_v);
	
	virtual ~TargeterHashd();
};

typedef boost::shared_ptr<TargeterHashd> TargeterHashdPtr;

#endif
