#ifndef _EVENTS_TREE_H_
#define _EVENTS_TREE_H_

#include "hiconfig.h"
#include "hiaux/network/HttpSrv.h"
#include <map>
#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>

#define EVENT_EQUALS true

class EventsTreeNode {

	std::map<std::string, EventsTreeNode*> m_children;
	bool m_handler_set;
	bool m_event_equals;
	boost::function<void(std::map<std::string, std::string> _params, HttpSrv::ConnectionPtr _conn, HttpSrv::RequestPtr _req)> m_handler; 
public:
	
	std::string parent_name;	
	
	EventsTreeNode(boost::function<void(std::map<std::string, std::string> _params, HttpSrv::ConnectionPtr _conn, HttpSrv::RequestPtr _req)> _handler);
	
	EventsTreeNode( const std::string &_arg,
					EventsTreeNode *_child);
	
	EventsTreeNode( const std::string &_arg0,
					EventsTreeNode *_child0,
					const std::string &_arg1,
					EventsTreeNode *_child1);
	
	EventsTreeNode( const std::string &_arg0,
					EventsTreeNode *_child0,
					const std::string &_arg1,
					EventsTreeNode *_child1,
					const std::string &_arg2,
					EventsTreeNode *_child3);
	
	EventsTreeNode( bool _equals,
					const std::string &_arg0,
					EventsTreeNode *_child0,
					const std::string &_arg1,
					EventsTreeNode *_child1);
	
	void doHandleEvent(std::map<std::string, std::string> &_params, HttpSrv::ConnectionPtr _conn, HttpSrv::RequestPtr _req);
	void handleEvent(HttpSrv::ConnectionPtr _conn, HttpSrv::RequestPtr _req);
};

typedef EventsTreeNode ETN;

class EventsTree {
public:
	
};

#endif
