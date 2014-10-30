#ifndef _VISITOR_COOKIE_ONLY_H_
#define _VISITOR_COOKIE_ONLY_H_

#include "Targeter.h"
#include "Visitor.h"
#include "hiaux/crypt/base64.h"
#include "VisitorCookie.pb.h"
#include <math.h>

class VisitorCookieOnly : public Visitor {
	boost::function<void(VisitorCookieOnly*)> m_onSave;
public:
	
	std::vector<std::string> m_queries;
	HttpConnectionPtr m_conn;

	hiaux::hashtable<uint64_t, int> ads_disped_today;
	hiaux::hashtable<uint64_t, int> places_today;

	uint64_t stats_day_begin_ts;

	VisitorCookieOnly(const std::string &_vid,
						boost::function<void(VisitorCookieOnly*)> _onSave,
						HttpConnectionPtr _conn);
	
	virtual void addQuery(const std::string &_q);
	virtual void getQueries(std::vector<std::string> &_q);
	virtual void save();
	
	
	void restore(const std::string &_dump);
	void dump(std::string &_dump);
	
	virtual ~VisitorCookieOnly();
};

typedef boost::shared_ptr<VisitorCookieOnly> VisitorCookieOnlyPtr;


#endif
