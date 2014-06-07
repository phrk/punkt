#include "punkt.h"

Punkt* Punkt::single;

void get_string_copy(cl_object _str, std::string &_to) {
	
	char *lisp_str = (char*)(_str)->string.self;

	int cap = 1;
	for (int i = 0; lisp_str[i] != 0; i+=4)
		cap++;
	
	char bf[cap];
	
	int k = 0;
	for (int i = 0; lisp_str[i] != 0; i+=4) {
		bf[k] = lisp_str[i];
		k++;
	}
	bf[k] = '\0';
	_to = std::string(bf);
}

cl_object g_onLispFormatterFinished(cl_narg narg, ...) {
	
	if (narg < 2) {
		std::cout << "g_onLispFormatterFinished narg < 2\n";
		return;
	}
	
	va_list ap;
	va_start(ap, narg);
	
	std::string str_connid;
	std::string resp;
	
	cl_lispunion *arg = va_arg(ap, cl_object);
	get_string_copy(arg, str_connid);
	
	arg = va_arg(ap, cl_object);
	get_string_copy(arg, resp);
	
	va_end(ap);
	
	Punkt::single->onLispFormatterFinished(strtoint(str_connid), resp);
}

Punkt::Punkt(boost::function<HttpSrv::ConnectionPtr(int)> _getConnById):
	m_getConnById(_getConnById) {

	char a[1][1];
	cl_boot(0, (char**)a);
	cl_def_c_function_va(c_string_to_object("on-formatter-finished"), &g_onLispFormatterFinished);
	
	std::string lisp_code = \
	"(defun on-formatter (connid req)"
	"	(format t \"on-formatter not set!~%\")"
	"	(on-formatter-finished connid \"RESPONSE\" ))";
	
	si_safe_eval(3, c_string_to_object(lisp_code.c_str()), Cnil, OBJNULL);
	
	std::string _lisp_code = "(on-formatter 4 \"aihoiah\")";
	si_safe_eval(3, c_string_to_object(_lisp_code.c_str()), Cnil, OBJNULL);
}

// connid, response 
void Punkt::onLispFormatterFinished(int _connid, const std::string &_resp) {
	std::cout << "Punkt::onLispFormatterFinished\n";
	
	HttpSrv::ConnectionPtr conn = m_getConnById(_connid);
	if (conn) {
		conn->sendResponse(_resp);
		conn->close();
	}
}

void Punkt::onLispSourceUpdate(const std::string &_lisp_code) {
	si_safe_eval(3, c_string_to_object(_lisp_code.c_str()), Cnil, OBJNULL);
}

void Punkt::connHandler(HttpSrv::ConnectionPtr _conn, HttpSrv::RequestPtr _req) {
	
	std::string _lisp_code = "(on-formatter " + inttostr(_conn->getSock());
	_lisp_code += " \"" + _req->toJson() + "\")";
	
	std::cout << "connHandler lispcode: " + _lisp_code << std::endl;
	
	_lisp_code = "(on-formatter 4 \"aihoiah\")";
	si_safe_eval(3, c_string_to_object(_lisp_code.c_str()), Cnil, OBJNULL);
}
