#include <iostream>
#include "include/scan.h"
#include "include/analysis.h"

int main( int argc, char** argv)
{
	if ( argc < 2)
	{
		::std::cerr << "[ERROR] Usage: LessonProject [filename]" << ::std::endl;
		return 0;
	}

	::std::FILE *fp = NULL;
	fp = ::std::fopen( argv[1], "r");

	::dh::scanner scan;

	scan.scan(fp);
	scan.debug();

	::dh::analysis an(scan);

	::std::cout << "Result: " << an.getResult() << ::std::endl;

	return 0;
}
