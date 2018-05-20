#include "include/global.h"
#include "include/parser.h"

namespace dh {

token::token( ::std::string val, TYPE type, int lineno)
{
	this->_val = val;
	this->_type = type;
	this->_lineno = lineno;
}

TYPE token::getType() const
{
	return _type;
}

::std::string token::getVal() const
{
	return _val;
}

int token::getLineno() const
{
	return _lineno;
}

token& token::operator=(const token& eq)
{
	this->_type = eq._type;
	this->_val = eq._val;
	return *this;
}

}
