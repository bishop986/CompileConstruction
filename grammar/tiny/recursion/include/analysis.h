#ifndef _ANALYSIS_H_
#define _ANALYSIS_H_

#include "scan.h"
#include "global.h"
#include "syntax_tree.h"
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

namespace dh{

class analysis{
	public:

		analysis( scanner tokens);
		int getResult();
		::std::unique_ptr< ::dh::TreeNode > getRoot();
	private:

		int getResult( const ::std::unique_ptr<TreeNode>& root);
		void match( ::std::string c);

		scanner tokens;
		int result;
		::boost::shared_ptr< token > tmp;
		::std::unique_ptr<TreeNode> _root;

};

}

#endif
