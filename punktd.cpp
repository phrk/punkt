#include "punktd.h"

void onFinished() {

}

void Punktd::fallDown(std::string _s) {
	std::cout << _s << std::endl;
	exit(0);
}

hiaux::hashtable<std::string,std::string> Punktd::parseConfig(const std::string &_config_file) {
	hiaux::hashtable<std::string,std::string> ret;
	
	std::vector<std::string> required_params;
	required_params.push_back("listen_port");
	
	try {
		ret = LoadConf::load(_config_file, required_params);
	}
	catch (const std::string s) {
		fallDown(s);
	} catch (const char *s) {
		fallDown(s);
	} catch (...) {
		fallDown("Geberd::parseConfig: Could not parse config file: " + _config_file);
	}
		
	return ret;
}

void get_string_copy(cl_object _str, std::string &_to) {
	std::cout << "get_string_copy: \n";
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

cl_object onPing(cl_narg narg, ...) {
	// va_arg va_end va_start
	std::cout << "onPing called nargs: " << narg << std::endl;
	
	va_list ap;
	va_start(ap, narg);
	for (int i = 0; i<narg; i++) {
		std::string str;
		cl_lispunion *arg = va_arg(ap, cl_object);
		get_string_copy(arg, str);
		std::cout << "param: " << str << std::endl;
	}
	va_end(ap);
}

Punktd::Punktd(const std::string &_config_file) {
	
	hiaux::hashtable<std::string,std::string> _config = parseConfig(_config_file);
	
	m_pool.reset(new hThreadPool(PUNKTD_NTHREADS));
	m_srv_tasklauncher.reset(new TaskLauncher(m_pool, PUNKTD_NTHREADS, boost::bind(&onFinished)));
	
	m_srv.reset(new HttpSrv(m_srv_tasklauncher,
							HttpSrv::ResponseInfo("text/html; charset=utf-8", "punktd"),
							boost::bind(&Punktd::connHandler, this, _1, _2)));

	m_srv->start(strtoint(_config["listen_port"]));
	m_pool->run();
	
	
	char a[1][1];
	cl_boot(0, (char**)a);

	const char *lisp_code = \
	"(defun ping (str)"
	"	(format t \"PING CALLED ~A\" str)"
	"	(setf s (format nil \"Xping:~A\" str))"
	"	(format t \"PARAM:~A~%\" s)"
	"	(on-ping s))";
	
	//cl_object (*l_on_ping)(cl_narg, ...) = (cl_object (*)(cl_narg, ...)) &onPing;
	
	cl_def_c_function_va(c_string_to_object("on-ping"), &onPing);
	
	si_safe_eval(3, c_string_to_object(lisp_code), Cnil, OBJNULL);
	
	cl_object l_ret = si_safe_eval(3, c_string_to_object("(ping \"123\")"), Cnil, Cnil);
		//cl_funcall(2, c_string_to_object("ping"), c_string_to_object("SIeG"));
	
	//cl_pprint(1, l_ret);
	std::string ret;
	//get_string_copy(l_ret, ret);
	
	std::cout << "ret:" << ret << std::endl;// get_string_copy(l_ret, ret) << std::endl;
	
	//cl_pprint(1,obj);
	cl_shutdown();
}

void Punktd::connHandler(HttpSrv::ConnectionPtr _conn, HttpSrv::RequestPtr _req) {
	
	std::string pid;
	if (_req->getField("pid", pid)) {
		_conn->sendResponse("pid set");
	} else
		_conn->sendResponse("pid not set!");
	
	_conn->close();
}

void Punktd::join() {
	m_pool->join();
}
