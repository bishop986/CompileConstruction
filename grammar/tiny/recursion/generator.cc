#include "include/generator.h"
#include "include/analysis.h"
#include <fstream>
#include <ctime>

namespace dh
{

void generator::genAsmCode( const ::std::string& path )
{
	::std::ofstream outf(path);

	outf << "; Generate by dh Tiny v0.1\n";
	outf << "; Filename: " << path << ::std::endl;

	::std::time_t t = time(0); 
	char tmp[64]; 
	::std::strftime( tmp, sizeof(tmp), "%Y/%m/%d %X %A %z",localtime(&t) ); 
	outf << "; Generate Time: " << tmp << ::std::endl;

	outf << "section .text\n";
	outf << "global _start\n";

	outf << "_start:\n\n";

	for( auto it : midcodes)
	{
		int size = it.getSize();
		switch(size)
		{
			case 1:
				if ( it.getOperand() == "read")
				{
					outf << ::std::endl;
					outf << "mov "
				}
		}
	}
}
}
