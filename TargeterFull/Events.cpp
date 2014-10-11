#include "TargeterFull.h"

void TargeterFull::handleClickEvent(uint64_t _pid,
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

	_conn->setHttpStatus(302); // Found
	_conn->addHeader("Location: " + aim);
	_conn->sendResponse(resp);

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

	m_zeit_acli->mergeCounter(owner_ad_place_counter, time(0), 1, boost::bind(&TargeterFull::onCalledZeit, this, _1));
//	m_zeit_acli->mergeCounter(owner_ad_item_counter, time(0), 1, boost::bind(&ShowcaseSliderFormatter::onCalledZeit, this, _1));
	m_zeit_acli->mergeCounter(owner_place_ad_counter, time(0), 1, boost::bind(&TargeterFull::onCalledZeit, this, _1));
//	m_zeit_acli->mergeCounter(owner_place_item_counter, time(0), 1, boost::bind(&ShowcaseSliderFormatter::onCalledZeit, this, _1));
//	m_zeit_acli->mergeCounter(owner_item_counter, time(0), 1, boost::bind(&ShowcaseSliderFormatter::onCalledZeit, this, _1));

}

void TargeterFull::handleDispEvent(uint64_t _pid,
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
	m_zeit_acli->mergeCounter(owner_ads_place_counter, time(0), 1, boost::bind(&TargeterFull::onCalledZeit, this, _1));
	m_zeit_acli->mergeCounter(owner_places_ad_counter, time(0), 1, boost::bind(&TargeterFull::onCalledZeit, this, _1));

//	std::cout << owner_ads_place_counter << std::endl;

	_conn->sendResponse("{ \"status\" : \"ShowcaseSliderFormatter::handleShowDispEvent\" }");
}

void TargeterFull::onCalledZeit (bool _success) {
	
}

void TargeterFull::handleEvent(const std::string &_method, uint64_t _pid, uint64_t _adid, const std::map<std::string, std::string> &_params, HttpConnectionPtr _conn, HttpRequestPtr _req) {
	
	if (_method == "disp") {
		
		handleDispEvent(_pid, _adid, _params, _conn, _req);
		
	} else if (_method == "click") {
		
		handleClickEvent(_pid, _adid, _params, _conn, _req);
	
	} else if (_method == "tm") {
		
		 
	}
}
