#ifndef _SYNTAX_TREE_H_
#define _SYNTAX_TREE_H_
#include <string>
#include <memory>
#include <boost/variant.hpp>
#include "global.h"

namespace dh{

class TreeNode;


typedef ::std::shared_ptr<TreeNode> NodePtr;

class TreeNode{
	public:
		TreeNode(const ::std::string& c);
		TreeNode(const int& val);

		// from left to right append child tree
		void appendChild( NodePtr& ptr);

		// set sibling
		void setSibling( const ::std::shared_ptr<TreeNode>& ptr);

		// set some extra info
		void setNodeKind( const int& _nk);
		void setKind( const int& _k);
		void setType( const int& _t);

		// get some extra info
		int getNodeKind() const;
		int getKind() const;
		int getType() const;
		int getChildSize() const;

		// get children
		::std::vector< NodePtr > getChildren() const;

		// Type need user check
		::boost::variant< ::std::string, int> getData() const;

		// get next sibling
		NodePtr getSibling() const;

		void setStrVal( const ::std::string str)
		{
			this->strval = str;
		}

		::std::string getStrVal()
		{
			return this->strval;
		}

	private:
		::std::vector< ::std::shared_ptr<TreeNode> > _children;
		NodePtr _sibling;
		::std::string strval;

		int _nodekind;
		int _kind;

		::boost::variant< ::std::string, int> _data;
		int _type;


};


}

#endif
