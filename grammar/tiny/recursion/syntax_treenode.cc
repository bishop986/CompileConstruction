#include <iostream>

#include "include/syntax_treenode.h"

namespace dh{

TreeNode::TreeNode( const ::std::string& c)
{
	this->_data = c;
}

TreeNode::TreeNode( const int& val)
{
	this->_data = val;
}

int TreeNode::getNodeKind() const
{
	return this->_nodekind;
}

int TreeNode::getKind() const
{
	return this->_kind;
}

int TreeNode::getType() const
{
	return this->_type;
}

::boost::variant< ::std::string, int> TreeNode::getData() const
{
	return this->_data;
}

void TreeNode::setNodeKind( const int& _nk)
{
	this->_nodekind = _nk;
}

void TreeNode::setKind( const int& _k)
{
	this->_kind = _k;
}

void TreeNode::setType( const int& _t)
{
	this->_type = _t;
}

void TreeNode::setSibling( const ::std::shared_ptr<TreeNode>& ptr)
{
	this->_sibling = ptr;
}

NodePtr TreeNode::getSibling() const
{
	return this->_sibling;
}

void TreeNode::appendChild( NodePtr& ptr)
{
	if ( _children.size() < 3)
	{
		this->_children.push_back( ptr);
	} else 
	{
		::std::cerr << "[ERROR] Append Child Error\n";
		::std::exit(1);
	}
}

}
