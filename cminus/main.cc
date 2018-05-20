#include "include/global.h"
#include "include/parser.h"
#include <iostream>
#include <cstdio>

int main( int argc, char** argv)
{
	if ( argc <= 1)
	{
		::std::cerr << "[HINT] Usage: cminus {<option> <args>} <filename>.c\n";
		::std::exit(1);
	}

	::std::FILE *fp = NULL;

	fp = ::std::fopen( argv[1], "r");
	if ( !fp)
	{
		::std::perror(argv[1]);
		::std::exit(1);
	}

	dh::parser tokens;

	tokens.scan(fp);

	if ( !tokens.isRight())
	{
		return 1;
	}

#ifdef _DEBUG_
	tokens.debug();
#endif

	return 0;
}

