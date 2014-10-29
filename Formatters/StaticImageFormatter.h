#ifndef _STATIC_IMAGE_FORMATTER_H_
#define _STATIC_IMAGE_FORMATTER_H_

#include "punkt_consts.h"

#include "formatter.h"
#include "FileCache.h"

#include <jansson.h>

class StaticImageFormatterArgs : public FormatterArgs {
public:
	
	std::string system_url;
	std::string system_rsrc_url;
	std::string json_dump;
	
	std::string click_url;
	std::string img_url;
	
};

class StaticImageFormatter : public Formatter {
	
	FileCachePtr m_jscache;
	
	boost::function<uint64_t(uint64_t)> m_getAdOwner;
	
//	std::string system_url;
//	std::string system_rsrc_url;
	
	void buildClickUrl(AdRequestPtr _ad_req, const std::string &_system_url, const std::string &_direct_url, std::string &_click_url);
public:
	
	StaticImageFormatter(FileCachePtr _jscache,
						boost::function<uint64_t(uint64_t)> _getAdOwner);
	
	virtual FormatterArgsPtr parseArgs(const std::string &_args_js);
	virtual void format(AdRequestPtr _ad_req, FormatterArgsPtr _args, const std::string &_exthtml, const std::string &_extjs);
	virtual void formatDemo(AdRequestPtr _ad_req, FormatterArgsPtr _args);
	
	virtual void handleFormatEvent(HttpConnectionPtr _conn, HttpRequestPtr _req);
	
	virtual void handleTargeterEvent(const std::string &_method,
									uint64_t _pid,
									uint64_t _adid,
									const std::map<std::string, std::string> &_params,
									HttpConnectionPtr _conn,
									HttpRequestPtr _req);
};

typedef boost::shared_ptr<StaticImageFormatter> StaticImageFormatterPtr;

#endif
