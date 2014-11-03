#include "VisitorsStorage.h"

VisitorsStorage::VisitorsStorage(HashdClientAsyncPtr _hashd_acli):
	m_hashd_acli(_hashd_acli) {
	
	m_default_ttl = 43200; // 12 hours
	
	std::map<std::string, std::string> opts;
	opts["type"] = "ttl";
	opts["default_ttl"] = uint64_to_string(m_default_ttl);
	opts["cleanup_period"] = "25";
	opts["cleanup_check_size"] = "2000";
	
	m_hashd_acli->createHash("visitors", opts, boost::bind(&VisitorsStorage::onSaved, this, _1));
	
	opts.clear();
	opts["type"] = "ttl";
	opts["default_ttl"] = uint64_to_string(m_default_ttl);
	opts["cleanup_period"] = "25";
	opts["cleanup_check_size"] = "2000";
	
	m_hashd_acli->createHash("vk_profiles", opts, boost::bind(&VisitorsStorage::onSaved, this, _1));
	
	opts.clear();
	opts["type"] = "ttl";
	opts["default_ttl"] = uint64_to_string(m_default_ttl);
	opts["cleanup_period"] = "25";
	opts["cleanup_check_size"] = "2000";
	m_hashd_acli->createHash("devicevids", opts, boost::bind(&VisitorsStorage::onSaved, this, _1));
}

/*
void VisitorsStorage::mergeVisitors(uint64_t _vid0, uint64_t _vid1) {
	
}
*/

void VisitorsStorage::genVid(std::string &_vid) {
	
	char bf[128];
	sprintf(bf, "v%llu_%llu", time(0), rand()%10000);
	_vid = std::string(bf);
}

std::string VisitorsStorage::genVid() {
	
	std::string vid;
	genVid(vid);
	return vid;
}

void VisitorsStorage::getVisitor(const std::string &_vdid, bool _new_cookie, boost::function<void(VisitorPtr)> _onVisitor) {
	
	if (_new_cookie) {
		
		VisitorHashdPtr v(new VisitorHashd(genVid(),
											_vdid,
											m_default_ttl,
											boost::bind(&VisitorsStorage::saveVisitor, this, _1),
											true)); // saving
		v->initCurDevice(_vdid, "_user_agent_");
		_onVisitor(v);
		
	} else
		m_hashd_acli->get("devicevids", _vdid, boost::bind(&VisitorsStorage::getVisitor_onGotDeviceVid, this, _1, _2, _vdid, _onVisitor));
}

void VisitorsStorage::getVisitor_onGotDeviceVid (int _err,
												const std::string &_vid,
												const std::string &_vdid,
												boost::function<void(VisitorPtr)> _onVisitor) {
	
	
	if (_err == E_HC_OK) { //(_success && vid _exists)

		//std::cout << "VisitorsStorage::getVisitor_onGotDeviceVid E_HC_OK\n";
		m_hashd_acli->getWithTtl("visitors", _vid, boost::bind(&VisitorsStorage::onGotVisitor, this, _1, _2, _3, _vid, _vdid, _onVisitor));
		return;
	}
	
	if (_err == E_HC_KEY_DONT_EXISTS) {
		
		//std::cout << "VisitorsStorage::getVisitor_onGotDeviceVid E_HC_KEY_DONT_EXISTS\n";
		VisitorHashdPtr v(new VisitorHashd(genVid(),
											_vdid,
											m_default_ttl,
											boost::bind(&VisitorsStorage::saveVisitor, this, _1),
											true)); // saving
		v->initCurDevice(_vdid, "_user_agent_");
		_onVisitor(v);
		return;
	}
	
	if (_err == E_HC_HASH_DONT_EXISTS) {
		
		std::cout << "VisitorsStorage::getVisitor_onGotDeviceVid E_HC_HASH_DONT_EXISTS\n"; 
	}
	
	// E_HC_CONNECTIVITY_ERROR
	//std::cout << "VisitorsStorage::getVisitor_onGotDeviceVid E_HC_CONNECTIVITY_ERROR\n";
	
	//std::cout << "err: " << _err << std::endl;
	
	VisitorHashdPtr v(new VisitorHashd(genVid(),
										_vdid,
										m_default_ttl,
										boost::bind(&VisitorsStorage::saveVisitor, this, _1),
										false)); // not saving
	v->initCurDevice(_vdid, "_user_agent_");
	_onVisitor(v);
}

void VisitorsStorage::saveVid(const std::string &_vdid, const std::string &_vid, uint64_t _ttl_inc) {
	
//	std::cout << "VisitorsStorage::saveVid " << _vdid << " " << _vid << std::endl;
	m_hashd_acli->setAndIncTtl("devicevids", _vdid, _vid, _ttl_inc, boost::bind(&VisitorsStorage::onSaved, this, _1));
}



void VisitorsStorage::onGotVisitor(int _err,
									const std::string &_dump,
									uint64_t _ttl,
									const std::string &_vid,
									const std::string &_vdid,
									boost::function<void(VisitorPtr)> _onVisitor) {
	
	if (_err == E_HC_OK) {
		
	//	std::cout << "VisitorsStorage::onGotVisitor ttl:" << _ttl << std::endl;
	//	std::cout << "dump size: " << _dump.size() << std::endl;
		
		VisitorHashdPtr v(new VisitorHashd(_vid,
											_vdid,
											_ttl,
											boost::bind(&VisitorsStorage::saveVisitor, this, _1),
											true)); // saving
		v->parseProtobuf(_dump);
		v->initCurDevice(_vdid, "_user_agent_");
		//v->ttl_inc += fmax(_ttl, 3600*24*7 - _ttl ); // repeated visit. set ttl at least one week
		_onVisitor(v);
		return;
	}
	
	if (_err == E_HC_KEY_DONT_EXISTS) {
		
		VisitorHashdPtr v(new VisitorHashd(_vid,
											_vdid,
											m_default_ttl,
											boost::bind(&VisitorsStorage::saveVisitor, this, _1),
											true)); // saving
		v->initCurDevice(_vdid, "_user_agent_");
		_onVisitor(v);
		return;
	}
	
	// E_HC_CONNECTIVITY_ERROR | E_HC_HASH_DONT_EXISTS
	
	VisitorHashdPtr v(new VisitorHashd(_vid,
										_vdid,
										m_default_ttl,
										boost::bind(&VisitorsStorage::saveVisitor, this, _1),
										false)); // saving
	v->initCurDevice(_vdid, "_user_agent_");
	_onVisitor(v);						
}

void VisitorsStorage::saveVisitor(VisitorHashd *_visitor) {
	
	if (!_visitor->m_saving)
		return;
	
	std::string dump, vid;
	_visitor->dump(dump);
	_visitor->getId(vid);
	
	//std::cout << "VisitorsStorage::saveVisitor " << vid << std::endl;	
	
	m_hashd_acli->setAndIncTtl("visitors", vid, dump, _visitor->ttl_inc, boost::bind(&VisitorsStorage::onSaved, this, _1));
	
	for (int i = 0; i<_visitor->devices.size(); i++) {
		
		saveVid(_visitor->devices[i].vdid, vid, _visitor->ttl_inc);
	}
	
	//	std::cout << "devices: " << _visitor->devices.size() << std::endl;
	
	//std::cout << "VisitorsStorage::saveVisitor NEW DEVICE=" << _visitor->newdevice << " " << _visitor->getId() << " " << _visitor->cur_vdid << " " << dump << std::endl;
}

void VisitorsStorage::onSaved(int _err) {
	
//	std::cout << "VisitorsStorage::onSaved " << _success << std::endl;
}
