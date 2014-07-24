#include "ShowcaseSliderFormatter.h"

ShowcaseSliderFormatterArgs::ShowcaseSliderFormatterArgs(uint64_t _shid, int _nitems,
	 const hiaux::hashtable<std::string, std::string> &_partner_ids,
	 const hiaux::hashtable<std::string, std::string> &_click_templates,
	 const std::string &_json_dump):
	shid(_shid),
	nitems(_nitems),
	partner_ids(_partner_ids),
	click_templates(_click_templates),
	json_dump(_json_dump) {
}

ShowcaseSliderFormatter::ShowcaseSliderFormatter(HttpOutRequestDispPtr _req_disp,
						FileCachePtr _jscache,
						const std::string &_punkt_url,
						const std::string &_punkt_rsrc_url,
						boost::function<HttpSrv::ConnectionPtr(int)> _getConnById,
						GeberdCliApiClientAsyncPtr _geber_acli,
						ZeitClientAsyncPtr _zeit_acli,
						boost::function<uint64_t(uint64_t)> _getAdOwner):
	m_req_disp(_req_disp),
	m_jscache(_jscache),
	m_punkt_url(_punkt_url),
	m_punkt_rsrc_url(_punkt_rsrc_url),
	m_getConnById(_getConnById),
	m_geber_acli(_geber_acli),
	m_zeit_acli(_zeit_acli),
	m_getAdOwner(_getAdOwner) {
	
}

FormatterArgsPtr ShowcaseSliderFormatter::parseArgs(const std::string &_args_js) {

	std::cout << "ShowcaseSliderFormatter::parseArgs " <<  _args_js << std::endl;

	uint64_t shid;
	int nitems;
	hiaux::hashtable<std::string, std::string> partner_ids;
	hiaux::hashtable<std::string, std::string> click_templates;
	
	json_error_t error;
	json_t *root = json_loads(_args_js.c_str(), 0, &error);
	
	json_t *j_shid = json_object_get(root, "shid");
	if (json_is_integer(j_shid)) {
		shid = json_integer_value(j_shid);
	} else {
		std::cout << "ShowcaseSimpleFormatter::parseArgs could not parse shid\n";
		throw "ShowcaseSimpleFormatter::parseArgs could not parse shid";
	}
	
	json_t *j_nitems = json_object_get(root, "nitems");
	if (json_is_integer(j_nitems)) {
		nitems = json_integer_value(j_nitems);
	} else {
		std::cout << "ShowcaseSimpleFormatter::parseArgs could not parse nitems\n";
		throw "ShowcaseSimpleFormatter::parseArgs could not parse nitems";
	}
	
	json_t *j_partner_ids = json_object_get(root, "partner_ids");
	if (json_is_object(j_partner_ids)) {
		
		json_t *j_ozon_partnerid = json_object_get(j_partner_ids, "ozon");
		if (json_is_string(j_ozon_partnerid))
			partner_ids["ozon"] = json_string_value(j_ozon_partnerid);
		
		json_t *j_wikimart_partnerid = json_object_get(j_partner_ids, "wikimart");
		if (json_is_string(j_wikimart_partnerid))
			partner_ids["wikimart"] = json_string_value(j_wikimart_partnerid);
				
	} else {
		std::cout << "ShowcaseSimpleFormatter::parseArgs could not parse partner_ids\n";
		throw "ShowcaseSimpleFormatter::parseArgs could not parse partner_ids";
	}
	
	json_t *j_click_templates = json_object_get(root, "click_templates");
	if (json_is_object(j_click_templates)) {
		
		json_t *j_ozon_click_template = json_object_get(j_click_templates, "ozon");
		if (json_is_string(j_ozon_click_template)) {
			std::string ozon_template (json_string_value(j_ozon_click_template));
			if (ozon_template != "")
				click_templates["ozon"] = ozon_template;
		}
		
		json_t *j_wikimart_click_template = json_object_get(j_click_templates, "wikimart");
		if (json_is_string(j_wikimart_click_template)) {
			std::string wikimart_template(json_string_value(j_wikimart_click_template));
			if (wikimart_template != "")
				click_templates["wikimart"] = wikimart_template;
		}
				
	} else {
		std::cout << "ShowcaseSimpleFormatter::parseArgs could not parse click_templates\n";
		throw "ShowcaseSimpleFormatter::parseArgs could not parse click_templates";
	}
	
	json_decref(root);
	
	return FormatterArgsPtr(new ShowcaseSliderFormatterArgs(shid, nitems, partner_ids, click_templates, _args_js));
}

void ShowcaseSliderFormatter::format(HttpSrv::ConnectionPtr _conn, HttpSrv::RequestPtr _req, uint64_t _pid, uint64_t _adid, FormatterArgsPtr _args) {
	
	ShowcaseSliderFormatterArgs* args = (ShowcaseSliderFormatterArgs*)_args.get();
	m_geber_acli->getShowcase(args->shid, args->nitems, boost::bind(&ShowcaseSliderFormatter::onGotShowcase, this, _1, _2, _conn, _pid, _adid, _args));
}

