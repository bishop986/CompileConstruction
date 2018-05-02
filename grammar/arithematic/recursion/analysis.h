#ifndef _ANALYSIS_H_
#define _ANALYSIS_H_

#include "scan.h"
#include "global.h"

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
		void addop();
		void mulop();
		int number();

		scanner tokens;
		int result;
};

}

#endif
