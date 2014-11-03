#include "VisitorHashd.h"

VisitorHashd::VisitorHashd(const std::string &_vid,
						const std::string &_vdid,
						uint64_t _ttl,
						boost::function<void(VisitorHashd*)> _onSave,
						bool _saving):
	cur_vdid(_vdid),
	m_onSave(_onSave),
	Visitor(_vid),
	m_saving(_saving),
	ttl(_ttl),
	ttl_inc(0),
	newdevice(false),
	tried_vk_matching(false) {
	
}

void VisitorHashd::initCurDevice(const std::string &_vdid, const std::string &_user_agent) {
	
//	std::cout << "VisitorHashd::initCurDevice devices: " << devices.size() << std::endl;
	
	for (size_t i = 0; i<devices.size(); i++)
		if (devices[i].vdid == _vdid) {
			cur_device = &devices[i];
//			std::cout << "device already exists " << _vdid << std::endl;
			return;
		}
	
//	std::cout << "adding new device " << _vdid << std::endl;
	newdevice = true;
	devices.push_back(VisitDevice(_vdid, _user_agent, time(0)));
}

std::string& VisitorHashd::getId(std::string &_vid) {
	
	_vid = m_vid;
	return _vid;
}

std::string VisitorHashd::getId() {
	
	return m_vid;
}

void VisitorHashd::dump(std::string &_dump) {
	
	punkt::Visitor pb;
	
	std::set<std::string>::iterator it = m_unique_props.begin();
	std::set<std::string>::iterator end = m_unique_props.end();
	
	while (it != end) {
		pb.add_unique_props(*it);
		it++;
	}
	
	pb.set_vid(m_vid);
	
	for (size_t i = 0; i<devices.size(); i++) {
		
		punkt::VisitDevice* device_pb = pb.add_devices();
		devices[i].dump(device_pb);
	}
	
	for (size_t i = 0; i<ext.size(); i++) {
		
		punkt::VisitorExt *s = pb.add_ext();
		ext[i].dump(s);
	}
	
	pb.set_tried_vk_matching(tried_vk_matching);
	
	if (vk_profile) 
		vk_profile->dump(pb.mutable_vk_profile()); 
	
	_dump = pb.SerializeAsString();
}

void VisitorHashd::parseProtobuf(const std::string &_dump) {
	
	
	punkt::Visitor pb;
	pb.ParseFromString(_dump);
	
	m_vid = pb.vid();
	
	for (size_t i = 0; i<pb.devices_size(); i++) {
		
		punkt::VisitDevice device_pb = pb.devices(i);
		devices.push_back(VisitDevice(device_pb));
	}
	
	for (size_t i = 0; i<pb.unique_props_size(); i++) {
		
		m_unique_props.insert(pb.unique_props(i));
	}
	
	for (size_t i = 0; i< pb.ext_size(); i++) {
		
		ext.push_back( VisitorExt(pb.ext(i)) );
	}
	
	tried_vk_matching = pb.tried_vk_matching();
	
	if (pb.has_vk_profile()) {
		
		tried_vk_matching = true;
		vk_profile.reset(new VkProfile(pb.vk_profile()));
	}	
}

void VisitorHashd::addQuery(const std::string &_q) {
	
	cur_device->search_queries.push_back(_q);	
}

void VisitorHashd::getQueries(std::vector<std::string> &_q) {
	
	for (size_t i = 0; i<devices.size(); i++) {
		for (size_t j = 0; j<devices[i].search_queries.size(); j++)
			_q.push_back(devices[i].search_queries[j]);
	}
}

void VisitorHashd::save() {
	
	m_onSave(this);
}

VisitorHashd::~VisitorHashd() {
	
}