void ShowcaseSliderFormatter::formatDemo(HttpSrv::ConnectionPtr _conn, HttpSrv::RequestPtr _req, uint64_t _pid, uint64_t _adid, FormatterArgsPtr _args) {
	
	ShowcaseSliderFormatterArgs* args = (ShowcaseSliderFormatterArgs*)_args.get();
	m_geber_acli->getShowcase(args->shid, args->nitems, boost::bind(&ShowcaseSliderFormatter::onGotShowcaseDemo, this, _1, _2, _conn, _pid, _adid, _args));
}

void ShowcaseSliderFormatter::renderClickTemplate(const std::string &_advid,
													uint64_t _pid,
													uint64_t _adid,
													uint64_t _itemid,
													const std::string _itemurl,
													const hiaux::hashtable<std::string, std::string> &_click_templates,
													std::string &_target) const {
	
	char clickid_c[256];
	sprintf(clickid_c, "%llu_%llu_%llu", _pid, _adid, _itemid);
	//std::string clickid(clickid_c);
	std::string itemurl = _itemurl;
	replaceSubstring(itemurl, "http://", "");
	// prepare itemurl 
	
	hiaux::hashtable<std::string, std::string>::const_iterator it = _click_templates.find(_advid);
	if (it == _click_templates.end()) {
		std::cout << "ShowcaseSliderFormatter::renderClickTemplate click template not found for advid: " << _advid << " adid: " << _adid << std::endl;
	}
		
	std::string aim = it->second;
	
	replaceSubstring(aim, "{target}", itemurl);
	replaceSubstring(aim, "{clickid}", clickid_c);
	
	escapeUrl(aim);
	_target = m_punkt_url + "?evtype=fev&fid=1&ev=click&pid=" + 
					uint64_to_string(_pid) + "&adid=" + uint64_to_string(_adid) + "&item=" + uint64_to_string(_itemid) + "&aim=" + aim;
}

void ShowcaseSliderFormatter::getPartnerParameter(const std::string &_advid,
												const hiaux::hashtable<std::string, std::string> &_partner_ids,
												std::string &_partner_param) const {
	
	if (_advid == OZON_ADVID) {
		
		hiaux::hashtable<std::string, std::string>::const_iterator it = _partner_ids.find(OZON_ADVID);
		if (it == _partner_ids.end()) {
			std::cout << "ShowcaseSliderFormatter::getPartnerParameter " << _advid << " partnerid not set " << std::endl;
			return;
		}
		// TODO: check for ? or &
		_partner_param += "&partner=" + it->second;
	}
}

void ShowcaseSliderFormatter::rebuildClickLinks(ShowcaseInstance &_show,
												uint64_t _pid,
												uint64_t _adid,
												const hiaux::hashtable<std::string, std::string> &_click_templates,
												const hiaux::hashtable<std::string, std::string> &_partner_ids) {
		
	for (int i = 0; i<_show.items.size(); i++) {
		
		if (_click_templates.find(_show.items[i].advid) != _click_templates.end()) {
			
			renderClickTemplate(_show.items[i].advid,
								_pid,
								_adid,
								_show.items[i].id,
								_show.items[i].directurl,
								_click_templates,
								_show.items[i].directurl);
			
		} else {
		
			std::string aim = _show.items[i].directurl;
			std::string partner_param;
			
			getPartnerParameter(_show.items[i].advid, _partner_ids, partner_param);
			aim += partner_param;

			escapeUrl(aim);
			_show.items[i].directurl = m_punkt_url + "?evtype=fev&fid=1&ev=click&pid=" + 
				uint64_to_string(_pid) + "&adid=" + uint64_to_string(_adid) + "&item=" + uint64_to_string(_show.items[i].id) + "&aim=" + aim;
		}
	}
}

