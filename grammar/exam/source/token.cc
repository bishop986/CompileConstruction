#include "include/global.h"
#include "include/scan.h"

namespace dh {

token::token( ::std::string val, TYPE type)
{
	this->_val = val;
	this->_type = type;
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

}
