#ifndef _AD_H_
#define _AD_H_

#include "hiconfig.h"
#include "formatter.h"

class AdTargeterArgs {
public:
	virtual ~AdTargeterArgs() { }
};

typedef boost::shared_ptr<AdTargeterArgs> AdTargeterArgsPtr;

class Ad {
public:
	uint64_t id;
	uint64_t format_id;
	uint64_t ownerid;
	std::string formatter_args_str;
	std::string targeter_args_str;

	FormatterArgsPtr args;
	AdTargeterArgsPtr targeter_args;
	
	Ad(uint64_t _id, uint64_t _format_id, uint64_t ownerid, const std::string &_formatter_args_str, const std::string &_targeter_args);
	virtual ~Ad();
};

typedef boost::shared_ptr<Ad> AdPtr;

#endif
