#ifndef _FORMATTER_H_
#define _FORMATTER_H_

#include "hiconfig.h"
#include <boost/shared_ptr.hpp>
#include "hiaux/network/HttpSrv.h"

// specific for place args 
class FormatterArgs {
public:
};

typedef boost::shared_ptr<FormatterArgs> FormatterArgsPtr;

class Formatter {
public:
	virtual FormatterArgsPtr parseArgs(const std::string &_args_js) = 0;
	virtual void format(HttpSrv::ConnectionPtr _conn, HttpSrv::RequestPtr _req, uint64_t _pid, FormatterArgsPtr _args) = 0;
	virtual void formatDemo(HttpSrv::ConnectionPtr _conn, HttpSrv::RequestPtr _req, uint64_t _pid, FormatterArgsPtr _args) = 0;
};

typedef boost::shared_ptr<Formatter> FormatterPtr;

#endif
