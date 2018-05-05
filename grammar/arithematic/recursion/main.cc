/*
 * Name: simple arithmetic calculator
 * Author: bishop986
 * Introduction:
 * Analysis the structure of a arithmetic expression
 */

#include <iostream>
#include <cstdio>
#include "include/scan.h"
#include "include/global.h"
#include "include/analysis.h"

int main( int argc, char** argv)
{
	if ( argc < 2)
	{
		::std::cout << "[ERROR] Usage: main filename" << ::std::endl;
		return 1;
	}

	::std::FILE* fp = ::std::fopen( argv[1] , "r+");
	if ( fp == NULL)
	{
		::std::perror(argv[1]);
		return 1;
	}

	::dh::scanner tokens;
	tokens.scan(fp);

	::dh::analysis an(tokens);

	int ret = an.getResult();
	::std::cout << "Result is " << ret << ::std::endl;
	
	// scanner DEBUG
	/*
	while ( 1)
	{
		::dh::token tmp = tokens.getToken();
		if ( tmp.getType() != ::dh::TYPE::NONE)
		{
			::std::cout << tmp.getVal() << ::std::endl;
		} else 
		{
			break;
		}
	}
	*/
	return 0;
}


