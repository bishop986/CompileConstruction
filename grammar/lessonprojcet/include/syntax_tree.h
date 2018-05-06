#ifndef _SYNTAX_TREE_H_
#define _SYNTAX_TREE_H_
#include <string>
#include <memory>
#include <boost/variant.hpp>
#include "global.h"

namespace dh{

class TreeNode{
	public:
		TreeNode(const ::std::string& c);
		TreeNode(const int& val);
		::dh::KIND getKind() const;
		void setLeft( ::std::unique_ptr<TreeNode>& ptr);
		void setRight( ::std::unique_ptr<TreeNode>& ptr);

	private:
		::std::unique_ptr<TreeNode> _left_child;
		::std::unique_ptr<TreeNode> _right_child;

		::boost::variant< ::std::string, int> _data;
		KIND _kind;
	friend class analysis;
};

}

#endif
