#include <iostream>

#include "include/syntax_treenode.h"

namespace dh{

TreeNode::TreeNode( const ::std::string& c)
{
	this->_data = c;
	this->_sibling = nullptr;
}

TreeNode::TreeNode( const double& val)
{
	this->_data = val;
	this->_sibling = nullptr;
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

int TreeNode::getChildSize() const
{
	return this->_children.size();
}

Combine TreeNode::getData() const
{
	return this->_data;
}

::std::vector< NodePtr > TreeNode::getChildren() const
{
	return this->_children;
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
	if ( _children.size() < 5)
	{
		this->_children.push_back( ptr);
	} else 
	{
		::std::cerr << "[ERROR] Append Child Error\n";
		::std::exit(1);
	}
}

}
