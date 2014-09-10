#ifndef _TARGETER_H_
#define _TARGETER_H_

#include "hiconfig.h"
#include "hiaux/network/HttpSrv.h"

#include "ad.h"
#include "PlaceTargets.h"

class Visitor {
protected:
	std::string m_vid;
public:
	
	Visitor(const std::string &_vid);
	void getId(std::string &_vid);
	virtual void addQuery(const std::string &_q) = 0;
	virtual void getQueries(std::vector<std::string> &_q) = 0;
	virtual void save() = 0;
	virtual ~Visitor();
};

typedef boost::shared_ptr<Visitor> VisitorPtr;

class Targeter {
	
	std::string m_repl_id;

public:
		
	typedef boost::shared_ptr<Visitor> VisitorPtr;
	
	Targeter(const std::string &_repl_id);
	
	virtual void getVisitor(HttpSrv::ConnectionPtr _conn, HttpSrv::RequestPtr _req, boost::function<void(VisitorPtr)> _onGot) = 0;
	
	virtual void updateAd(AdPtr _ad) = 0;
	virtual void updatePlaceTargets(uint64_t _pid, const std::vector<uint64_t> &_targets) = 0;
	
	virtual AdPtr getAd(uint64_t _adid) = 0;
	virtual uint64_t getAdOwner(uint64_t _adid) = 0;
	
	virtual AdPtr getAdToShow(uint64_t _pid, VisitorPtr _visitor, std::vector<std::string> &_queries) = 0;
	
	virtual ~Targeter();
};

typedef boost::shared_ptr<Targeter> TargeterPtr;

#endif
