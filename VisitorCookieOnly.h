#ifndef _VISITOR_COOKIE_ONLY_H_
#define _VISITOR_COOKIE_ONLY_H_

#include "Targeter.h"
#include "hiaux/crypt/base64.h"
#include "VisitorCookie.pb.h"

class VisitorCookieOnly : public Visitor {
	boost::function<void(VisitorCookieOnly*)> m_onSave;
public:
	
	std::vector<std::string> m_queries;
	HttpSrv::ConnectionPtr m_conn;

	VisitorCookieOnly(const std::string &_vid,
						boost::function<void(VisitorCookieOnly*)> _onSave,
						HttpSrv::ConnectionPtr _conn);
	
	virtual void addQuery(const std::string &_q);
	virtual void getQueries(std::vector<std::string> &_q);
	virtual void save();
	
	
	void restore(const std::string &_dump);
	void dump(std::string &_dump);
	
	virtual ~VisitorCookieOnly();
};

typedef boost::shared_ptr<VisitorCookieOnly> VisitorCookieOnlyPtr;


#endif
