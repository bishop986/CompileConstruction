#ifndef _SCAN_H_
#define _SCAN_H_

#include <iostream>
#include <cstdio>
#include <string>
#include <vector>
#include "global.h"

namespace dh{

class token{
	public:

		token( ::std::string val, TYPE type);
		TYPE getType();
		::std::string getVal();
		token& operator=(const token& eq);
	private:

		::std::string _val;
		TYPE _type;
};

class scanner{
	public:

		scanner();
		bool scan( ::std::FILE *fp);
		token getToken();
		bool ungetToken();
		void reset() { destroy(); }
		~scanner() { destroy(); }
		scanner& operator=(const scanner& eq);

	private:

		bool scanflag;
		::std::FILE *_fp;
		::std::vector< token > _tokens;
		::std::vector< token >::iterator _it;
		void destroy();
};

}

#endif
