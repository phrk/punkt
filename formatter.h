#ifndef _FORMATTER_H_
#define _FORMATTER_H_

#include "hiconfig.h"
#include <boost/shared_ptr.hpp>
#include "hiaux/network/HttpServer/HttpServer.h"

#include "AdRequest.h"

// specific for place args 
class FormatterArgs {
public:
	std::string system_url;
	std::string system_rsrc_url;

	virtual ~FormatterArgs() { }
};

typedef boost::shared_ptr<FormatterArgs> FormatterArgsPtr;

class Formatter {
public:

	
	virtual FormatterArgsPtr parseArgs(const std::string &_args_js) = 0;
	virtual void format(AdRequestPtr _ad_req, FormatterArgsPtr _args, const std::string &_exthtml, const std::string &_extjs) = 0;
	virtual void formatDemo(AdRequestPtr _ad_req, FormatterArgsPtr _args) = 0;
	
	virtual void handleFormatEvent(HttpConnectionPtr _conn, HttpRequestPtr _req) = 0;
	
	virtual void handleTargeterEvent(const std::string &_method,
									uint64_t _pid,
									uint64_t _adid,
									const std::map<std::string, std::string> &_params,
									HttpConnectionPtr _conn,
									HttpRequestPtr _req) = 0;
									
	virtual ~Formatter() { }
};

typedef boost::shared_ptr<Formatter> FormatterPtr;

#endif
