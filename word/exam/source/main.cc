#include <iostream>
#include "include/global.h"
#include "include/scan.h"

int main( int argc, char** argv)
{
	if ( argc <= 1)
	{
		::std::cerr << "[HINT] Usage: thread <filename>.txt" << ::std::endl;
		::std::exit(1);
	}

	::std::FILE *fp = NULL;
	fp = ::std::fopen( argv[1], "r");
	if ( NULL == fp)
	{
		::std::perror(argv[1]);
		::std::exit(1);
	}

	dh::scanner tokens;
	tokens.scan( fp);

	tokens.debug();

	return 0;
}
