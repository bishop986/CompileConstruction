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

		token( ::std::string val, TYPE type, int lineno);
		TYPE getType() const;
		::std::string getVal() const;
		int getLineno() const;
		token& operator=(const token& eq);
	private:

		int _lineno;
		::std::string _val;
		TYPE _type;
};

class parser{
	public:

		void debug();
		parser();
		bool scan( ::std::FILE *fp);
		token getToken();
		bool ungetToken();
		void reset() { destroy(); }
		~parser() { destroy(); }
		parser& operator=(const parser& eq);
		void setBegin();
		bool isScanned();
		bool isRight();
	private:

		bool scanflag;
		bool rightflag;
		::std::FILE *_fp;
		::std::vector< token > _tokens;
		::std::vector< token >::const_iterator _it;
		int line;
		void destroy();
};

}

#endif
