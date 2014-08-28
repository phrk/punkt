#ifndef _AD_H_
#define _AD_H_

#include "hiconfig.h"
#include "formatter.h"

class Ad {
public:
	uint64_t id;
	uint64_t format_id;
	uint64_t ownerid;
	std::string formatter_args_str;
	FormatterArgsPtr args;
	
	Ad(uint64_t _id, uint64_t _format_id, uint64_t ownerid, const std::string &_formatter_args_str);
	virtual ~Ad();
};

typedef boost::shared_ptr<Ad> AdPtr;

#endif
