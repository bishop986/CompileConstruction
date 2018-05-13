#include <iostream>
#include "include/global.h"
#include "include/scan.h"
#include "include/analysis.h"

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

	if ( !tokens.isRight())
	{
		return 1;
	}

	std::cout << "\n\n[HINT] PostOrder print this tree:" << ::std::endl;
	dh::analysis an(tokens);
	an.printTree();

	return 0;
}
