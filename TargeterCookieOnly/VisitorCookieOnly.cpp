#include "VisitorCookieOnly.h"

VisitorCookieOnly::VisitorCookieOnly(const std::string &_vid,
									boost::function<void(VisitorCookieOnly*)> _onSave,
									HttpConnectionPtr _conn):
	Visitor::Visitor(_vid),
	m_onSave(_onSave),
	m_conn(_conn),
	stats_day_begin_ts(0) {
	
}

void VisitorCookieOnly::addQuery(const std::string &_q) {
	
	m_queries.push_back(_q);
}

void VisitorCookieOnly::getQueries(std::vector<std::string> &_q) {
	
	_q = m_queries;
}

void VisitorCookieOnly::save() {
	
	m_onSave(this);
}

void VisitorCookieOnly::restore(const std::string &_dump) {
	
	//std::cout << "VisitorCookieOnly::restore\n";
	
	try {
		
		//std::cout << "got dump: " << _dump << std::endl;
		std::string dump = base64_decode(_dump);
		
		
		VisitorCookie pb;
		pb.ParseFromString(dump);
		for (int i = 0; i<pb.queries_size(); i++) {
			m_queries.push_back(pb.queries(i));
	//		std::cout << "VisitorCookieOnly::restore q: " << m_queries[m_queries.size() - 1];
		}
		
		for (size_t i = 0; i< pb.ads_disped_today_size(); i++) {
			
			//std::cout << "restoring ad disp\n";
			ads_disped_today[ pb.ads_disped_today(i).id() ] = pb.ads_disped_today(i).n();
		}
		
		for (size_t i = 0; i< pb.places_today_size(); i++) {
		
			//std::cout << "restoring place\n";
			places_today[ pb.places_today(i).id() ] = pb.places_today(i).n();
		}
		
		stats_day_begin_ts = pb.stats_day_begin_ts();
	
		uint64_t cur_beginning_of_day_ts = getBeginningOfDayTs(time(0));
	
		
		if (cur_beginning_of_day_ts > stats_day_begin_ts) {
		
			ads_disped_today.clear();
			places_today.clear();
			stats_day_begin_ts = cur_beginning_of_day_ts;
		}
		
	} catch (...) {
		
		std::cout << "VisitorCookieOnly::restore exception\n";
	}
}

void VisitorCookieOnly::dump(std::string &_dump) {
	
	std::string dump;
	VisitorCookie pb;
	
	for (int i = 0; i<fmin(m_queries.size(), 5); i++)
		pb.add_queries(m_queries[i]);
	
	hiaux::hashtable<uint64_t, int>::const_iterator it = ads_disped_today.begin();
	hiaux::hashtable<uint64_t, int>::const_iterator end = ads_disped_today.end();
	
	while (it != end) {
				
		VisitorCookie::EvCount *ad_disps_pb = pb.add_ads_disped_today();
		ad_disps_pb->set_id( it->first );
		ad_disps_pb->set_n( it->second );
		it++;
	}
	
	it = places_today.begin();
	end = places_today.end();
	
	while (it != end) {
		
		VisitorCookie::EvCount *places_today_pb = pb.add_places_today();
		places_today_pb->set_id( it->first );
		places_today_pb->set_n( it->second );
		it++;
	}
	
	pb.set_stats_day_begin_ts(stats_day_begin_ts);
	
	dump = pb.SerializeAsString();
	
	//std::cout << "dump size: " << dump.size() << std::endl;
	
	_dump = base64_encode((unsigned char *)dump.data(), dump.size());
	//std::cout << "dumping: " << _dump << std::endl;
}

VisitorCookieOnly::~VisitorCookieOnly() {
	
}
