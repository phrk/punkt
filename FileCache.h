#ifndef _FILE_CACHE_H_
#define _FILE_CACHE_H_

#include "hiconfig.h"
#include <boost/shared_ptr.hpp>
#include "hiaux/threads/locks.h"
#include "hiaux/strings/string_utils.h"

class FileCache {
	hiaux::hashtable<std::string, std::string> m_cache;
	hAutoLock lock;
public:
	
	void addFile(const std::string &_name, const std::string &_path);
	bool getFile(const std::string &_name, std::string &_content);
	void apply(const std::string &_name, boost::function<std::string(const std::string &)> _f);
};

typedef boost::shared_ptr<FileCache> FileCachePtr;

#endif
