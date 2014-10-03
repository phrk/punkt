#include "TargeterFull.h"

TargeterFull::TargeterFull(const std::string &_repl_id, VisitorsStoragePtr _storage,
			const std::string &_punkt_rsrc_url,
			ZeitClientAsyncPtr _zeit_acli,
			boost::function<FormatterArgsPtr(uint64_t _format_id, const std::string &_args)> _parseFormatterArgs):
	 Targeter::Targeter(_repl_id, _parseFormatterArgs),
	 m_storage(_storage),
	 m_punkt_rsrc_url(_punkt_rsrc_url),
	 m_zeit_acli(_zeit_acli) {
		
}

void TargeterFull::genVdid(std::string &_vdid) const {
	
	char bf[128];
	sprintf(bf, "vd%llu_%llu", time(0), rand()%10000);
	_vdid = std::string(bf);
}

void TargeterFull::getVisitor(HttpConnectionPtr _conn, HttpRequestPtr _req, boost::function<void(VisitorPtr)> _onGot) {
	
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

AdTargeterArgsPtr TargeterFull::parseAdTargeterArgs(const std::string &_targeter_args_str) {
	
	return AdTargeterArgsPtr();
}

void TargeterFull::updateAd(uint64_t _id,
							uint64_t _format_id,
							uint64_t _ownerid,
							const std::string &_formatter_args_str,
							const std::string &_targeter_args_str) {
	
	hLockTicketPtr ticket = lock.lock();
	
	AdPtr ad(new Ad(_id, _format_id, _ownerid, _formatter_args_str, _targeter_args_str));
	
	ad->args = m_parseFormatterArgs(_format_id, _formatter_args_str);
	ad->targeter_args = parseAdTargeterArgs(_targeter_args_str);
	
	if (!ad->args)
		return;
	
	hiaux::hashtable<uint64_t, AdPtr>::iterator it = m_ads.find(_id);
	
	if (it == m_ads.end()) {
		
		m_ads.insert(std::pair<uint64_t, AdPtr> (_id, ad) );
	}
	else
	{
		* (it->second.get() ) = * ( ad.get() );	
	}
}

void TargeterFull::updatePlace(uint64_t _pid, const std::string &_targeter_args, const std::vector<uint64_t> &_targets) {
	
	hLockTicketPtr ticket = lock.lock();
	
	
	PlaceTargets place_targets;
	
	for (int i = 0; i<_targets.size(); i++) {
		
		AdPtr ad = m_ads[ _targets[i] ];
		if (ad)
			place_targets.ads.push_back(ad);
	}
	
	hiaux::hashtable<uint64_t, PlacePtr>::iterator it = m_places.find(_pid);
	
	PlacePtr place(new Place(_pid, _targeter_args, place_targets));
	
	if (it == m_places.end()) {
		
		m_places.insert(std::pair<uint64_t, PlacePtr> (_pid, place));
	}
	else {
		
		it->second = place;
	}
}

void TargeterFull::updateCampaign(uint64_t _cid,
							const std::string &_targeter_args,
							const std::vector<uint64_t> &_ads) {
	
	
}

AdPtr TargeterFull::getAd(uint64_t _adid) {
	
	hLockTicketPtr ticket = lock.lock();
	
	hiaux::hashtable<uint64_t, AdPtr>::iterator ad_it = m_ads.find(_adid);
	if (ad_it == m_ads.end())
		return AdPtr();
	return ad_it->second;
}

uint64_t TargeterFull::getAdOwner(uint64_t _adid) {
	
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

AdPtr TargeterFull::getAdToShow(uint64_t _pid, VisitorPtr _visitor, std::vector<std::string> &_queries, std::string &_extcode) {
	
	hLockTicketPtr ticket = lock.lock();
	
	hiaux::hashtable<uint64_t, PlacePtr>::iterator pit = m_places.find(_pid);
	if (pit == m_places.end())
		return AdPtr();

	if (pit->second->targeted_ads.ads.size()==0)
		return AdPtr();

	AdPtr ad = pit->second->targeted_ads.ads [ rand() % pit->second->targeted_ads.ads.size() ];
	return ad;
}

void TargeterFull::saveVisitor(VisitorHashd *_v) {
	
	m_storage->saveVisitor(_v);
}

ETN* TargeterFull::getCustomEventsRouter() {
	
	throw "TargeterHashd::getCustomEventsRouter";
}

TargeterFull::~TargeterFull() {
	
}
