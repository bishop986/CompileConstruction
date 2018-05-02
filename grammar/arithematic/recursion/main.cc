/*
 * Name: simple arithmetic calculator
 * Author: bishop986
 * Introduction:
 * Analysis the structure of a arithmetic expression
 */

#include <iostream>
#include <cstdio>
#include "scan.h"
#include "global.h"

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

	// TODO: Analysis the token table
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
}


