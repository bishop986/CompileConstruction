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

		// from left to right append child tree
		void appendChild( ::std::unique_ptr<TreeNode>& ptr);

		// set sibling
		void setSibling( ::std::unique_ptr<TreeNode>& ptr);

		// set some extra info
		void setNodeKind( const int& _nk);
		void setKind( const int& _k);
		void setType( const int& _t);

		// get some extra info
		auto getNodeKind() const;
		auto getKind() const;
		auto getType() const;

		// Type need user check
		auto getData() const;

	private:
		::std::vector< ::std::unique_ptr<TreeNode> > _children;
		::std::unique_ptr<TreeNode> _sibling;

		int _nodekind;
		int _kind;

		::boost::variant< ::std::string, int> _data;
		int _type;

	friend class analysis;
};

}

#endif
