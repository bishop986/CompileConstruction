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
		TYPE getType() const;
		::std::string getVal() const;
		token& operator=(const token& eq);
	private:

		::std::string _val;
		TYPE _type;
};

class scanner{
	public:

		void debug();
		scanner();
		bool scan( ::std::FILE *fp);
		token getToken();
		bool ungetToken();
		void reset() { destroy(); }
		~scanner() { destroy(); }
		scanner& operator=(const scanner& eq);
		void setBegin();
		bool isScanned();
	private:

		bool scanflag;
		::std::FILE *_fp;
		::std::vector< token > _tokens;
		::std::vector< token >::const_iterator _it;
		void destroy();
};

}

#endif
