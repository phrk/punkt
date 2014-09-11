#include "VisitorCookieOnly.h"

VisitorCookieOnly::VisitorCookieOnly(const std::string &_vid,
									boost::function<void(VisitorCookieOnly*)> _onSave,
									HttpSrv::ConnectionPtr _conn):
	Visitor::Visitor(_vid),
	m_onSave(_onSave),
	m_conn(_conn) {
	
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
	
	try {
		std::string dump = base64_decode(_dump);
		VisitorCookie pb;
		pb.ParseFromString(dump);
		for (int i = 0; i<pb.queries_size(); i++) {
			m_queries.push_back(pb.queries(i));
	//		std::cout << "VisitorCookieOnly::restore q: " << m_queries[m_queries.size() - 1];
		}
	} catch (...) {
		
	}
}

void VisitorCookieOnly::dump(std::string &_dump) {
	
	std::string dump;
	VisitorCookie pb;
	
	for (int i = 0; i<fmin(m_queries.size(), 5); i++)
		pb.add_queries(m_queries[i]);
	
	dump = pb.SerializeAsString();
	_dump = base64_encode((unsigned char *)dump.c_str(), dump.size());
}

VisitorCookieOnly::~VisitorCookieOnly() {
	
}
