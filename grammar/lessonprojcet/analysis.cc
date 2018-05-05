#include "include/scan.h"
#include "include/global.h"
#include "include/analysis.h"
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
#ifdef _DEBUG_
	tokens.debug();
#endif
	*tmp = tokens.getToken();
	result = lexp();
	return result;
}

void analysis::match( ::std::string c)
{
#ifdef _DEBUG_
	::std::cout << "[DEBUG] in match"  << ::std::endl;
	::std::cout << "[get] "<< tmp->getVal() <<" [match] " << c << ::std::endl;
#endif
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
#ifdef _DEBUG_
	::std::cout << "[DEBUG] in number"  << ::std::endl;
#endif
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

int analysis::lexp()
{
#ifdef _DEBUG_
	::std::cout << "[DEBUG] in lexp"  << ::std::endl;
#endif

	int ret = 0;
	match("(");
	if ( tmp->getType() != TYPE::MULOP && tmp->getType() != TYPE::ADDOP)
	{
		::std::cerr << "[ERROR] Syntax Error near " << tmp->getVal() << ::std::endl;
		::std::exit(1);
	}

	if ( tmp->getVal() == "*")
	{
		match("*");
		ret = lexpseq();
		match(" ");
		ret *= lexpseq();
	} else if ( tmp->getVal() == "+")
	{
		match("+");
		ret = lexpseq();
		match(" ");
		ret += lexpseq();
	} else if ( tmp->getVal() == "-")
	{
		match("-");
		ret = lexpseq();
		match(" ");
		ret -= lexpseq();
	}
	return ret;
}

int analysis::lexpseq()
{
#ifdef _DEBUG_
	::std::cout << "[DEBUG] in lexpseq"  << ::std::endl;
#endif

	int ret = 0;
	if ( tmp->getVal() == "(")
	{
		ret = lexp();
		return ret;
	}

	ret = number();
	return ret;
}
	

}
