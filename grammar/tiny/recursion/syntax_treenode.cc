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

auto TreeNode::getNodeKind() const
{
	return this->_nodekind;
}

auto TreeNode::getKind() const
{
	return this->_kind;
}

auto TreeNode::getType() const
{
	return this->_type;
}

auto TreeNode::getData() const
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

void TreeNode::setSibling( ::std::unique_ptr<TreeNode>& ptr)
{
	this->_sibling = ::std::move(ptr);
}

void TreeNode::appendChild( ::std::unique_ptr<TreeNode>& ptr)
{
	if ( _children.size() < 3)
	{
		this->_children.push_back( ::std::move(ptr));
	} else 
	{
		::std::cerr << "[ERROR] Append Child Error\n";
		::std::exit(1);
	}
}

}
