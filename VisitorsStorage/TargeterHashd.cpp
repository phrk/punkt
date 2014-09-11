#include "TargeterHashd.h"

TargeterHashd::TargeterHashd(const std::string &_repl_id, VisitorsStoragePtr _storage):
	 Targeter::Targeter(_repl_id),
	 m_storage(_storage) {
	
}

void TargeterHashd::genVdid(std::string &_vdid) const {
	
	char bf[128];
	sprintf(bf, "vd%llu_%llu", time(0), rand()%10000);
	_vdid = std::string(bf);
}

void TargeterHashd::getVisitor(HttpSrv::ConnectionPtr _conn, HttpSrv::RequestPtr _req, boost::function<void(VisitorPtr)> _onGot) {
	
	std::string vdid;
	std::string dump;
//	bool new_visitor = true;
	if (!_req->getCookie("punkt_vdid", vdid)) {
		
		genVdid(vdid);
		_conn->setCookie("punkt_vdid", vdid);
		
//		VisitorHashdPtr visitor(new VisitorHashd(vid, boost::bind(&TargeterHashd::saveVisitor, this, _1)));
//		_onGot(visitor);
//		return;
	}
	// else
//		new_visitor = false;
	
//	std::cout << "TargeterHashd::getVisitor " << vid << std::endl;
	m_storage->getVisitor(vdid, _onGot);
	
	//boost::bind(&TargeterHashd::onGotVisitor, this, vid, _onGot, _1, _2));
}
/*
void TargeterHashd::onGotVisitor(const std::string &_vid, boost::function<void(VisitorPtr)> _onGot, bool _success, VisitorPtr _v) {
	
	if (_success) {
		_onGot(_v);
		return;
	}
	
//	std::cout << "TargeterHashd::onGotVisitor " << _vid << std::endl;
	
	VisitorPtr v(new VisitorHashd(_vid, boost::bind(&TargeterHashd::saveVisitor, this, _1)));
	_onGot(v);
}*/

void TargeterHashd::updateAd(AdPtr _ad) {
	
	hLockTicketPtr ticket = lock.lock();
	m_ads[_ad->id] = _ad;
}

void TargeterHashd::updatePlaceTargets(uint64_t _pid, const std::vector<uint64_t> &_targets) {
	
	hLockTicketPtr ticket = lock.lock();
	
	PlaceTargetsPtr place = m_places[_pid];
	if (!place) {
		m_places[_pid].reset(new PlaceTargets);
		place = m_places[_pid];
	}

	place->ads.clear();
	for (int i = 0; i<_targets.size(); i++) {
		
		AdPtr ad = m_ads[ _targets[i] ];
		if (ad)
			place->ads.push_back(ad);
	}
}

AdPtr TargeterHashd::getAd(uint64_t _adid) {
	
	hLockTicketPtr ticket = lock.lock();
	
	hiaux::hashtable<uint64_t, AdPtr>::iterator ad_it = m_ads.find(_adid);
	if (ad_it == m_ads.end())
		return AdPtr();
	return ad_it->second;
}

uint64_t TargeterHashd::getAdOwner(uint64_t _adid) {
	
	uint64_t ownerid;
	{
		hLockTicketPtr ticket = lock.lock();
		hiaux::hashtable<uint64_t, AdPtr>::iterator it = m_ads.find(_adid);
		if (it == m_ads.end()) {
			std::cout << "Punkt::getAdOwner unknown ad: " << _adid << std::endl;
			throw "Punkt::getAdOwner unknown ad"; 
		}
		
		ownerid = it->second->ownerid;
	}
	return ownerid;
}

AdPtr TargeterHashd::getAdToShow(uint64_t _pid, VisitorPtr _visitor, std::vector<std::string> &_queries) {
	
	hLockTicketPtr ticket = lock.lock();
	
	hiaux::hashtable<uint64_t, PlaceTargetsPtr>::iterator pit = m_places.find(_pid);
	if (pit == m_places.end())
		return AdPtr();

	if (pit->second->ads.size()==0)
		return AdPtr();

	AdPtr ad = pit->second->ads[ rand() % pit->second->ads.size() ];
	return ad;
}

void TargeterHashd::saveVisitor(VisitorHashd *_v) {
	
	m_storage->saveVisitor(_v);
}

TargeterHashd::~TargeterHashd() {
	
}
