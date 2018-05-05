#ifndef _ANALYSIS_H_
#define _ANALYSIS_H_

#include "scan.h"
#include "global.h"
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

namespace dh{

class analysis{
	public:

		analysis( scanner tokens);
		int getResult();
	private:

		void match( ::std::string c);
		int exp();
		int term();
		int factor();
		int number();

		scanner tokens;
		int result;
		::boost::shared_ptr< token > tmp;
};

}

#endif
