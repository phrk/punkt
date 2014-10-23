#include "EventsTree.h"

EventsTreeNode::EventsTreeNode(boost::function<void(std::map<std::string, std::string> _params,
													HttpConnectionPtr _conn, HttpRequestPtr _req)> _handler):
	m_handler(_handler),
	m_handler_set(true),
	m_event_equals(false) {
									
}

EventsTreeNode::EventsTreeNode(const std::string &_arg,
								EventsTreeNode *_child):
								m_handler_set(false),
								m_event_equals(false) {
	m_children[_arg] = _child;
	_child->parent_name = _arg;
}

EventsTreeNode::EventsTreeNode( const std::string &_arg0,
				EventsTreeNode *_child0,
				const std::string &_arg1,
				EventsTreeNode *_child1):
	m_handler_set(false),
	m_event_equals(false) {
					
	m_children[_arg0] = _child0;
	m_children[_arg1] = _child1;
	
	_child0->parent_name = _arg0;
	_child1->parent_name = _arg1;
}

EventsTreeNode::EventsTreeNode( const std::string &_arg0,
				EventsTreeNode *_child0,
				const std::string &_arg1,
				EventsTreeNode *_child1,
				const std::string &_arg2,
				EventsTreeNode *_child2):
	m_handler_set(false),
	m_event_equals(false) {
	
	m_children[_arg0] = _child0;
	m_children[_arg1] = _child1;
	m_children[_arg2] = _child2;
	
	_child0->parent_name = _arg0;
	_child1->parent_name = _arg1;
	_child2->parent_name = _arg2;
}

EventsTreeNode::EventsTreeNode( bool _equals,
				const std::string &_arg0,
				EventsTreeNode *_child0,
				const std::string &_arg1,
				EventsTreeNode *_child1):
	m_event_equals(true),
	m_handler_set(false) {

 	m_children[_arg0] = _child0;
 	m_children[_arg1] = _child1;
	
	_child0->parent_name = _arg0;
	_child1->parent_name = _arg1;
}

void EventsTreeNode::doHandleEvent(std::map<std::string, std::string> &_params, HttpConnectionPtr _conn, HttpRequestPtr _req) {
	
	if (m_handler_set) {
	
		m_handler(_params, _conn, _req);

	} else {
		
	}

	std::string parent_value;
	
	if (m_event_equals) {
	
		if (!_req->getField(parent_name, parent_value)) {
		
			return;
		} 
		else {
		
		}
	}

	std::string bf;
	std::map<std::string, EventsTreeNode*>::iterator it = m_children.begin();
	std::map<std::string, EventsTreeNode*>::iterator end = m_children.end();
	
	while (it != end) {
		
		if (m_event_equals) {			
			
			if (it->first == parent_value) {
			
				it->second->doHandleEvent(_params, _conn, _req);
				return;
			}
			
		} else {
		
			if (_req->getField(it->first, bf)) {
				
				_params[ it->first ] = bf;
				it->second->doHandleEvent(_params, _conn, _req);
				return;
			}
		}
		
		it++;
	}
}

void EventsTreeNode::handleEvent(HttpConnectionPtr _conn, HttpRequestPtr _req) {
	
	std::map<std::string, std::string> params;
	this->doHandleEvent(params, _conn, _req);
}
