#ifndef _TARGETER_H_
#define _TARGETER_H_

#include "hiconfig.h"
#include "hiaux/network/HttpServer/HttpServer.h"

#include "ad.h"
#include "PlaceTargets.h"

#include "Visitor.h"

#include "EventsTree.h"

class Targeter {
	
protected:
	std::string m_repl_id;
	boost::function<uint64_t(uint64_t)> m_getAdOwner;
public:
		
	typedef boost::shared_ptr<Visitor> VisitorPtr;
	
	Targeter(const std::string &_repl_id);
	
	virtual void getVisitor(HttpConnectionPtr _conn, HttpRequestPtr _req, boost::function<void(VisitorPtr)> _onGot) = 0;
	
	virtual void updateAd(AdPtr _ad) = 0;
	virtual void updatePlaceTargets(uint64_t _pid, const std::vector<uint64_t> &_targets) = 0;
	
	virtual AdPtr getAd(uint64_t _adid) = 0;
	virtual uint64_t getAdOwner(uint64_t _adid) = 0;
	
	virtual AdPtr getAdToShow(uint64_t _pid, VisitorPtr _visitor, std::vector<std::string> &_queries, std::string &_extcode) = 0;
	
	virtual ETN* getCustomEventsRouter() = 0;
	virtual void handleEvent(const std::string &_method, uint64_t _pid, uint64_t _adid, const std::map<std::string, std::string> &_params, HttpConnectionPtr _conn, HttpRequestPtr _req) = 0;
	
	virtual ~Targeter();
};

typedef boost::shared_ptr<Targeter> TargeterPtr;

#endif
