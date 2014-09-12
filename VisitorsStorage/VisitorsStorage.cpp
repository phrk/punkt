#include "VisitorsStorage.h"

VisitorsStorage::VisitorsStorage(HashdClientAsyncPtr _hashd_acli):
	m_hashd_acli(_hashd_acli) {
	
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

void VisitorsStorage::getVisitor_onGotDeviceVid (bool _success,
												bool _exists,
												const std::string &_vid,
												const std::string &_vdid,
												boost::function<void(VisitorPtr)> _onVisitor) {
	
	std::cout << "VisitorsStorage::getVisitor_onGotDeviceVid _success: " << _success << " _exists:" << _exists << " _vid:" << _vid << "|" << std::endl;
	
	bool newdevice = false;
	if (_success && !_exists)
		newdevice =  true;
	
	if (_success && _exists) {
		std::cout << "m_hashd_acli->get visitors " << _vid << std::endl;
		m_hashd_acli->get("visitors", _vid, boost::bind(&VisitorsStorage::onGotVisitor, this, _1, _2, _3, newdevice, _vid, _vdid, _onVisitor));
		return;
	}
	
	std::string vid;
	genVid(vid);
	onGotVisitor(false, false, "", newdevice, vid, _vdid, _onVisitor);
}

void VisitorsStorage::saveVid(const std::string &_vdid, const std::string &_vid) {
	
	std::cout << "VisitorsStorage::saveVid " << _vdid << " " << _vid << std::endl;
	m_hashd_acli->set("devicevids", _vdid, _vid, boost::bind(&VisitorsStorage::onSaved, this, _1));
}

void VisitorsStorage::getVisitor(const std::string &_vdid, boost::function<void(VisitorPtr)> _onVisitor) {
	
	m_hashd_acli->get("devicevids", _vdid, boost::bind(&VisitorsStorage::getVisitor_onGotDeviceVid, this, _1, _2, _3, _vdid, _onVisitor));
}

void VisitorsStorage::onGotVisitor(bool _success,
									bool _exists,
									const std::string &_dump,
									bool _newdevice,
									const std::string &_vid,
									const std::string &_vdid,
									boost::function<void(VisitorPtr)> _onVisitor) {
	
	std::cout << "VisitorsStorage::onGotVisitor _success:" << _success << " _exists:" << _exists << std::endl; 
	
	std::string _user_agent = "_USER AGENT_";
	
	VisitorHashdPtr v(new VisitorHashd(_vid, _vdid, boost::bind(&VisitorsStorage::saveVisitor, this, _1), _newdevice));
	
	if (!_success || !_exists) {
		
		
		v->initCurDevice(_vdid, _user_agent);
		_onVisitor(v);
		return;
	}
	
	v->parseProtobuf(_dump);
	v->initCurDevice(_vdid, _user_agent);
	_onVisitor(v);
}

void VisitorsStorage::saveVisitor(VisitorHashd *_visitor) {
	
	std::string dump, vid;
	_visitor->dump(dump);
	_visitor->getId(vid);
	
	std::cout << "VisitorsStorage::saveVisitor " << vid << std::endl;	
	
	m_hashd_acli->set("visitors", vid, dump, boost::bind(&VisitorsStorage::onSaved, this, _1));
	
	if (_visitor->newdevice) {

		saveVid(_visitor->cur_vdid, _visitor->getId());
	}
	
	std::cout << "devices: " << _visitor->devices.size() << std::endl;
	
	//std::cout << "VisitorsStorage::saveVisitor NEW DEVICE=" << _visitor->newdevice << " " << _visitor->getId() << " " << _visitor->cur_vdid << " " << dump << std::endl;
}

void VisitorsStorage::onSaved(bool _success) {
	
	std::cout << "VisitorsStorage::onSaved " << _success << std::endl;
}
