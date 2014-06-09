#ifndef _FORMATTER_H_
#define _FORMATTER_H_

// specific for place args 
class FormatterArgs {
public:
};

typedef boost::shared_ptr<FormatterArgs> FormatterArgsPtr;

class Formatter {
public:
	virtual FormatterArgsPtr parseArgs(uint64_t pid, const std::string &_args_js) = 0;
	virtual void format(HttpSrv::ConnectionPtr _conn, HttpSrv::RequestPtr _req, FormatterArgsPtr _args) = 0;
};

typedef boost::shared_ptr<Formatter> FormatterPtr;

#endif