void ShowcaseSliderFormatter::onGotShowcaseDemo (bool _success, ShowcaseInstance &_show, HttpSrv::ConnectionPtr _conn,
						uint64_t _pid, uint64_t _adid, FormatterArgsPtr _args) {

	if (!_success) {
		
		_conn->close();
		return;
	}
	
	if (_show.items.size()<3) {
		
		_conn->sendResponse("<font color=B22222>Добавьте больше категорий или товаров на витрину, чтобы она показывалась</font>");
		_conn->close();
	}
	
	ShowcaseSliderFormatterArgs* args = (ShowcaseSliderFormatterArgs*)_args.get();
	
	std::string slider_events;
	std::string render_slider;
	std::string mootools;
	std::string slider;
	
	if (!m_jscache->getFile("ShowcaseSliderEvents.js", slider_events)) {
		std::cout << "ShowcaseSliderFormatter::onCalledGeberOkDemo ShowcaseSliderEvents.js not loaded in cache  \n";
	}
	
	if (!m_jscache->getFile("renderShowcaseSlider.js", render_slider)) {
		
	}
	
	if (!m_jscache->getFile("mootools", mootools)) {
		
	}
	
	if (!m_jscache->getFile("slider.js", slider)) {
		
	}
	
	std::string format_files_path = m_punkt_rsrc_url + "sh_slider_240x400/";
	//std::string punkt_url = "http://127.0.0.1:4249/";
	
	std::string showcase_dump;
	rebuildClickLinks(_show, _pid, _adid, args->click_templates, args->partner_ids);
	_show.dumpJson(showcase_dump);
	
	std::string format_renderer_bind = \
	
	"if (document._punkt_codes == null)\n"
	"	document._punkt_codes = {};\n"
	"\n"
	"if (document._punkt_codes_post == null)\n"
	"	document._punkt_codes_post = {};\n"
	"\n"
	"document._punkt_codes[\"" + uint64_to_string(_pid) + "\"] = function () {\n"
	"	var show = JSON.parse(\'" + showcase_dump + "\');\n"
	"	var formatter_args = JSON.parse(\'" + args->json_dump + "\') \n"
	"	return renderShowcaseSlider(" + uint64_to_string(_pid) +  ", show, formatter_args, '" + format_files_path + "');\n"
	"}\n"
	
	"document._punkt_codes_post[\"" + uint64_to_string(_pid) + "\"] = function () { \n"
	"	buildSlider('" + uint64_to_string(_pid) + "', true, '" +m_punkt_url+ "', " +uint64_to_string(_adid)+ " ); \n"
	"} \n";
	
	std::string resp = \		
		"<div id=\"punkt_place_0\" width=\"240\" height=\"400\"></div>"
		"<script type=\"text/JavaScript\">";

	resp.reserve(slider_events.size() + render_slider.size() + mootools.size() + slider.size() + format_renderer_bind.size());
	
	resp += slider_events;
	resp += render_slider;
	resp += mootools;
	resp += slider;
	resp += format_renderer_bind + 
		"document.punkt_rsrc_url = \"" + m_punkt_rsrc_url + "\";\n"
		"var place = document.getElementById(\"punkt_place_0\");\n"
		"place.innerHTML = document._punkt_codes[\"0\"]();\n"
		"document._punkt_codes_post[\"0\"]();\n"
		"</script>";
	
	_conn->sendResponse(resp);
	_conn->close();
}

void ShowcaseSliderFormatter::onGotShowcase(bool _success, ShowcaseInstance &_show, HttpSrv::ConnectionPtr _conn, 
								uint64_t _pid, uint64_t _adid, FormatterArgsPtr _args) {
	
	if (!_success) {
		
		_conn->close();
		return;
	}
	
	ShowcaseSliderFormatterArgs* args = (ShowcaseSliderFormatterArgs*)_args.get();	
	
	std::string slider_events;
	std::string render_slider;
	std::string mootools;
	std::string slider;
	
	std::string format_files_path = m_punkt_rsrc_url + "sh_slider_240x400/";
	
	if (!m_jscache->getFile("ShowcaseSliderEvents.js", slider_events)) {
		std::cout << "ShowcaseSliderFormatter::onCalledGeberOkDemo ShowcaseSliderEvents.js not loaded in cache  \n";
	}
	
	if (!m_jscache->getFile("renderShowcaseSlider.js", render_slider)) {
		
	}
	
	if (!m_jscache->getFile("mootools", mootools)) {
		
	}
	
	if (!m_jscache->getFile("slider.js", slider)) {
		
	}
	
	std::string showcase_dump;
	rebuildClickLinks(_show, _pid, _adid, args->click_templates, args->partner_ids);
	_show.dumpJson(showcase_dump);
	
	std::string format_renderer_bind = \
	
	"if (document._punkt_codes == null)\n"
	"	document._punkt_codes = {};\n"
	"\n"
	"if (document._punkt_codes_post == null)\n"
	"	document._punkt_codes_post = {};\n"
	"\n"
	"document._punkt_codes[\"" + uint64_to_string(_pid) + "\"] = function () {\n"
	"	var show = JSON.parse(\'" + showcase_dump + "\');\n"
	"	var formatter_args = JSON.parse(\'" + args->json_dump + "\') \n"
	"	return renderShowcaseSlider(" + uint64_to_string(_pid) +  ", show, formatter_args, '" + format_files_path + "');\n"
	"}\n"
	
	"document._punkt_codes_post[\"" + uint64_to_string(_pid) + "\"] = function () { \n"
	"	buildSlider('" + uint64_to_string(_pid) + "', false, '" +m_punkt_url+ "', " +uint64_to_string(_adid)+ " ); \n"
	"} \n";
	
	std::string resp;
	resp.reserve(slider_events.size() + render_slider.size() + mootools.size() + slider.size() + format_renderer_bind.size());
	
	resp += slider_events;
	resp += render_slider;
	resp += mootools;
	resp += slider;
	resp += format_renderer_bind;
	
	_conn->sendResponse(resp);
	_conn->close();
}

