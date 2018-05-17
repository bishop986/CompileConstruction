#include <iostream>
#include <algorithm>
#include "include/global.h"
#include "include/scan.h"
#include "include/analysis.h"
#include "include/generator.h"
#include <unistd.h>
#include <stdlib.h>

int main( int argc, char** argv)
{
	bool notexecflag = false;
	if ( argc <= 1)
	{
		::std::cerr << "[HINT] Usage: tiny [<option> <args> ... ] <filename>.tny" << ::std::endl;
		::std::exit(1);
	}

	char getFile = 0;
	::std::FILE *fp = NULL;
	
	::std::string outname;
	::std::string execname = "a.out";
	for ( int i = 1; i < argc; ++i)
	{
		if ( argv[i][0] == '-')
		{

			switch(argv[i][1])
			{
				case 'o':
					i += 1;
					if ( i >= argc)
					{
						::std::cerr << "[ERROR] Need output name\n";
						::std::exit(1);
					}
					execname = argv[i];
					break;
				case 's':
					notexecflag = true;
					outname = argv[i];
					break;
				case 'h':
					::std::cout << "Tiny Compiler Version 0.1 by Dh" << ::std::endl;
					::std::cout << "option:" << ::std::endl;
					::std::cout << "\t-s\tgenerate assemble file" << ::std::endl;
					::std::cout << "\t-o\texecutable file with specific name" << ::std::endl;
					::std::cout << "\t-h\thelp info" << ::std::endl;
					::std::exit(0);
					break;
			}
		} else 
		{
			if ( getFile != 0)
			{
				::std::cerr << "[HIND] Sorry, tiny can not support multi file" << ::std::endl;
				::std::exit(1);
			}
			getFile += 1;
			fp = ::std::fopen( argv[i], "r");
			if ( NULL == fp)
			{
				::std::perror(argv[1]);
				::std::exit(1);
			}
			outname = argv[i];
		}
	}

	dh::scanner tokens;
	tokens.scan( fp);

#ifdef _DEBUG_
	tokens.debug();
#endif

	if ( !tokens.isRight())
	{
		return 1;
	}
	
	dh::analysis an( tokens);
	an.buildSymTab();

	an.genMidCode();

	::std::string outfilename = "";
	for( int i = outname.size(); i > 0; --i)
	{
		if ( outname[i-1] == '/'){
			break;
		}
		outfilename += outname[i-1];
	}
	::std::reverse( outfilename.begin(), outfilename.end());

	dh::generator gen(an);
	gen.genAsmCode(outfilename+".asm","../modle");


	if (!notexecflag) {
		system(("/usr/bin/nasm -f elf64 " + outfilename+".asm").c_str());
		system(("/usr/bin/ld -o" + execname + " " + outfilename+".o").c_str());
		system("rm *.o *.asm");
	}
	return 0;
}
