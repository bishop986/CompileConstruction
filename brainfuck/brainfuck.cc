/*
 * Name: brainfuck.cc
 * Time: 18.5.1
 * Author: Dh
 * Introduction:
 * Implement a brainfuck interpreter
 */

#include <iostream>
#include <string>
#include <vector>
#include <exception>
#include <fstream>

#include "include/brainfuck.h"
#include "include/symbolflag.h"

::dh::exceptExplain preProcess( const ::std::string&, 
		::std::vector<size_t>&, 
		::std::vector<size_t>&);

void process( const ::std::string&, 
		const ::std::vector<size_t>&, 
		const ::std::vector<size_t>&, 
		::dh::Pointer& );

void InputMode()
{
	::std::cout << "BrainFuck Interpreter v0.1 [by Dh]" << ::std::endl;
	::std::cout << ">>: ";

	// input string;
	::std::string inputstr;

	// while start position and end position
	::std::vector<size_t> posStart;
	::std::vector<size_t> posEnd;

	// process pointer
	::dh::Pointer ptr;

	while(::std::cin >> inputstr) 
	{
		// pre-process to locate while symbol and check syntax
		dh::exceptExplain explain = preProcess( inputstr, posStart, posEnd);
		bool flag = explain.what();

		if ( !flag)
		{
			::std::cout << "\n>>: ";
			continue;
		}

		try {
			process( inputstr, posStart, posEnd, ptr);
		} catch ( ::std::exception& e)
		{
			::std::cerr << e.what() << ::std::endl;
		}

		::std::cout << "\n>>: ";
		inputstr.clear();
	}
}

void FileMode( ::std::fstream& inf)
{
	::std::vector<size_t> posS;
	::std::vector<size_t> posE;
	::std::string str = "";
	::dh::Pointer ptr;

	while(1)
	{
		char tmp = inf.get();
		if ( EOF == tmp)
		{
			break;
		}
		//::std::cout << "[DEBUG] " << int(tmp) << ::std:: endl ;
		str += tmp;
	}


	dh::exceptExplain explain = preProcess( str, posS, posE);
	bool flag = explain.what();

	if ( !flag)
	{
		return ;
	}

	try {
		process( str, posS, posE, ptr);
	} catch ( ::std::exception& e)
	{
		::std::cerr << e.what() << ::std::endl;
	}
}

::dh::exceptExplain preProcess( const ::std::string& str, 
		::std::vector<size_t>& posS, 
		::std::vector<size_t>& posE)
{
	for( size_t i = 0; i < str.length(); ++i)
	{
		switch(str.at(i))
		{
			case '\r':
			case '\n':
			case '+':
			case '-':
			case '>':
			case '<':
			case '.':
			case ',':
				break;
			case '[':
				posS.push_back(i);
				//::std::cout << "[DEBUG] posS " << i << ::std::endl;
				break;
			case ']':
				posE.push_back(i);
				//::std::cout << "[DEBUG] posE " << i << ::std::endl;
				break;
			default:
				::dh::exceptExplain e( ::dh::UNEXPECTTOKEN, str.at(i), i);	
				return e;
		}
	}

	if ( posS.size() == posE.size())
	{
		::dh::exceptExplain e;
		return e;
	} else if ( posS.size() > posE.size())
	{
		::dh::exceptExplain e( ::dh::exceptFlag::EXPECTTOEKN, ']', posS.at(0));
		return e;
	} else 
	{
		::dh::exceptExplain e( ::dh::exceptFlag::EXPECTTOEKN, '[', posE.at(posE.size()-1));
		return e;
	}
}

void process( const ::std::string& str, 
		const ::std::vector<size_t>& posS, 
		const ::std::vector<size_t>& posE,
		::dh::Pointer& ptr)
{
	size_t while_count = 0;
	size_t i = 0;
	while ( i < str.length())
	{
		//::std::cout << "[DEBUG] i " << i << ::std::endl;
		switch(str.at(i))
		{
			case '>':
				++ptr;
				break;
			case '<':
				--ptr;
				break;
			case '+':
				++(*ptr);
				break;
			case '-':
				--(*ptr);
				break;
			case ',':
				*ptr = ::std::getchar();
				break;
			case '.':
				::std::putchar(*ptr);
				break;
			case '[':
				++while_count;
				if ( 0 == *ptr)
				{
					i = posE.at( posE.size() - while_count);
					continue;
				}
				//::std::cout << "[DEBUG] while_count " << while_count << ::std::endl;
				break;
			case ']':
				if ( 0 != *ptr)
				{
					i = posS.at( while_count - 1);
					--while_count;
					continue;
				}
				--while_count;
				//::std::cout << "[DEBUG] while_count " << while_count << ::std::endl;
				break;
		}
		++i;
	}
}

/*
 * There's two mode of interpreter
 */
int main( int argc, char** argv)
{
	if ( 1 == argc)
	{
		InputMode();
		return 0;
	}

	::std::fstream inf(argv[1], ::std::ios::in);
	if ( !inf)
	{
		::std::cerr << "[ERROR] Fail to open file " << argv[1] << ::std::endl;
		return 1;
	}

	FileMode(inf);
	return 0;
}
