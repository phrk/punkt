#include "StaticImageFormatter.h"

StaticImageFormatter::StaticImageFormatter(FileCachePtr _jscache,
					const std::string &_punkt_url,
					const std::string &_punkt_rsrc_url,
					boost::function<uint64_t(uint64_t)> _getAdOwner):
	m_jscache(_jscache),
	system_url(_punkt_url),
	system_rsrc_url(_punkt_rsrc_url),
	m_getAdOwner(_getAdOwner)
{	
}

FormatterArgsPtr StaticImageFormatter::parseArgs(const std::string &_args_js) {
	
	std::cout << "StaticImageFormatter::parseArgs: " << _args_js << std::endl;
	StaticImageFormatterArgs *a = new StaticImageFormatterArgs();
	a->json_dump = _args_js;

//	a->click_url = "http://advaction.ru/tds?cid=651&pid=a4fb9ee9afa2859342ff8f3fd64d69f0";
//	a->img_url = "http://advaction.ru/creo/5f7/5f79205eb91c7e9b97612415ba09e0ad.jpg";
	
	json_error_t error;
	json_t *root = json_loads(_args_js.c_str(), 0, &error);
	
	json_t *j_click_url = json_object_get(root, "click_url");
	if (json_is_string(j_click_url)) {
		a->click_url = json_string_value(j_click_url);
	} else {
		std::cout << "StaticImageFormatter::parseArgs could not parse click_url\n";
		throw "StaticImageFormatter::parseArgs could not parse click_url";
	}
	
	json_t *j_img_url = json_object_get(root, "img_url");
	if (json_is_string(j_img_url)) {
		a->img_url = std::string(json_string_value(j_img_url));
	} else {
		std::cout << "StaticImageFormatter::parseArgs could not parse type\n";
		throw "StaticImageFormatter::parseArgs could not parse type";
	}
	
	json_decref(root);
	
	a->system_url = system_url;
	return FormatterArgsPtr(a);
}

void StaticImageFormatter::buildClickUrl(AdRequestPtr _ad_req, const std::string &_system_url, const std::string &_direct_url, std::string &_click_url) {

	std::string direct_url(_direct_url);
	_click_url = _system_url + "?evtype=tev&fid=4&ev=click&pid=" + uint64_to_string(_ad_req->pid )
			+ "&adid=" + uint64_to_string( _ad_req->adid ) + "&aim=" + escapeUrl(direct_url);
	
}

void StaticImageFormatter::formatDemo(AdRequestPtr _ad_req, FormatterArgsPtr _args) {
	
	StaticImageFormatterArgs* args = (StaticImageFormatterArgs*)_args.get();
	
	std::string static_image_js;
	
	if (!m_jscache->getFile("static_image.js", static_image_js)) {
		
	}
	
	std::string format_files_path = args->system_rsrc_url + "static_image/";
	
	std::string click_url;
	
	buildClickUrl(_ad_req, args->system_url, args->click_url, click_url);
	
	std::string format_renderer_bind = \
	
	"if (document._punkt_codes == null)\n"
	"	document._punkt_codes = {};\n"
	"\n"
	"if (document._punkt_codes_post == null)\n"
	"	document._punkt_codes_post = {};\n"
	"\n"
	"document._punkt_codes[\"" + uint64_to_string(_ad_req->pid) + "\"] = function () {\n"
	"	return renderStaticImage('" + click_url + "' ," + "'" + args->img_url + "'," + booltostr(_ad_req->https) + ");\n"
	"}\n"
	
	"document._punkt_codes_post[\"" + uint64_to_string(_ad_req->pid) + "\"] = function () { \n"
	"	staticImagePost('" + uint64_to_string(_ad_req->pid) + "', true, '" + args->system_url + "', " +uint64_to_string(_ad_req->adid) + "); \n"
	"} \n";
	
	std::string resp = "<div id=\"punkt_place_0\" width=\"240\" height=\"400\"></div>"
		"<script type=\"text/JavaScript\">";
	resp += static_image_js;
	resp += format_renderer_bind + 
		"document.punkt_rsrc_url = \"" + args->system_rsrc_url + "\";\n"
		"var place = document.getElementById(\"punkt_place_0\");\n"
		"place.innerHTML = document._punkt_codes[\"0\"]();\n"
		"document._punkt_codes_post[\"0\"]();\n"
		"</script>";
	
	_ad_req->conn->sendResponse(resp);
}

void StaticImageFormatter::format(AdRequestPtr _ad_req, FormatterArgsPtr _args, const std::string &_exthtml, const std::string &_extjs) {
	
	StaticImageFormatterArgs* args = (StaticImageFormatterArgs*)_args.get();
	
	std::string static_image_js;
	
	if (!m_jscache->getFile("static_image.js", static_image_js)) {
		
	}
	
	std::string format_files_path = args->system_rsrc_url + "static_image/";
	
	std::string click_url;
	
	buildClickUrl(_ad_req, args->system_url, args->click_url, click_url);
	
	std::string format_renderer_bind = \
	
	"if (document._punkt_codes == null)\n"
	"	document._punkt_codes = {};\n"
	"\n"
	"if (document._punkt_codes_post == null)\n"
	"	document._punkt_codes_post = {};\n"
	"\n"
	"document._punkt_codes[\"" + uint64_to_string(_ad_req->pid) + "\"] = function () {\n"
	"	return renderStaticImage('" + click_url + "' ," + "'" + args->img_url + "'," + booltostr(_ad_req->https) + ") +\n"
	"\"" + _exthtml + "\";\n"
	"}\n"
	
	"document._punkt_codes_post[\"" + uint64_to_string(_ad_req->pid) + "\"] = function () { \n"
	"	staticImagePost('" + uint64_to_string(_ad_req->pid) + "', false, '" + args->system_url + "', " +uint64_to_string(_ad_req->adid) + "); \n"
	"	"+ _extjs + "\n"
	"} \n";
	
	std::string resp;
	
	resp += static_image_js;
	resp += format_renderer_bind;
	
	_ad_req->conn->sendResponse(resp);
}

void StaticImageFormatter::handleFormatEvent(HttpConnectionPtr _conn, HttpRequestPtr _req) {
	
}

void StaticImageFormatter::handleTargeterEvent(const std::string &_method,
								uint64_t _pid,
								uint64_t _adid,
								const std::map<std::string, std::string> &_params,
								HttpConnectionPtr _conn,
								HttpRequestPtr _req) {
									
}
