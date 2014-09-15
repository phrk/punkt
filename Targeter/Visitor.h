#ifndef _VISITOR_H_
#define _VISITOR_H_

#include "hiconfig.h"
#include <string>
#include <set>
#include <vector>
#include <boost/shared_ptr.hpp>

class Visitor {
protected:
	std::string m_vid;
	std::set<std::string> m_unique_props;
	std::vector<std::string> m_new_unique_props;
public:
	
	Visitor(const std::string &_vid);
	void getId(std::string &_vid);
	void addUniqueProp(const std::string &_prop);
	
	virtual void addQuery(const std::string &_q) = 0;
	virtual void getQueries(std::vector<std::string> &_q) = 0;
	virtual void save() = 0;
	virtual ~Visitor();
};

typedef boost::shared_ptr<Visitor> VisitorPtr;

#endif
