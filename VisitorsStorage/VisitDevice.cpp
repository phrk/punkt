#include "VisitDevice.h"
#include "hiaux/strings/string_utils.h"

VisitDevice::VisitDevice(const std::string &_vdid, const std::string &_user_agent, uint64_t _create_ts):
	 vdid(_vdid),
	 user_agent(_user_agent),
	 create_ts(_create_ts) {
	
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
	
	for (size_t i = 0; i< _pb.ads_disped_today_size(); i++) {
		
		ads_disped_today[ _pb.ads_disped_today(i).id() ] = _pb.ads_disped_today(i).n();
	}
	
	for (size_t i = 0; i< _pb.ads_clicked_today_size(); i++) {
		
		ads_clicked_today[ _pb.ads_clicked_today(i).id() ] = _pb.ads_clicked_today(i).n();
	}
	
	for (size_t i = 0; i< _pb.places_today_size(); i++) {
		
		places_today[ _pb.places_today(i).id() ] = _pb.places_today(i).n();
	}
	
	for (size_t i = 0; i< _pb.domains_today_size(); i++) {
		
		domains_today[ _pb.domains_today(i).id() ] = _pb.domains_today(i).n();
	}
	
	for (size_t i = 0; i< _pb.ext_size(); i++) {
		
		ext.push_back( VisitorExt(_pb.ext(i)) );
	}
	
	stats_day_begin_ts = _pb.stats_day_begin_ts();
	
	uint64_t cur_beginning_of_day_ts = getBeginningOfDayTs(time(0));
	
	if (cur_beginning_of_day_ts > stats_day_begin_ts) {
		
		ads_disped_today.clear();
		ads_clicked_today.clear();
		places_today.clear();
		domains_today.clear();
		stats_day_begin_ts = cur_beginning_of_day_ts;
	}
}

void VisitDevice::dump(punkt::VisitDevice *_pb) {
	
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
	
	
	hiaux::hashtable<uint64_t, int>::const_iterator it = ads_disped_today.begin();
	hiaux::hashtable<uint64_t, int>::const_iterator end = ads_disped_today.end();
	
	while (it != end) {
		
		punkt::VisitDevice::EvCount *_ad_disps_pb = _pb->add_ads_disped_today();
		_ad_disps_pb->set_id( it->first );
		_ad_disps_pb->set_n( it->second );
		it++;
	}
	
	it = ads_clicked_today.begin();
	end = ads_clicked_today.end();
	
	while (it != end) {
		
		punkt::VisitDevice::EvCount *_ad_clicked_today_pb = _pb->add_ads_clicked_today();
		_ad_clicked_today_pb->set_id( it->first );
		_ad_clicked_today_pb->set_n( it->second );
		it++;
	}

	it = places_today.begin();
	end = places_today.end();
	
	while (it != end) {
		
		punkt::VisitDevice::EvCount *_places_today_pb = _pb->add_places_today();
		_places_today_pb->set_id( it->first );
		_places_today_pb->set_n( it->second );
		it++;
	}

	it = domains_today.begin();
	end = domains_today.end();
	
	while (it != end) {
		
		punkt::VisitDevice::EvCount *_domains_today_pb = _pb->add_domains_today();
		_domains_today_pb->set_id( it->first );
		_domains_today_pb->set_n( it->second );
		it++;
	}

	for (size_t i = 0; i< ext.size(); i++) {
		
		punkt::VisitorExt *ext_pb = _pb->add_ext();
		ext[i].dump(ext_pb);
	}
	
	_pb->set_stats_day_begin_ts(stats_day_begin_ts);
}
