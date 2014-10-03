#ifndef _TARGETER_HASHD_H_
#define _TARGETER_HASHD_H_

#include "hiconfig.h"
#include "Targeter.h"
#include "VisitorHashd.h"
#include "VisitorsStorage.h"
#include "Visitor.h"

#include "zeit_client_async.h"

#include "Place.h"

class TargeterFull : public Targeter {
	
	hAutoLock lock;
	hiaux::hashtable<uint64_t, AdPtr> m_ads;
	hiaux::hashtable<uint64_t, PlacePtr> m_places;
	//hiaux::hashtable<uint64_t, PlaceTargetsPtr> m_places;
	
	VisitorsStoragePtr m_storage;
	ZeitClientAsyncPtr m_zeit_acli;
	
	std::string m_punkt_rsrc_url;
	
	void genVdid(std::string &_vdid) const;
	void handleClickEvent(uint64_t _pid, uint64_t _adid, const std::map<std::string, std::string> &_params, HttpConnectionPtr _conn, HttpRequestPtr _req);
	void handleDispEvent(uint64_t _pid, uint64_t _adid, const std::map<std::string, std::string> &_params, HttpConnectionPtr _conn, HttpRequestPtr _req);
	void saveVisitor(VisitorHashd *_v);
	void onCalledZeit (bool _success);
	
	AdTargeterArgsPtr parseAdTargeterArgs(const std::string &_targeter_args_str);
	
public:
	
	TargeterFull(const std::string &_repl_id, VisitorsStoragePtr _storage,
			const std::string &_punkt_rsrc_url,
			ZeitClientAsyncPtr _zeit_acli,
			boost::function<FormatterArgsPtr(uint64_t _format_id, const std::string &_args)> _parseFormatterArgs);
	
	virtual void getVisitor(HttpConnectionPtr _conn, HttpRequestPtr _req, boost::function<void(VisitorPtr)> _onGot);
	
	virtual void updateAd(uint64_t _id,
							uint64_t _format_id,
							uint64_t _ownerid,
							const std::string &_formatter_args_str,
							const std::string &_targeter_args_str);
	
	virtual void updatePlace(uint64_t _pid, const std::string &_targeter_args, const std::vector<uint64_t> &_targets);
	
	virtual void updateCampaign(uint64_t _cid,
								const std::string &_targeter_args,
								const std::vector<uint64_t> &_ads);

	virtual AdPtr getAd(uint64_t _adid);
	virtual uint64_t getAdOwner(uint64_t _adid);

	virtual AdPtr getAdToShow(uint64_t _pid, VisitorPtr _visitor, std::vector<std::string> &_queries, std::string &_extcode);
	
	virtual ETN* getCustomEventsRouter();
	virtual void handleEvent(const std::string &_method, uint64_t _pid, uint64_t _adid, const std::map<std::string, std::string> &_params, HttpConnectionPtr _conn, HttpRequestPtr _req);
	
	virtual ~TargeterFull();
};

typedef boost::shared_ptr<TargeterFull> TargeterFullPtr;

#endif
