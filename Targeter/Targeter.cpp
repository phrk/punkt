#include "Targeter.h"

Targeter::Targeter(const std::string &_repl_id, boost::function<FormatterArgsPtr(uint64_t _format_id, const std::string &_args)> _parseFormatterArgs):
	m_repl_id(_repl_id),
	m_parseFormatterArgs(_parseFormatterArgs) {
}

Targeter::~Targeter() {
	
}
