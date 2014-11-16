#include "TargeterFull.h"

TargeterFull::TargeterFull(const std::string &_repl_id,
			VisitorsStoragePtr _storage,
			ZeitClientAsyncPtr _zeit_acli,
			FileCachePtr _files_cache,
			boost::function<FormatterArgsPtr(uint64_t _format_id, const std::string &_args)> _parseFormatterArgs):
	 Targeter::Targeter(_repl_id, _parseFormatterArgs),
	 m_storage(_storage),
	 m_zeit_acli(_zeit_acli),
	 m_files_cache(_files_cache) {
	
}

void TargeterFull::genVdid(std::string &_vdid) const {
	
	char bf[128];
	sprintf(bf, "vd%llu_%llu", time(0), rand()%10000);
	_vdid = std::string(bf);
}

void TargeterFull::getVisitor(HttpConnectionPtr _conn, HttpRequestPtr _req, boost::function<void(VisitorPtr)> _onGot) {
	
	std::string vdid;
	std::string dump;
	bool new_cookie = false;
	if (!_req->getCookie("punkt_vdid", vdid)) {
		
		genVdid(vdid);
		_conn->setCookie("punkt_vdid", vdid);
		new_cookie = true;
//		VisitorHashdPtr visitor(new VisitorHashd(vid, boost::bind(&TargeterHashd::saveVisitor, this, _1)));
//		_onGot(visitor);
//		return;
	}
	// else
//		new_visitor = false;
	
//	std::cout << "TargeterHashd::getVisitor " << vid << std::endl;
	m_storage->getVisitor(vdid, new_cookie, _onGot);
	
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
	else {
		* (it->second.get() ) = * ( ad.get() );	
	}
}

void TargeterFull::updatePlace(uint64_t _pid, const std::string &_targeter_args, const std::vector<uint64_t> &_targets) {
	
	hLockTicketPtr ticket = lock.lock();
	
	
	PlaceTargets place_targets;
	
	for (int i = 0; i<_targets.size(); i++) {
		
		//AdPtr ad = m_ads[ _targets[i] ];
		//if (ad)
		//	place_targets.ads.push_back(ad);
		place_targets.ads.push_back( _targets[i] );
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

void TargeterFull::getVkMatchCode(PlacePtr _place, const std::string &_punkt_url, uint64_t _adid, uint64_t _fid, std::string &_exthtml, std::string &_extjs) const {
	
	std::cout << "TargeterFull::getVkMatchCode " << _place->id << " " << _place->domain << std::endl;
	
	//hiaux::hashtable<std::string, std::string>::const_iterator it = m_vk_app_ids.find(_domain);
	
	if (!m_files_cache->getFile("vkauth.html", _exthtml)) {
		std::cout << "TargeterFull::getVkMatchCode file vkauth.html not loaded\n"; 
		return;
	}
	
	if (!m_files_cache->getFile("vkauth.js", _extjs)) {
		std::cout << "TargeterFull::getVkMatchCode file vkauth.js not loaded\n"; 
		return;
	}
	
	std::string onmatch_url =  _punkt_url
								+"?evtype=tev&fid=" + uint64_to_string(_fid)
								+"&ev=tm&m=vkmatch&pid="+uint64_to_string(_place->id)
								+"&adid="+uint64_to_string(_adid);
	
	_extjs += "\n"
		"authVk("+ _place->vk_app_id +", true, null, true, function (profile) {\n"
		"	var xmlHttp = new XMLHttpRequest();\n"
		"	xmlHttp.withCredentials = true;\n"
		"	xmlHttp.open( \"POST\", '" +onmatch_url+ "', true);\n"
		"\n"
		"	xmlHttp.onload = function (e) {\n"
		"		if (xmlHttp.readyState === 4) {\n"
		"			if (xmlHttp.status === 200) {\n"
		"			} else {\n"
		"			}\n"
		"		}\n"
		"	};\n"
		"	xmlHttp.send( JSON.stringify(profile) );\n"
		"	});\n";
}

AdPtr TargeterFull::getAdToShow(AdRequestPtr _ad_req, VisitorPtr _visitor, std::vector<std::string> &_queries, std::string &_exthtml, std::string &_extjs) {
	
	hLockTicketPtr ticket = lock.lock();
	
	VisitorHashd* visitor = (VisitorHashd*)_visitor.get();
	
	hiaux::hashtable<uint64_t, PlacePtr>::iterator pit = m_places.find(_ad_req->pid);
	if (pit == m_places.end())
		return AdPtr();

	if (pit->second->targeted_ads.ads.size() == 0)
		return AdPtr();

	AdPtr ad = m_ads [ pit->second->targeted_ads.ads [ rand() % pit->second->targeted_ads.ads.size() ] ];

	if (pit->second->vk_match &&
		(_ad_req->domain == pit->second->domain) &&
		(pit->second->vk_app_id.size() != 0) &&
		(!visitor->tried_vk_matching)) {
		
		getVkMatchCode (pit->second, ad->args->system_url, ad->id, ad->format_id, _exthtml, _extjs);
		visitor->tried_vk_matching = true;
		
		std::string vk_match_try_counter = "owners:owner_places-" + uint64_to_string(ad->ownerid)
											+ ":ads_place-"+ uint64_to_string(_ad_req->pid) +":vk_match_try";
		m_zeit_acli->mergeCounter(vk_match_try_counter, time(0), 1, boost::bind(&TargeterFull::onCalledZeit, this, _1));
	}

	if (pit->second->targeted_ads.ads.size()==0)
		return AdPtr();

	// inc ttl
	if (visitor->vk_uid != "")
		visitor->ttl_inc = fmax(visitor->ttl, 3600*24*30 - visitor->ttl); // at least month
	else
		visitor->ttl_inc = fmax(visitor->ttl, 3600*24*7 - visitor->ttl); // at least week
	////

	return ad;
}

void TargeterFull::saveVisitor(VisitorHashd *_v) {
	
	m_storage->saveVisitor(_v);
}

TargeterFull::~TargeterFull() {
	
}
