#include "scan.h"
#include "global.h"
#include "analysis.h"
#include <sstream>

namespace dh{

analysis::analysis(scanner tokens)
{
	this->tokens = tokens;
	this->result = 0;
}

int analysis::getResult()
{
	return result;
}

void analysis::match( ::std::string c)
{
	if ( tokens.getToken().getVal() == c)
	{
		return;
	}
	::std::cerr << "[ERROR] Syntax Error" << ::std::endl;
	::std::exit(1);
}

int analysis::number()
{
	token tmp = tokens.getToken();
	if ( tmp.getType() == TYPE::NUMBER)
	{
		::std::stringstream sstream;
		int ret = 0;

		sstream << tmp.getVal();
		sstream >> ret;

		return ret;
	}

	::std::cerr << "[ERROR] Syntax Error" << ::std::endl;
	::std::exit(1);
}

int analysis::exp()
{
	int ret = term();

	token tmp = tokens.getToken();
	while ( tokens.getToken() )
	return ret;
}

}
