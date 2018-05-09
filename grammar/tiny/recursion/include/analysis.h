#ifndef _ANALYSIS_H_
#define _ANALYSIS_H_

#include "scan.h"
#include "global.h"
#include "syntax_treenode.h"
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

namespace dh{

class get_visitor : public ::boost::static_visitor<void>{
	public:
		::std::string operator()(::std::string& s) const
		{
			return s;
		}

		int operator()(int& i) const
		{
			return i;
		}
};

class analysis{
	public:

		analysis( scanner tokens);
		void printTree() const;
		auto getRoot();
	private:

		void printTree( const NodePtr& ptr) const;
		void init();
		bool initFlag;

		int getResult( const NodePtr& root);
		void match( ::std::string c);
		int number();
		::std::string identifier();

		scanner _tokens;
		::std::shared_ptr< token > tmp;
		NodePtr _root;

		// recursion analysis
		NodePtr program();
		NodePtr stmt_sequence();
		NodePtr statement();
		NodePtr if_stmt();
		NodePtr repeat_stmt();
		NodePtr assgin_stmt();
		NodePtr read_stmt();
		NodePtr write_stmt();
		NodePtr exp();
		NodePtr simple_exp();
		NodePtr term();
		NodePtr factor();
};

}

#endif
