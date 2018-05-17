#include "include/scan.h"
#include "include/global.h"

namespace dh{

scanner::scanner()
{
	_fp = NULL;
	scanflag = false;
	rightflag = true;
	line = 1;
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

bool scanner::isRight()
{
	return rightflag;
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
		return token( "", TYPE::NONE, 0);
	}

	//::std::cout << "[SS] " << _it->getVal() << std::endl;
	token tmp(_it->getVal(),_it->getType(), _it->getLineno());
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
	while( cur == ' ' || cur == '\t') cur = ::std::fgetc(fp);

	while( cur != EOF)
	{
		if ( cur == '\n')
		{
			++line;
		}
		switch(state)
		{
			case STATE::START:
				if ( cur == ' ' || cur == '\t'
						|| cur == '\n' || cur == '\r')
				{
					state = STATE::START;
				} else if ( cur == '{')
				{
					state = STATE::INCOMMENT;
				} else if ( cur >= '0' && cur <= '9')
				{
					state = STATE::INNUMBER;
					tmp += cur;
				} else if ( (cur >= 'a' && cur <= 'z')
						|| ( cur >= 'A' && cur <= 'Z'))
				{
					state = STATE::INID;
					tmp += cur;
				} else if ( cur == ':') 
				{
					state = STATE::INASSIGN;
					tmp += cur;
				}else 
				{
					state = STATE::DONE;
					assign = cur;
					continue;
				}
				break;
			case STATE::INNUMBER:
				if ( cur >= '0' && cur <= '9')
				{
					state = STATE::INNUMBER;
					tmp += cur;
				} else 
				{
					state = STATE::DONE;
					assign = cur;

					_tokens.push_back( dh::token( tmp, TYPE::NUM,line));
					tmp.clear();
					continue;
				}
				break;
			case STATE::INCOMMENT:
				if ( cur != '}')
				{
					state = STATE::INCOMMENT;
				} else 
				{
					state = STATE::START;
				}
				break;
			case STATE::INASSIGN:
				if ( cur == '=')
				{
					state = STATE::START;	
					tmp += cur;
					
					_tokens.push_back( dh::token( tmp, TYPE::ASSIGN, line));
					tmp.clear();
				} else 
				{
					state = STATE::START;
					::std::cerr << "[ERROR] Unexpected Token: \":" << cur << "\""
						<< " in line " << line 
						<< ::std::endl;
					rightflag = false;
				}
				break;
			case STATE::INID:
				if ( ( cur >= 'a' && cur <= 'z') 
						|| ( cur >= 'A' && cur <= 'Z'))
				{
					state = STATE::INID;
					tmp += cur;
				} else 
				{
					state = STATE::DONE;
					assign = cur;

					if ( tmp == "read" || tmp == "if" || tmp == "then"
							|| tmp == "else" || tmp == "write" || tmp == "repeat"
							|| tmp == "until" || tmp == "end")
					{
						_tokens.push_back( dh::token( tmp, TYPE::RESERVED, line));
					} else 
					{
						_tokens.push_back( dh::token( tmp, TYPE::ID, line));
					}

					tmp.clear();
					continue;
				}
				break;
			case STATE::DONE:
				if ( assign == ' ' || assign == '\t' || assign == '\n'
						|| assign == '\r')
				{
					state = STATE::START;
				} else if ( assign == '+' || assign == '-' || assign == '*'
						|| assign == '/' || assign == '<' || assign == '='
						|| assign == '(' || assign == ')' || assign == ';')
				{
					state = STATE::START;
					::std::string str = "";
					str += assign;

					_tokens.push_back( dh::token( str, TYPE::ASSIGN, line));
				} else 
				{
					state = STATE::START;
					::std::cerr << "[ERROR] Unexpected Token: \"" << assign << "\""
						<< " in line " << line
						<< ::std::endl;
					rightflag = false;
				}
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
	if ( !rightflag)
	{
		return ;
	}
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
