#include "FileCache.h"

void FileCache::addFile(const std::string &_name, const std::string &_path) {
	
	hLockTicketPtr ticket = lock.lock();
	std::string content;
	getFileContents(_path, content);
	m_cache[_name] = content;
}

bool FileCache::getFile(const std::string &_name, std::string &_content) {
	
	hLockTicketPtr ticket = lock.lock();
	hiaux::hashtable<std::string, std::string>::iterator it = m_cache.find(_name);
	if (it == m_cache.end())
		return false;
	
	_content = it->second;
	return true;
}
