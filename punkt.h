#ifndef _PUNKT_H_
#define _PUNKT_H_

#include "hiconfig.h"
#include "hiaux/network/HttpServer/HttpServer.h"

#include "punkt_consts.h"
#include "ad.h"
#include "formatter.h"
#include "Targeter.h"

#include "hiaux/strings/RefParser.h"

#include "EventsTree.h"

class Punkt {
public:
	
	typedef boost::function<void(HttpConnectionPtr, HttpRequestPtr, FormatterArgsPtr)> FormatterFun;

private:
	
	hiaux::hashtable<uint64_t, FormatterPtr> m_formatters;
	
	hAutoLock lock;
	
	TargeterPtr m_targeter;
	std::string m_systemid;
	std::string m_replid;
	const std::string m_punkt_rsrc_url;

	ETN *m_event_router;

	void handleFormatEvent(const std::map<std::string, std::string> &_params, HttpConnectionPtr _conn, HttpRequestPtr _req);	
	void handleTargeterEvent(const std::string &_method, const std::map<std::string, std::string> &_params, HttpConnectionPtr _conn, HttpRequestPtr _req);
	
	std::string getVkAuthCode(const std::string &_domain);
	
public:
	
	Punkt(TargeterPtr _targeter,
		const std::string &_systemid,
		const std::string &_replid,
		const std::string &_punkt_rsrc_url);
	
	void updateAd(AdPtr _ad);
	void updatePlaceTargets(uint64_t _pid, const std::vector<uint64_t> &_targets);
	void updateFormatter(uint64_t _fid, FormatterPtr _formatter);
	
	uint64_t getAdOwner(uint64_t _adid);
	
	void handleDemo(const std::map<std::string, std::string> &_params, HttpConnectionPtr _conn, HttpRequestPtr _req);
	void handleDemoGotVisitor(VisitorPtr _visitor, HttpConnectionPtr _conn, HttpRequestPtr _req);
	
	void handlePlace(const std::map<std::string, std::string> &_params, HttpConnectionPtr _conn, HttpRequestPtr _req);
	void handlePlaceGotVisitor(uint64_t _pid, VisitorPtr _visitor, HttpConnectionPtr _conn, HttpRequestPtr _req);
	
	void connHandler(HttpConnectionPtr _conn, HttpRequestPtr _req);
};

typedef boost::shared_ptr<Punkt> PunktPtr;

#endif
