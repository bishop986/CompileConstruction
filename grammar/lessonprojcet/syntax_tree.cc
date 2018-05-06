#include <memory>
#include "include/syntax_tree.h"
#include "include/global.h"
#include <iostream>

namespace dh{

TreeNode::TreeNode( const ::std::string& c)
{
	this->_data = c;
	this->_kind = KIND::OpK;
}

TreeNode::TreeNode( const int& val)
{
	this->_data = val;
	this->_kind = KIND::NumK;
}

KIND TreeNode::getKind() const
{
	return this->_kind;
}

void TreeNode::setLeft( ::std::unique_ptr<TreeNode>& ptr)
{
	this->_left_child = ::std::move(ptr);
}

void TreeNode::setRight( ::std::unique_ptr<TreeNode>& ptr)
{
	this->_right_child = ::std::move(ptr);
}



}
