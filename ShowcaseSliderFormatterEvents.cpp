#include "ShowcaseSliderFormatter.h"


void ShowcaseSliderFormatter::handleShowDispEvent(HttpSrv::ConnectionPtr _conn, HttpSrv::RequestPtr _req) {
		
	std::string pid_str;
	std::string adid_str;
	std::string aim;
	//uint64_t pid;
	uint64_t ad_owner;
	
	if (!_req->getField("pid", pid_str)) {
		return;
	}
	
	if (!_req->getField("adid", adid_str)) {
		return;
	}
	
	if (!_req->getField("aim", aim)) {
		return;
	}
	
	try {
		
		ad_owner = m_getAdOwner(string_to_uint64(adid_str)) ;
		
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
	m_zeit_acli->mergeCounter(owner_ads_place_counter, time(0), 1, boost::bind(&ShowcaseSliderFormatter::onCalledZeit, this, _1));
	m_zeit_acli->mergeCounter(owner_places_ad_counter, time(0), 1, boost::bind(&ShowcaseSliderFormatter::onCalledZeit, this, _1));
	
	_conn->sendResponse("{ \"status\" : \"ShowcaseSliderFormatter::handleShowDispEvent\" }");
}

void ShowcaseSliderFormatter::handleItemDispEvent(HttpSrv::ConnectionPtr _conn, HttpSrv::RequestPtr _req) {
	
	std::string pid_str;
	std::string adid_str;
	std::string item_str;
	uint64_t ad_owner;
	
	if (!_req->getField("pid", pid_str)) {
		return;
	}
	
	if (!_req->getField("adid", adid_str)) {
		return;
	}
	
	if (!_req->getField("item", item_str)) {
		return;
	}
	
	try {
		ad_owner = m_getAdOwner(string_to_uint64(adid_str)) ;
		
	} catch (...) {
		return;
	}
	
	std::string ad_owner_str = uint64_to_string(ad_owner);
	
	std::string owner_ad_item_counter = "owners:owner_ads-" + ad_owner_str + ":items_ad-" + adid_str + ":disps:item-" + item_str;
	std::string owner_place_item_counter = "owners:owner_places-" + ad_owner_str + ":items_place-" + pid_str + ":disps:item-" + item_str;
	std::string owner_item_counter = "owners:owner_items-" + ad_owner_str + ":disps:item-" + item_str;
	
	m_zeit_acli->mergeCounter(owner_ad_item_counter, time(0), 1, boost::bind(&ShowcaseSliderFormatter::onCalledZeit, this, _1));
	m_zeit_acli->mergeCounter(owner_place_item_counter, time(0), 1, boost::bind(&ShowcaseSliderFormatter::onCalledZeit, this, _1));
	m_zeit_acli->mergeCounter(owner_item_counter, time(0), 1, boost::bind(&ShowcaseSliderFormatter::onCalledZeit, this, _1));
	
	std::cout << owner_ads_counter << "\n" << owner_ads_place_counter << "\n" << owner_items_counter << "\n";
	
	_conn->sendResponse("{ \"status\" : \"ShowcaseSliderFormatter::handleItemsShowEvent\" }");
}

void ShowcaseSliderFormatter::handleClickEvent(HttpSrv::ConnectionPtr _conn, HttpSrv::RequestPtr _req) {
	
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
	
	std::string pid_str;
	std::string adid_str;
	std::string item_str;
	//uint64_t pid;
	uint64_t ad_owner;
	
	if (!_req->getField("pid", pid_str)) {
		return;
	}
	
	if (!_req->getField("adid", adid_str)) {
		return;
	}
	
	if (!_req->getField("item", item_str)) {
		return;
	}
	
	try {
		ad_owner = m_getAdOwner(string_to_uint64(adid_str)) ;
		
	} catch (...) {
		return;
	}
	
	std::string ad_owner_str = uint64_to_string(ad_owner);
	
	std::string owner_ad_place_counter = "owners:owner_ads-" + ad_owner_str
										+ ":places_ad-" + adid_str 
										+ ":clicks:place-" + pid_str;
	
	std::string owner_ad_item_counter = "owners:owner_ads-" + ad_owner_str 
										+ ":items_ad-" + adid_str 
										+ ":clicks:item-" + item_str;
	
	std::string owner_place_ad_counter = "owners:owner_places-" + ad_owner_str 
										+ ":ads_place-" + pid_str 
										+ ":clicks:ad-" + adid_str;
	
	std::string owner_place_item_counter = "owners:owner_places-" + ad_owner_str 
										+ ":items_place-" + pid_str 
										+ ":clicks:item-" + item_str;
	
	std::string owner_item_counter = "owners:owner_items-" + ad_owner_str + ":clicks:item-" + item_str;
	
	m_zeit_acli->mergeCounter(owner_ad_place_counter, time(0), 1, boost::bind(&ShowcaseSliderFormatter::onCalledZeit, this, _1));
	m_zeit_acli->mergeCounter(owner_ad_item_counter, time(0), 1, boost::bind(&ShowcaseSliderFormatter::onCalledZeit, this, _1));
	m_zeit_acli->mergeCounter(owner_place_ad_counter, time(0), 1, boost::bind(&ShowcaseSliderFormatter::onCalledZeit, this, _1));
	m_zeit_acli->mergeCounter(owner_place_item_counter, time(0), 1, boost::bind(&ShowcaseSliderFormatter::onCalledZeit, this, _1));
	m_zeit_acli->mergeCounter(owner_item_counter, time(0), 1, boost::bind(&ShowcaseSliderFormatter::onCalledZeit, this, _1));
}

void ShowcaseSliderFormatter::handleConvEvent(HttpSrv::ConnectionPtr _conn, HttpSrv::RequestPtr _req) {
	
	std::string clickid;
	if (!_req->getField("clickid", clickid)) {
		
		_conn->sendResponse("{ \"status\" : \"clickid not set\" }");
		_conn->close();
		return;
	}
	
	std::string pid;
	std::string adid;
	std::string itemid;
	std::string ad_owner;
	std::vector<std::string> strs;
	split(clickid, '_', strs);
	
	if (strs.size() == 3) {
		pid = strs[0];
		adid = strs[1];
		itemid = strs[2];
	}
	
	try {
		ad_owner = m_getAdOwner(string_to_uint64(adid));
		
	} catch (...) {
		return;
	}
	
	std::string owner_ads_counter = "owners:owner_ads-" + ad_owner + ":ad-" + adid + ":convs:place-" + pid;
	
	m_zeit_acli->mergeCounter(owner_ads_counter, time(0), 1, boost::bind(&ShowcaseSliderFormatter::onCalledZeit, this, _1));
	//std::cout << "conversion happened: pid: " << pid << " adid: " << adid << " itemid: " << itemid << std::endl; 
	
	_conn->sendResponse("{ \"status\": \"ok\" }");
	_conn->close();
}

void ShowcaseSliderFormatter::handleFormatEvent(HttpSrv::ConnectionPtr _conn, HttpSrv::RequestPtr _req) {

	std::string ev_str;
	if (!_req->getField("ev", ev_str)) {
		
		_conn->sendResponse("{ \"status\" : \"ev not set\" }");
		_conn->close();
		return;
	}

	if (ev_str == "showdisp") {
		
		handleShowDispEvent(_conn, _req);

	} if (ev_str == "itemdisp") {
		
		handleItemDispEvent(_conn, _req);

	} else if (ev_str == "click") {
		
		handleClickEvent(_conn, _req);

	} else if (ev_str == "conv") {
		
		handleConvEvent(_conn, _req);

	} else {
		_conn->sendResponse("{ \"status\" : \"ShowcaseSliderFormatter::handleFormatEvent unknown event\" }");
	}

	//_conn->sendResponse("{ \"status\" : \"ShowcaseSliderFormatter::handleFormatEvent called\" }");
	_conn->close();
}

void ShowcaseSliderFormatter::onCalledZeit (bool _success) {
	
	if (!_success)
		std::cout << "zeit call fail" << std::endl;
}

