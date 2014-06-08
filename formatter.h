#ifndef _FORMATTER_H_
#define _FORMATTER_H_

class Formatter {
public:
	virtual void format(HttpSrv::ConnectionPtr _conn, HttpSrv::RequestPtr _req, const std::string &_args) = 0;
};

typedef boost::shared_ptr<Formatter> FormatterPtr;

#endif
