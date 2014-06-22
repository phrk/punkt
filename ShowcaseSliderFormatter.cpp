#include "ShowcaseSliderFormatter.h"

ShowcaseSliderFormatterArgs::ShowcaseSliderFormatterArgs(uint64_t _shid, int _nres):
	shid(_shid),
	nres(_nres) {
}

ShowcaseSliderFormatter::ShowcaseSliderFormatter(HttpOutRequestDispPtr _req_disp,
						FileCachePtr _jscache,
						boost::function<HttpSrv::ConnectionPtr(int)> _getConnById,
						GeberdCliApiClientPtr _geber_cli):
	m_req_disp(_req_disp),
	m_jscache(_jscache),
	m_getConnById(_getConnById),
	m_geber_cli(_geber_cli) {
							
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
						
void ShowcaseSliderFormatter::format(HttpSrv::ConnectionPtr _conn, HttpSrv::RequestPtr _req, uint64_t _pid, FormatterArgsPtr _args) {
	
	ShowcaseSliderFormatterArgs* args = (ShowcaseSliderFormatterArgs*)_args.get();
	
	std::string geberd_call_url;	
	m_geber_cli->buildUrlGetShowcase(args->shid, args->nres, geberd_call_url);
	
	HttpOutRequestDisp::RequesterPtr requester
		(new ShowcaseSimpleRequester (boost::bind(&HttpOutRequestDisp::onCall, m_req_disp.get(), _1, _2, _3),
										boost::bind(&HttpOutRequestDisp::onRequesterFinished, m_req_disp.get(), _1),
										_conn->getSock(),
										_pid,
										geberd_call_url,
										boost::bind(&ShowcaseSliderFormatter::onCalledGeberOk, this, _1, _2, _3),
										boost::bind(&ShowcaseSliderFormatter::onCalledGeberFail, this, _1) ) );
	
	m_req_disp->addRequester(requester);
}

void ShowcaseSliderFormatter::formatDemo(HttpSrv::ConnectionPtr _conn, HttpSrv::RequestPtr _req, uint64_t _pid, FormatterArgsPtr _args) {
	
	ShowcaseSliderFormatterArgs* args = (ShowcaseSliderFormatterArgs*)_args.get();
	
	std::string geberd_call_url;	
	m_geber_cli->buildUrlGetShowcase(args->shid, args->nres, geberd_call_url);
	
	HttpOutRequestDisp::RequesterPtr requester
		(new ShowcaseSimpleRequester (boost::bind(&HttpOutRequestDisp::onCall, m_req_disp.get(), _1, _2, _3),
										boost::bind(&HttpOutRequestDisp::onRequesterFinished, m_req_disp.get(), _1),
										_conn->getSock(),
										_pid,
										geberd_call_url,
										boost::bind(&ShowcaseSliderFormatter::onCalledGeberOkDemo, this, _1, _2, _3),
										boost::bind(&ShowcaseSliderFormatter::onCalledGeberFail, this, _1) ) );
	
	m_req_disp->addRequester(requester);
}

void ShowcaseSliderFormatter::onCalledGeberOkDemo (int _connid, uint64_t _pid, const std::string &_resp) {
	
	HttpSrv::ConnectionPtr conn = m_getConnById(_connid);
	if (conn) {
		
		GeberdCliApiClient::ShowcaseInst show;
		
		try {
			m_geber_cli->parseResponse(_resp, show);
		}
		catch (...) {
			conn->sendResponse("{ \"status\" : \"internal server error\" }");
			std::cout << "ShowcaseSimpleFormatter::onCalledGeberOk could not parse geber response: " << _resp << std::endl; 
			conn->close();
			return;
		}
		
		std::string render_slider;
		std::string mootools;
		std::string slider;
		
		if (!m_jscache->getFile("renderShowcaseSlider.js", render_slider)) {
			
		}
		
		if (!m_jscache->getFile("mootools", mootools)) {
			
		}
		
		if (!m_jscache->getFile("slider.js", slider)) {
			
		}
		
		std::string format_files_path = "http://localhost:8080/tests/punkt/sh_slider_240x400/";
		
		std::string format_renderer_bind = \
		
		"if (document._punkt_codes == null)\n"
		"	document._punkt_codes = {};\n"
		"\n"
		"if (document._punkt_codes_post == null)\n"
		"	document._punkt_codes_post = {};\n"
		"\n"
		"document._punkt_codes[\"" + uint64_to_string(_pid) + "\"] = function () {\n"
		"	var show = JSON.parse(\'" + _resp + "\');\n"
		"	return renderShowcaseSlider(" + uint64_to_string(_pid) +  ", show, '" + format_files_path + "');\n"
		"}\n"
		
		"document._punkt_codes_post[\"" + uint64_to_string(_pid) + "\"] = function () { \n"
		"	buildSlider('" + uint64_to_string(_pid) + "'); \n"
		"} \n";
		
		std::string resp = \		
			"<div id=\"punkt_place_0\" width=\"240\" height=\"400\"></div>"
			"<script type=\"text/JavaScript\">";
	
		resp.reserve(render_slider.size() + mootools.size() + slider.size() + format_renderer_bind.size());
		
		resp += render_slider;
		resp += mootools;
		resp += slider;
		resp += format_renderer_bind + 
			"document.getElementById(\"punkt_place_0\").innerHTML = document._punkt_codes[\"0\"]();\n"
			"document._punkt_codes_post[\"0\"]();\n"
			"</script>";
		
		conn->sendResponse(resp);
		conn->close();
	}
}

void ShowcaseSliderFormatter::onCalledGeberOk (int _connid, uint64_t _pid, const std::string &_resp) {
	//std::cout << "ShowcaseSimpleFormatter::onCalledGeberOk resp: " << _resp << std::endl;
	HttpSrv::ConnectionPtr conn = m_getConnById(_connid);
	if (conn) {
		
		GeberdCliApiClient::ShowcaseInst show;
		
		try {
			m_geber_cli->parseResponse(_resp, show);
		}
		catch (...) {
			conn->sendResponse("{ \"status\" : \"internal server error\" }");
			std::cout << "ShowcaseSimpleFormatter::onCalledGeberOk could not parse geber response: " << _resp << std::endl; 
			conn->close();
			return;
		}
		
		std::string render_slider;
		std::string mootools;
		std::string slider;
		
		std::string format_files_path = "http://localhost:8080/tests/punkt/sh_slider_240x400/";
		
		if (!m_jscache->getFile("renderShowcaseSlider.js", render_slider)) {
			
		}
		
		if (!m_jscache->getFile("mootools", mootools)) {
			
		}
		
		if (!m_jscache->getFile("slider.js", slider)) {
			
		}
		
		std::string format_renderer_bind = \
		
		"if (document._punkt_codes == null)\n"
		"	document._punkt_codes = {};\n"
		"\n"
		"if (document._punkt_codes_post == null)\n"
		"	document._punkt_codes_post = {};\n"
		"\n"
		"document._punkt_codes[\"" + uint64_to_string(_pid) + "\"] = function () {\n"
		"	var show = JSON.parse(\'" + _resp + "\');\n"
		"	return renderShowcaseSlider(" + uint64_to_string(_pid) +  ", show, '" +format_files_path+ "');\n"
		"}\n"
		
		"document._punkt_codes_post[\"" + uint64_to_string(_pid) + "\"] = function () { \n"
		"	buildSlider('" + uint64_to_string(_pid) + "'); \n"
		"} \n";
		
		std::string resp;
		resp.reserve(render_slider.size() + mootools.size() + slider.size() + format_renderer_bind.size());
		
		resp += render_slider;
		resp += mootools;
		resp += slider;
		resp += format_renderer_bind;
		
		conn->sendResponse(resp);
		conn->close();
	}
}

void ShowcaseSliderFormatter::onCalledGeberFail (int _connid) {
	std::cout << "ShowcaseSimpleFormatter::onCalledGeberFail\n";

	HttpSrv::ConnectionPtr conn = m_getConnById(_connid);
	if (conn) {
		conn->sendResponse("{ \"status\" : \"ShowcaseSimpleFormatter::onCalledGeberFail\" }");
		conn->close();
	}
}
