#include "VisitDevice.h"

VisitDevice::VisitDevice(const std::string &_vdid, const std::string &_user_agent, uint64_t _create_ts):
	 vdid(_vdid),
	 user_agent(_user_agent),
	 create_ts(_create_ts) {
	
}

AdDisps::AdDisps(const punkt::VisitDevice::AdDisps &_pb) {
	
	adid = _pb.adid();
	n = _pb.n();
}

void AdDisps::dump(punkt::VisitDevice::AdDisps *_pb) {
	
	_pb->set_adid(adid);
	_pb->set_n(n);
}

AdClicks::AdClicks(const punkt::VisitDevice::AdClicks &_pb) {
	
	adid = _pb.adid();
	n = _pb.n();
}

void AdClicks::dump(punkt::VisitDevice::AdClicks *_pb) {
	
	_pb->set_adid(adid);
	_pb->set_n(n);
}

VisitorExt::VisitorExt(const punkt::VisitorExt &_pb) {
	
	extid = _pb.extid();
	data = _pb.data();
}

void VisitorExt::dump(punkt::VisitorExt *_pb) {
	
	_pb->set_extid(extid);
	_pb->set_data(data);
}

VisitDevice::VisitDevice(const punkt::VisitDevice &_pb) {
	
	vdid = _pb.vdid();
	create_ts = _pb.create_ts();
	user_agent = _pb.user_agent();
	
	for (size_t i = 0; i< _pb.search_queries_size(); i++) {
		
		search_queries.push_back(_pb.search_queries(i));
	}
	
	for (size_t i = 0; i< _pb.domains_visited_size(); i++) {
		
		domains_visited.push_back(_pb.domains_visited(i));
	}
	
	for (size_t i = 0; i< _pb.ads_disped_size(); i++) {
		
		ads_disped.push_back( AdDisps(_pb.ads_disped(i)) );
	}
	
	for (size_t i = 0; i< _pb.ads_clicked_size(); i++) {
		
		ads_clicked.push_back( AdClicks(_pb.ads_clicked(i)) );
	}
	
	for (size_t i = 0; i< _pb.ext_size(); i++) {
		
		ext.push_back( VisitorExt(_pb.ext(i)) );
	}
}

void VisitDevice::dump(punkt::VisitDevice *_pb) {
	
//	std::cout << "VisitDevice::dump\n";
	
	_pb->set_vdid(vdid);
	_pb->set_create_ts(create_ts);
	_pb->set_user_agent(user_agent);
	
	for (size_t i = 0; i<search_queries.size(); i++) {
		std::string *s = _pb->add_search_queries();
		*s = search_queries[i];
	}
	
	for (size_t i = 0; i<domains_visited.size(); i++) {
		std::string *s = _pb->add_domains_visited();
		*s = domains_visited[i];
	}
	
	for (size_t i = 0; i< ads_disped.size(); i++) {
		
		punkt::VisitDevice::AdDisps *_ad_disps_pb = _pb->add_ads_disped();
		ads_disped[i].dump(_ad_disps_pb);
	}
	
	for (size_t i = 0; i< ads_clicked.size(); i++) {
		
		punkt::VisitDevice::AdClicks *_ad_clicked_pb = _pb->add_ads_clicked();
		ads_clicked[i].dump(_ad_clicked_pb);
	}
	
	for (size_t i = 0; i< ext.size(); i++) {
		
		punkt::VisitorExt *ext_pb = _pb->add_ext();
		ext[i].dump(ext_pb);
	}
}
