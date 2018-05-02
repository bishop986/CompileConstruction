#include "scan.h"
#include "global.h"

namespace dh{

scanner::scanner()
{
	_fp = NULL;
	scanflag = false;
	_tokens.clear();
}

void scanner::destroy()
{
	if ( _fp != NULL)
	{
		::std::fclose(NULL);
		_fp = NULL;
	}
	_tokens.clear();
}

bool scanner::isScanned()
{
	return scanflag;
}

scanner& scanner::operator=(const scanner& eq)
{
	this->_tokens.assign( eq._tokens.begin(), eq._tokens.end());
	this->_fp = eq._fp;
	this->scanflag = eq.scanflag;
	_it = _tokens.begin();;

	return *this;
}

void scanner::setBegin()
{
	_it = _tokens.begin();
}

token scanner::getToken()
{
	if ( scanflag == false || _tokens.end() == _it)
	{
		return token( "", TYPE::NONE);
	}

	//::std::cout << "[SS] " << _it->getVal() << std::endl;
	token tmp(_it->getVal(),_it->getType());
	++_it;
	//::std::cout << "[DEBUG] " << tmp.getVal() << std::endl;

	return tmp;
}

bool scanner::ungetToken()
{
	if ( scanflag == false || _tokens.begin() == _it)
	{
		return false;
	}

	--_it;
	return true;
}

/*
 * Transform Function:
 * S0 = START ; S1 = INNUMBER; S2 = DONE
 * T(S0,[0-9])=S1 ; T(S0,[+-*])=S2
 * T(S1,[0-9])=S0 ; T(S1,[+-*])=S2
 */
bool scanner::scan(::std::FILE *fp)
{
	STATE state = STATE::START;
	char cur = 0;
	char assign = 0;
	::std::string tmp = "";

	if ( fp == NULL)
	{
		return false;
	}

	cur = ::std::fgetc(fp);
	while( cur != EOF)
	{
		switch(state)
		{
			case STATE::START:
				if ( cur <= '9' && cur >= '0')
				{
					state = INNUMBER;
					tmp += cur;
				} else if ( cur == ' ' || cur == '\t'
						|| cur == '\n' || cur == '\r')
				{
					break;
				} else if ( cur == '+' || cur == '-' 
						|| cur == '*' || cur == '('
						|| cur == ')')
				{
					// when get assign we must turn to DONE immediately
					assign = cur;
					state = DONE;
					tmp.clear();
					continue;
				} else 
				{
					return false;
				}
				break;
			case STATE::INNUMBER:
				if ( cur <= '9' && cur >= '0')
				{
					tmp += cur;
				} else 
				{
					assign = cur;
					state = DONE;
					_tokens.push_back( token( tmp, TYPE::NUMBER));
					tmp.clear();
					continue;
				}
				break;
			case STATE::DONE:
				if ( assign == '+' || assign == '-')
				{
					::std::string str = "";
					str += assign;

					_tokens.push_back( token( str, TYPE::ADDOP));
				} else if ( assign == '*')
				{
					::std::string str = "";
					str += assign;

					_tokens.push_back( token( str, TYPE::MULOP));
				} else if ( assign == ' ' || assign == '\r' 
						|| assign == '\n' || assign == '\t')
				{
					state = STATE::START;
					break;
				} else if ( assign == '(' || assign == ')')
				{
					::std::string str = "";
					str += assign;

					_tokens.push_back( token( str, TYPE::BRACKET));
				}

				state = STATE::START;
				break;
		}
		cur = ::std::fgetc(fp);
	}
	_it = _tokens.begin();
	scanflag = true;;
	return true;
}

void scanner::debug()
{
	//::std::cout << _tokens.begin().base() << ::std::endl;
	::std::cout << "[size]:" << _tokens.size() << ::std::endl;
	for(auto i = _tokens.begin(); i != _tokens.end(); ++i)
	{
		::std::cout << "[token VAL] "  << i->getVal();
		::std::cout << " [token TYPE] " << i->getType();
		::std::cout << ::std::endl;
	}
}

}
