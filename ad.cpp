#include "hiconfig.h"
#include "ad.h"

Ad::Ad(uint64_t _id, uint64_t _format_id, const std::string &_formatter_args_str):
	id(_id),
	format_id(_format_id),
	formatter_args_str(_formatter_args_str) {
	
}
