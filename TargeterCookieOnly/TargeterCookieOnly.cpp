#include "TargeterCookieOnly.h"

TargeterCookieOnly::TargeterCookieOnly(const std::string &_repl_id, ZeitClientAsyncPtr _zeit_acli):
	 Targeter::Targeter(_repl_id),
	 m_zeit_acli(_zeit_acli) {
	
}

void TargeterCookieOnly::genVid(std::string &_vid) const {
	
	char bf[128];
	sprintf(bf, "v%llu_%llu", time(0), rand()%10000);
	_vid = std::string(bf);
}

void TargeterCookieOnly::getVisitor(HttpConnectionPtr _conn, HttpRequestPtr _req, boost::function<void(VisitorPtr)> _onGot) {
	
	std::string vid;
	std::string dump;
	bool new_visitor = true;
	if (!_req->getCookie("punkt_vid", vid)) {
		
		genVid(vid);
		_conn->setCookie("punkt_vid", vid);
	}
	
	if (_req->getCookie("punkt_vdump", dump)) {
		
		new_visitor = false;
	}
	
	VisitorCookieOnlyPtr visitor(new VisitorCookieOnly(vid, boost::bind(&TargeterCookieOnly::saveVisitor, this, _1), _conn));
	
	if (!new_visitor)
		visitor->restore(dump);
	
	_onGot(visitor);
}

void TargeterCookieOnly::updateAd(AdPtr _ad) {
	
	hLockTicketPtr ticket = lock.lock();
	m_ads[_ad->id] = _ad;
}

void TargeterCookieOnly::updatePlaceTargets(uint64_t _pid, const std::vector<uint64_t> &_targets) {
	
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

AdPtr TargeterCookieOnly::getAd(uint64_t _adid) {
	
	hLockTicketPtr ticket = lock.lock();
	
	hiaux::hashtable<uint64_t, AdPtr>::iterator ad_it = m_ads.find(_adid);
	if (ad_it == m_ads.end())
		return AdPtr();
	return ad_it->second;
}

uint64_t TargeterCookieOnly::getAdOwner(uint64_t _adid) {
	
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

AdPtr TargeterCookieOnly::getAdToShow(uint64_t _pid, VisitorPtr _visitor, std::vector<std::string> &_queries, std::string &_extcode) {
	
	hLockTicketPtr ticket = lock.lock();
	
	hiaux::hashtable<uint64_t, PlaceTargetsPtr>::iterator pit = m_places.find(_pid);
	if (pit == m_places.end())
		return AdPtr();

	if (pit->second->ads.size()==0)		
		return AdPtr();

	AdPtr ad = pit->second->ads[ rand() % pit->second->ads.size() ];
	return ad;
}


void TargeterCookieOnly::saveVisitor(VisitorCookieOnly *_v) {
	
	std::string vis_dump;
	_v->dump(vis_dump);
	_v->m_conn->setCookie("punkt_vdump", vis_dump);
}

TargeterCookieOnly::~TargeterCookieOnly() {
	
}
