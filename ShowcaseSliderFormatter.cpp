#include "ShowcaseSliderFormatter.h"

ShowcaseSliderFormatterArgs::ShowcaseSliderFormatterArgs(uint64_t _shid, int _nres):
	shid(_shid),
	nres(_nres) {
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

	uint64_t shid;
	int nitems;
	
	json_error_t error;
	json_t *root = json_loads(_args_js.c_str(), 0, &error);
	
	json_t *j_shid = json_object_get(root, "shid");
	if (json_is_integer(j_shid)) {
		shid = json_integer_value(j_shid);
	} else {
		std::cout << "ShowcaseSimpleFormatter::parseArgs could not parse shid\n";
		throw "ShowcaseSimpleFormatter::parseArgs could not parse shid";
	}
	json_t *j_n = json_object_get(root, "n");
	if (json_is_integer(j_n)) {
		nitems = json_integer_value(j_n);
	} else {
		std::cout << "ShowcaseSimpleFormatter::parseArgs could not parse n\n";
		throw "ShowcaseSimpleFormatter::parseArgs could not parse n";
	}
	json_decref(root);
	
	return FormatterArgsPtr(new ShowcaseSliderFormatterArgs(shid, nitems));
}

void ShowcaseSliderFormatter::format(HttpSrv::ConnectionPtr _conn, HttpSrv::RequestPtr _req, uint64_t _pid, uint64_t _adid, FormatterArgsPtr _args) {
	
	ShowcaseSliderFormatterArgs* args = (ShowcaseSliderFormatterArgs*)_args.get();
	m_geber_acli->getShowcase(args->shid, args->nres, boost::bind(&ShowcaseSliderFormatter::onGotShowcase, this, _1, _2, _conn, _pid, _adid));
}

void ShowcaseSliderFormatter::formatDemo(HttpSrv::ConnectionPtr _conn, HttpSrv::RequestPtr _req, uint64_t _pid, uint64_t _adid, FormatterArgsPtr _args) {
	
	ShowcaseSliderFormatterArgs* args = (ShowcaseSliderFormatterArgs*)_args.get();
	m_geber_acli->getShowcase(args->shid, args->nres, boost::bind(&ShowcaseSliderFormatter::onGotShowcaseDemo, this, _1, _2, _conn, _pid, _adid));
}

void ShowcaseSliderFormatter::rebuildClickLinks(ShowcaseInstance &_show, uint64_t _pid, uint64_t _adid) {
		
	for (int i = 0; i<_show.items.size(); i++) {
		std::string aim = _show.items[i].clickurl;
		escapeUrl(aim);
		_show.items[i].clickurl = m_punkt_url + "?evtype=fev&fid=1&ev=click&pid=" + 
			uint64_to_string(_pid) + "&adid=" + uint64_to_string(_adid) + "&item=" + uint64_to_string(_show.items[i].id) + "&aim=" + aim;
	}
}

void ShowcaseSliderFormatter::onGotShowcaseDemo (bool _success, ShowcaseInstance &_show, HttpSrv::ConnectionPtr _conn, uint64_t _pid, uint64_t _adid) {
	
	if (!_success) {
		
		_conn->close();
		return;
	}
				
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
	rebuildClickLinks(_show, _pid, _adid);
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
	"	return renderShowcaseSlider(" + uint64_to_string(_pid) +  ", show, '" + format_files_path + "');\n"
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
		"document.getElementById(\"punkt_place_0\").innerHTML = document._punkt_codes[\"0\"]();\n"
		"document._punkt_codes_post[\"0\"]();\n"
		"</script>";
	
	_conn->sendResponse(resp);
	_conn->close();
}

void ShowcaseSliderFormatter::onGotShowcase(bool _success, ShowcaseInstance &_show, HttpSrv::ConnectionPtr _conn, uint64_t _pid, uint64_t _adid) {
	
	if (!_success) {
		
		_conn->close();
		return;
	}
		
	std::string slider_events;
	std::string render_slider;
	std::string mootools;
	std::string slider;
	
	std::string format_files_path = m_punkt_rsrc_url + "sh_slider_240x400/";
	//std::string punkt_url = "http://127.0.0.1:4249/";
	
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
	rebuildClickLinks(_show, _pid, _adid);
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
	"	return renderShowcaseSlider(" + uint64_to_string(_pid) +  ", show, '" +format_files_path+ "');\n"
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


