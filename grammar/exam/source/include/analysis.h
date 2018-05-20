#ifndef _ANALYSIS_H_
#define _ANALYSIS_H_

#include "scan.h"
#include "global.h"
#include "syntax_treenode.h"
#include <sstream>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

namespace dh{

class get_visitor : public ::boost::static_visitor<::std::string>{
	public:
		::std::string operator()(::std::string& s) const
		{
			return s;
		}

		::std::string operator()(int& num) const
		{
			::std::ostringstream ss;

			ss << num;
			return ss.str();
		}
};

class analysis{
	public:

		analysis( scanner tokens);
		void printTree();
		NodePtr getRoot();
	private:

		void printTree( const NodePtr& ptr);
		void init();
		bool initFlag;

		int getResult( const NodePtr& root);
		void match( ::std::string c);
		::std::string identifier();
		double decimal();

		scanner _tokens;
		::std::shared_ptr< token > tmp;
		NodePtr _root;

		int tabcounter;

		// recursion analysis
		NodePtr ThreadSpec();
		NodePtr featureSpec();
		NodePtr portType();
		NodePtr IOType();
		NodePtr flowSpec();
		NodePtr association();
		NodePtr reference();
};

}

#endif
