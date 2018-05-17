#include "include/generator.h"
#include "include/analysis.h"
#include <fstream>
#include <ctime>

namespace dh
{

void generator::genAsmCode( const ::std::string& path , const ::std::string& modle)
{
	::std::ofstream outf(path);
	::std::ifstream intfmain(modle+"/main.asm");

	outf << "; Generate by dh Tiny v0.1\n";
	outf << "; Filename: " << path << ::std::endl;

	::std::time_t t = time(0); 
	char tmp[64]; 
	::std::strftime( tmp, sizeof(tmp), "%Y/%m/%d %X %A %z",localtime(&t) ); 
	outf << "; Generate Time: " << tmp << ::std::endl;

	::std::string tmp_str = "";

	while(tmp_str != "\tnop")
	{
		::std::getline(intfmain,tmp_str);
		outf << tmp_str << ::std::endl;
	}


	for( auto it : midcodes)
	{
		int size = it.getSize();
		switch(size)
		{
			case 1:
				if ( it.getOperand() == "read")
				{
					outf << ::std::endl;
					outf << "\tmov rax, " << it.getVal1() << ::std::endl;
					outf << "\tcall _read\n";
				} else if ( it.getOperand() == "write")
				{
					outf << ::std::endl;
					if ( (it.getVal1().at(0) <= '9' && it.getVal1().at(0) >= '0')
							|| it.getVal1().at(0) == '-')
					{
						outf << "\tmov rax, " << it.getVal1() << "\n";
					} else 
					{
						outf << "\tmov rax, [" << it.getVal1() << "]\n";
					}
					outf << "\tcall _write\n";
				} else if ( it.getOperand() == "label")
				{
					outf << ::std::endl;
					outf << it.getVal1() << ":\n";
				} else if ( it.getOperand() == "jne")
				{
					outf << ::std::endl;
					outf << "\tjne " << it.getVal1() << ::std::endl;
				} else if ( it.getOperand() == "cmp")
				{
					outf << ::std::endl;
					outf << "\tcmp qword [" << it.getVal1() << "], 1" << ::std::endl;
				} else if ( it.getOperand() == "jmp")
				{
					outf << ::std::endl;
					outf << "\tjmp " << it.getVal1() << ::std::endl;
				}
			case 2:
				if ( it.getOperand() == ":=")
				{
					outf << ::std::endl;
					if ( (it.getVal1().at(0) <= '9' && it.getVal1().at(0) >= '0')
							|| it.getVal1().at(0) == '-')
					{
						outf << "\tmov rax, " << it.getVal1() <<  ::std::endl;
						outf << "\tmov qword [" << it.getRes() << "], rax" << ::std::endl;
					} else 
					{
						outf << "\tmov rax, [" << it.getVal1() << "]"<< ::std::endl;
						outf << "\tmov qword [" << it.getRes() << "], rax" << ::std::endl;
					}
				}
			case 3:
				if ( it.getOperand() == "+")
				{
					outf << ::std::endl;
					if ( (it.getVal1().at(0) <= '9' && it.getVal1().at(0) >= '0')
							|| it.getVal1().at(0) == '-')
					{
						outf << "\tmov rax," << it.getVal1() <<  ::std::endl;
						outf << "\tmov qword [" << it.getRes() << "], rax" << ::std::endl;
					} else 
					{
						outf << "\tmov rax, [" << it.getVal1() << "]"<< ::std::endl;
						outf << "\tmov qword [" << it.getRes() << "], rax" << ::std::endl;
					}
					if ( (it.getVal2().at(0) <= '9' && it.getVal2().at(0) >= '0')
							|| it.getVal2().at(0) == '-')
					{
						outf << "\tmov rax," << it.getVal2() <<  ::std::endl;
					} else 
					{
						outf << "\tmov rax, [" << it.getVal2() << "]"<< ::std::endl;
					}
					outf << "\tadd qword [" << it.getRes() << "], rax" << ::std::endl;
				} else if ( it.getOperand() == "-")
				{
					outf << ::std::endl;
					if ( it.getVal1().at(0) <= '9' && it.getVal1().at(0) >= '0')
					{
						outf << "\tmov rax," << it.getVal1() <<  ::std::endl;
						outf << "\tmov qword [" << it.getRes() << "], rax" << ::std::endl;
					} else 
					{
						outf << "\tmov rax, [" << it.getVal1() << "]"<< ::std::endl;
						outf << "\tmov qword [" << it.getRes() << "], rax" << ::std::endl;
					}
					if ( (it.getVal2().at(0) <= '9' && it.getVal2().at(0) >= '0')
							|| it.getVal2().at(0) == '-')
					{
						outf << "\tmov rax," << it.getVal2() <<  ::std::endl;
					} else 
					{
						outf << "\tmov rax, [" << it.getVal2() << "]"<< ::std::endl;
					}
					outf << "\tsub qword [" << it.getRes() << "], rax" << ::std::endl;
				} else if ( it.getOperand() == "*")
				{
					outf << ::std::endl;
					if ( (it.getVal1().at(0) <= '9' && it.getVal1().at(0) >= '0')
							|| it.getVal1().at(0) == '-')
					{
						outf << "\tmov rax," << it.getVal1() <<  ::std::endl;
					} else 
					{
						outf << "\tmov rax, [" << it.getVal1() << "]"<< ::std::endl;
					}
					if ( (it.getVal2().at(0) <= '9' && it.getVal2().at(0) >= '0')
							|| it.getVal2().at(0) == '-')
					{
						outf << "\tmov rbx, " << it.getVal2() <<  ::std::endl;
						outf << "\tmul rbx " << ::std::endl;
					} else 
					{
						outf << "\tmul qword [" << it.getVal2() << "]"<< ::std::endl;
					}
					outf << "\tmov qword [" << it.getRes() << "], rax" << ::std::endl;
				} else if ( it.getOperand() == "=")
				{
					outf << ::std::endl;
					if ( (it.getVal1().at(0) <= '9' && it.getVal1().at(0) >= '0')
							|| it.getVal1().at(0) == '-')
					{
						outf << "\tmov rax," << it.getVal1() <<  ::std::endl;
					} else 
					{
						outf << "\tmov rax, [" << it.getVal1() << "]"<< ::std::endl;
					}
					if ( (it.getVal2().at(0) <= '9' && it.getVal2().at(0) >= '0')
							|| it.getVal2().at(0) == '-')
					{
						outf << "\tmov rbx, " << it.getVal2() <<  ::std::endl;
					} else 
					{
						outf << "\tmov rbx, [" << it.getVal2() << "]"<< ::std::endl;
					}
					outf << "\tcall _eqfunc\n";
					outf << "\tmov qword [" << it.getRes() << "], rax" << ::std::endl;
				} else if ( it.getOperand() == "<")
				{
					outf << ::std::endl;
					if ( (it.getVal1().at(0) <= '9' && it.getVal1().at(0) >= '0') 
							|| it.getVal1().at(0) == '-')
					{
						outf << "\tmov rax, " << it.getVal1() <<  ::std::endl;
					} else 
					{
						outf << "\tmov rax, [" << it.getVal1() << "]"<< ::std::endl;
					}
					if ( (it.getVal2().at(0) <= '9' && it.getVal2().at(0) >= '0')
							|| it.getVal2().at(0) == '-')
					{
						outf << "\tmov rbx, " << it.getVal2() <<  ::std::endl;
					} else 
					{
						outf << "\tmov rbx, [" << it.getVal2() << "]"<< ::std::endl;
					}
					outf << "\tcall _lefunc\n";
					outf << "\tmov qword [" << it.getRes() << "], rax" << ::std::endl;
				}
		}
	}

	while(!intfmain.eof())
	{
		::std::getline(intfmain,tmp_str);
		outf << tmp_str << ::std::endl;
	}

	for ( auto x : this->symTab)
	{
		outf << "\t" << x.first << ": resb 8\n" ;
	}

	outf.close();
	intfmain.close();
}

}
