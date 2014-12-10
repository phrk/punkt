#include "TargeterCookieOnly.h"

void TargeterCookieOnly::handleClickEvent(uint64_t _pid,
									uint64_t _adid,
									const std::map<std::string, std::string> &_params,
									HttpConnectionPtr _conn,
									HttpRequestPtr _req) {

	std::string aim;

	if (!_req->getField("aim", aim)) {

	}

	std::string resp =
	"<!DOCTYPE HTML>"
		"<html>"
		"<head>"
		" <title>Перенаправление</title>"
		"</head>"
		"<body>"
				"Если вы не перенаправлены автоматически, проследуйте <a href=\'" + aim + "\'>по этой ссылке</a>"
		"</body>"
	"</html>";

	
	HttpResponse res(302, resp);
	res.addHeader("Location: " + aim);
	
	_conn->sendResponse(res);

	std::string pid_str = _params.at("pid");
	std::string adid_str = _params.at("adid");
//	std::string item_str 
	//uint64_t pid;
	uint64_t ad_owner;


//	if (!_req->getField("item", item_str)) {
//		return;
//	}

	try {
		ad_owner = getAdOwner(_adid) ;

	} catch (...) {
		return;
	}

	std::string ad_owner_str = uint64_to_string(ad_owner);

	std::string owner_ad_place_counter = "owners:owner_ads-" + ad_owner_str
										+ ":places_ad-" + adid_str 
										+ ":clicks:place-" + pid_str;

//	std::string owner_ad_item_counter = "owners:owner_ads-" + ad_owner_str 
//										+ ":items_ad-" + adid_str 
//										+ ":clicks:item-" + item_str;

	std::string owner_place_ad_counter = "owners:owner_places-" + ad_owner_str 
										+ ":ads_place-" + pid_str 
										+ ":clicks:ad-" + adid_str;

//	std::string owner_place_item_counter = "owners:owner_places-" + ad_owner_str 
//										+ ":items_place-" + pid_str 
//										+ ":clicks:item-" + item_str;

//	std::string owner_item_counter = "owners:owner_items-" + ad_owner_str + ":clicks:item-" + item_str;

	m_zeit_acli->mergeCounter(owner_ad_place_counter, time(0), 1, boost::bind(&TargeterCookieOnly::onCalledZeit, this, _1));
//	m_zeit_acli->mergeCounter(owner_ad_item_counter, time(0), 1, boost::bind(&ShowcaseSliderFormatter::onCalledZeit, this, _1));
	m_zeit_acli->mergeCounter(owner_place_ad_counter, time(0), 1, boost::bind(&TargeterCookieOnly::onCalledZeit, this, _1));
//	m_zeit_acli->mergeCounter(owner_place_item_counter, time(0), 1, boost::bind(&ShowcaseSliderFormatter::onCalledZeit, this, _1));
//	m_zeit_acli->mergeCounter(owner_item_counter, time(0), 1, boost::bind(&ShowcaseSliderFormatter::onCalledZeit, this, _1));

}

void TargeterCookieOnly::handleDispEvent(uint64_t _pid,
									uint64_t _adid,
									const std::map<std::string, std::string> &_params,
									HttpConnectionPtr _conn,
									HttpRequestPtr _req) {
	
	std::string pid_str = _params.at("pid");
	std::string adid_str = _params.at("adid");

	uint64_t ad_owner;

	try {

		ad_owner = getAdOwner(string_to_uint64(adid_str)) ;

	} catch (...) {
		return;
	}

	std::string ad_owner_str = uint64_to_string(ad_owner);

	std::string owner_ads_place_counter = "owners:owner_ads-" + ad_owner_str
									+ ":places_ad-" + adid_str 
									+ ":disps:place-" + pid_str;

	std::string owner_places_ad_counter = "owners:owner_places-" + ad_owner_str 
										+ ":ads_place-" + pid_str 
										+ ":disps:ad-" + adid_str;

	// call zeit
	m_zeit_acli->mergeCounter(owner_ads_place_counter, time(0), 1, boost::bind(&TargeterCookieOnly::onCalledZeit, this, _1));
	m_zeit_acli->mergeCounter(owner_places_ad_counter, time(0), 1, boost::bind(&TargeterCookieOnly::onCalledZeit, this, _1));

//	std::cout << owner_ads_place_counter << std::endl;
	
	getVisitor(_conn, _req, boost::bind(&TargeterCookieOnly::saveDispStats, this, _1, ad_owner, _pid, _adid));
}

void TargeterCookieOnly::saveDispStats(VisitorPtr _v, uint64_t _ownerid, uint64_t _pid, uint64_t _adid) {
	
	VisitorCookieOnly* v = (VisitorCookieOnly*)_v.get();
	
	std::string owner_ads_place_counter = "owners:owner_ads-" + uint64_to_string(_ownerid)
									+ ":places_ad-" + uint64_to_string(_adid)
									+ ":visitors:place-" + uint64_to_string(_pid);

	std::string owner_places_ad_counter = "owners:owner_places-" + uint64_to_string(_ownerid) 
										+ ":ads_place-" + uint64_to_string(_pid) 
										+ ":visitors:ad-" + uint64_to_string(_adid);
	
	
	if (v->ads_disped_today.find(_adid) == v->ads_disped_today.end()) {
		
		v->ads_disped_today[ _adid ] = 1;
		m_zeit_acli->mergeCounter(owner_ads_place_counter, time(0), 1, boost::bind(&TargeterCookieOnly::onCalledZeit, this, _1));
	}
	
	if (v->places_today.find(_pid) == v->places_today.end()) {
		
		v->places_today[ _pid ] = 1;
		m_zeit_acli->mergeCounter(owner_places_ad_counter, time(0), 1, boost::bind(&TargeterCookieOnly::onCalledZeit, this, _1));
	}
	
	
	//std::cout << "TargeterCookieOnly::saveDispStats " << std::endl;
	
	v->ads_disped_today[ _adid ] ++;
	v->places_today[ _pid ] ++;
	v->save();
		
	v->m_conn->sendResponse( HttpResponse(200, "{ \"status\" : \"ShowcaseSliderFormatter::handleShowDispEvent\" }") );
	//domains_today[]
}

void TargeterCookieOnly::onCalledZeit (bool _success) {
	
}

void TargeterCookieOnly::handleEvent(const std::string &_method, uint64_t _pid, uint64_t _adid, const std::map<std::string, std::string> &_params, HttpConnectionPtr _conn, HttpRequestPtr _req) {
	
	if (_method == "disp") {
		
		handleDispEvent(_pid, _adid, _params, _conn, _req);
		
	} else if (_method == "click") {
		
		handleClickEvent(_pid, _adid, _params, _conn, _req);
	}
}
