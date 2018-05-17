#include "include/global.h"
#include "include/scan.h"

namespace dh {

token::token( ::std::string val, TYPE type, int num)
{
	this->_val = val;
	this->_type = type;
	this->lineno = num;
}

TYPE token::getType() const
{
	return _type;
}

::std::string token::getVal() const
{
	return _val;
}

token& token::operator=(const token& eq)
{
	this->_type = eq._type;
	this->_val = eq._val;
	return *this;
}

int token::getLineno() const
{
	return lineno;
}

}
