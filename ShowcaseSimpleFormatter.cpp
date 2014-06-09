#include "ShowcaseSimpleFormatter.h"

ShowcaseSimpleFormatterArgs::ShowcaseSimpleFormatterArgs(uint64_t _pid, uint64_t _shid, int _nres):
	pid(_pid),
	shid(_shid),
	nres(_nres) {
}

ShowcaseSimpleFormatter::ShowcaseSimpleFormatter(HttpOutRequestDispPtr _req_disp,
						boost::function<HttpSrv::ConnectionPtr(int)> _getConnById,
						GeberdCliApiClientPtr _geber_cli):
	m_req_disp(_req_disp),
	m_getConnById(_getConnById),
	m_geber_cli(_geber_cli) {
							
}

FormatterArgsPtr ShowcaseSimpleFormatter::parseArgs(uint64_t _pid, const std::string &_args_js) {

	uint64_t shid;
	int nres;
	
	json_error_t error;
	json_t *root = json_loads(_args_js.c_str(), 0, &error);
	
	json_t *j_shid = json_object_get(root, "shid");
	if (json_is_string(j_shid)) {
		shid = string_to_uint64(json_string_value(j_shid));
	} else
		throw "ShowcaseSimpleFormatter::parseArgs could not parse shid";
		
	json_t *j_n = json_object_get(root, "n");
	if (json_is_string(j_n)) {
		nres = string_to_uint64(json_string_value(j_n));
	} else
		throw "ShowcaseSimpleFormatter::parseArgs could not parse n";
	
	json_decref(root);
	
	return FormatterArgsPtr(new ShowcaseSimpleFormatterArgs(_pid, shid, nres));
}
						
void ShowcaseSimpleFormatter::format(HttpSrv::ConnectionPtr _conn, HttpSrv::RequestPtr _req, FormatterArgsPtr _args) {
	
	ShowcaseSimpleFormatterArgs* args = (ShowcaseSimpleFormatterArgs*)_args.get();
	
	std::string geberd_call_url;	
	m_geber_cli->buildUrlGetShowcase(args->shid, args->nres, geberd_call_url);
	
	HttpOutRequestDisp::RequesterPtr requester
		(new ShowcaseSimpleRequester (boost::bind(&HttpOutRequestDisp::onCall, m_req_disp.get(), _1, _2, _3),
										boost::bind(&HttpOutRequestDisp::onRequesterFinished, m_req_disp.get(), _1),
										_conn->getSock(),
										args->pid,
										geberd_call_url,
										boost::bind(&ShowcaseSimpleFormatter::onCalledGeberOk, this, _1, _2, _3),
										boost::bind(&ShowcaseSimpleFormatter::onCalledGeberFail, this, _1) ) );
	
	m_req_disp->addRequester(requester);
}

void ShowcaseSimpleFormatter::onCalledGeberOk (int _connid, uint64_t _pid, const std::string &_resp) {
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
		
		std::string resp = \
		\
		"function renderShowcaseSimple(show) {\n"
		"\n"
		"	view = \"<table><tr>\";\n"
		"	for (var i in show.items) {\n"
		"\n"
		"		view = view + \"<td width=100 height=100 valign=top>\";\n"
		"		view = view + \"<a href= \"+ show.items[i].clickurl + \">\";\n"
		"		view = view + \"<img height=100px width=100px src=\" + show.items[i].imgurl + \"></img><br>\";\n"
		"		view = view + show.items[i].caption + \"<br>\";\n"
		"		view = view + \"<b>\"+ show.items[i].price + \"руб </b><br>\";\n"
		"		view = view + \"</a> id: \" + show.items[i].id + \"</td>\";\n"
		"	}\n"
		"	view = view + \"</tr></table>\";\n"
		"	return view;\n"
		"}\n"
		"\n"
		"if (document._punkt_codes == null)\n"
		"	document._punkt_codes = {};\n"
		"\n"	
		"document._punkt_codes[\"" + uint64_to_string(_pid) + "\"] = function () {\n"
		"	var show = JSON.parse(\'" + _resp + "\');\n"
		"	return renderShowcaseSimple(show);\n"
		"}\n";
		
		conn->sendResponse(resp);
		conn->close();
	}
}

void ShowcaseSimpleFormatter::onCalledGeberFail (int _connid) {
	std::cout << "ShowcaseSimpleFormatter::onCalledGeberFail\n";

	HttpSrv::ConnectionPtr conn = m_getConnById(_connid);
	if (conn) {
		conn->sendResponse("{ \"status\" : \"ShowcaseSimpleFormatter::onCalledGeberFail\" }");
		conn->close();
	}
}
