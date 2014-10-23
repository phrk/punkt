#ifndef _EVENTS_TREE_H_
#define _EVENTS_TREE_H_

#include "hiconfig.h"
#include "hiaux/network/HttpServer/HttpServer.h"
#include <map>
#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>

#define EVENT_EQUALS true

class EventsTreeNode {

	std::map<std::string, EventsTreeNode*> m_children;
	bool m_handler_set;
	bool m_event_equals;
	bool m_is_stub;
	boost::function<void(std::map<std::string, std::string> _params, HttpConnectionPtr _conn, HttpRequestPtr _req)> m_handler; 

public:
	
	std::string parent_name;
	
	EventsTreeNode(); // stub
	
	EventsTreeNode(boost::function<void(std::map<std::string, std::string> _params, HttpConnectionPtr _conn, HttpRequestPtr _req)> _handler);
	
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
	
	EventsTreeNode( bool _equals,
					const std::string &_arg0,
					EventsTreeNode *_child0,
					const std::string &_arg1,
					EventsTreeNode *_child1,
					const std::string &_arg2,
					EventsTreeNode *_child2);
	
	void doHandleEvent(std::map<std::string, std::string> &_params, HttpConnectionPtr _conn, HttpRequestPtr _req);
	void handleEvent(HttpConnectionPtr _conn, HttpRequestPtr _req);
};

typedef EventsTreeNode ETN;

class EventsTree {
public:
	
};

#endif
