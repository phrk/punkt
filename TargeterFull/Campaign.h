#ifndef _CAMPAIGN_H_
#define _CAMPAIGN_H_

#include "hiconfig.h"
#include <boost/shared_ptr.hpp>

#include "ad.h"

class Campaign {
	
	void parseArgs(const std::string &_args);
public:
	
	uint64_t cid;
	//hiaux::hashtable<uint64_t, AdPtr> m_ads;
	std::vector<uint64_t> ads;
	int mode;
	
	Campaign(uint64_t _cid, const std::string &_args, const std::vector<uint64_t> &_ads);
};

typedef boost::shared_ptr<Campaign> CampaignPtr;

#endif
