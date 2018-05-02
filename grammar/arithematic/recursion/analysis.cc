#include "scan.h"
#include "global.h"
#include "analysis.h"
#include <exception>
#include <sstream>

namespace dh{

/*
 * G[exp]
 * exp->term{ addop term}
 * term->factor{ mulop term}
 * factor->(exp)|number
 * addop=+|-
 * mulop=*
 * number=[0-9]+
 */

analysis::analysis(scanner tokens)
{
	this->tokens = tokens;
	this->result = 0;
	tmp = ::boost::make_shared<token>("",TYPE::NONE);

	if ( !tokens.isScanned())
	{
		throw "[ERROR] scanner is not ready";
	}
}

int analysis::getResult()
{
	//tokens.debug();
	*tmp = tokens.getToken();
	result = exp();
	return result;
}

void analysis::match( ::std::string c)
{
	::std::cout << "[DEBUG] in match"  << ::std::endl;
	::std::cout << "[get] "<< tmp->getVal() <<" [match] " << c << ::std::endl;
	if ( tmp->getVal() == c)
	{
		*tmp = tokens.getToken();
		return;
	}

	tokens.ungetToken();
	*tmp = tokens.getToken();
	::std::cerr << "[ERROR] Syntax Error near " << tmp->getVal() << ::std::endl;
	::std::exit(1);
}

int analysis::number()
{
	::std::cout << "[DEBUG] in number"  << ::std::endl;
	if ( tmp->getType() == TYPE::NUMBER)
	{
		::std::stringstream sstream;
		int ret = 0;

		sstream << tmp->getVal();
		sstream >> ret;
		*tmp = tokens.getToken();

		return ret;
	}

	tokens.ungetToken();
	*tmp = tokens.getToken();
	::std::cerr << "[ERROR] Syntax Error near " << tmp->getVal() << ::std::endl;
	::std::exit(1);
}

int analysis::exp()
{

	::std::cout << "[DEBUG] in exp"  << ::std::endl;
	int ret = term();

	while (1)
	{
		if ( tmp->getType() != TYPE::ADDOP) 
		{
			break;
		}

		if ( tmp->getVal() == "+")
		{
			match("+");
			ret += term();
		} else {
			match("-");
			ret -= term();
		}

	}
	return ret;
}

int analysis::term()
{
	::std::cout << "[DEBUG] in term"  << ::std::endl;
	int ret = factor();

	while(1)
	{
		if ( tmp->getType() != TYPE::MULOP) 
		{
			break;
		}

		match("*");
		ret *= factor();

	}
	return ret;
}

int analysis::factor()
{
	::std::cout << "[DEBUG] in factor"  << ::std::endl;

	//std::cout << "[DEBUG] " << tmp->getVal() << ::std::endl;

	if ( "(" == tmp->getVal())
	{
		match("(");
		int ret = exp();
		match(")");
		return ret;
	} else if ( tmp->getType() == TYPE::NUMBER)
	{
		return number();
	} else 
	{
		tokens.ungetToken();
		*tmp = tokens.getToken();
		::std::cerr << "[ERROR] Syntax Error near " << tmp->getVal() << ::std::endl;
		::std::exit(1);
	}

	
}

}
