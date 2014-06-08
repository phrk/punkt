#include "ShowcaseSimpleFormatter.h"

ShowcaseSimpleFormatter::ShowcaseSimpleFormatter(HttpOutRequestDispPtr _req_disp,
						boost::function<HttpSrv::ConnectionPtr(int)> _getConnById,
						GeberdCliApiClientPtr _geber_cli):
	m_req_disp(_req_disp),
	m_getConnById(_getConnById),
	m_geber_cli(_geber_cli) {
							
}
						
void ShowcaseSimpleFormatter::format(HttpSrv::ConnectionPtr _conn, HttpSrv::RequestPtr _req, const std::string &_args) {
	//std::cout << "ShowcaseSimpleFormatter::format\n";
	
	std::string geberd_call_url;
	
	// TODO parse shid and n
	m_geber_cli->buildUrlGetShowcase(2, 6, geberd_call_url);
	
	HttpOutRequestDisp::RequesterPtr requester
		(new ShowcaseSimpleRequester (boost::bind(&HttpOutRequestDisp::onCall, m_req_disp.get(), _1, _2, _3),
										boost::bind(&HttpOutRequestDisp::onRequesterFinished, m_req_disp.get(), _1),
										_conn->getSock(),
										geberd_call_url,
										boost::bind(&ShowcaseSimpleFormatter::onCalledGeberOk, this, _1, _2),
										boost::bind(&ShowcaseSimpleFormatter::onCalledGeberFail, this, _1) ) );
	
	m_req_disp->addRequester(requester);
}

void ShowcaseSimpleFormatter::onCalledGeberOk (int _connid, const std::string &_resp) {
	//std::cout << "ShowcaseSimpleFormatter::onCalledGeberOk resp: " << _resp << std::endl;
	HttpSrv::ConnectionPtr conn = m_getConnById(_connid);
	if (conn) {
		
		std::string view = "<table><tr>";
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
		
		for (int i = 0; i < show.items.size(); i++) {
			
			view = view + "<td width=100 height=100 valign=top>";
			view = view + "<a href=" + show.items[i].clickurl + ">";
			view = view + "<img height=100px width=100px src=" + show.items[i].imgurl + "     </img><br>";
			view = view + show.items[i].caption + "<br>";
			view = view + "<b>"+ show.items[i].price + "руб </b><br>";
			view = view + "</a> id: " + show.items[i].id + "</td>";
		}
		view = view + "</tr></table>";
		
		conn->sendResponse(view);
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
