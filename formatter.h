#ifndef _FORMATTER_H_
#define _FORMATTER_H_

#include "hiconfig.h"
#include <boost/shared_ptr.hpp>
#include "hiaux/network/HttpSrv.h"

#include "AdRequest.h"

// specific for place args 
class FormatterArgs {
public:
};

typedef boost::shared_ptr<FormatterArgs> FormatterArgsPtr;

class Formatter {
public:
	virtual FormatterArgsPtr parseArgs(const std::string &_args_js) = 0;
	virtual void format(AdRequestPtr _ad_req, FormatterArgsPtr _args) = 0;
	virtual void formatDemo(AdRequestPtr _ad_req, FormatterArgsPtr _args) = 0;
	
	virtual void handleFormatEvent(HttpSrv::ConnectionPtr _conn, HttpSrv::RequestPtr _req) = 0;
};

typedef boost::shared_ptr<Formatter> FormatterPtr;

#endif
