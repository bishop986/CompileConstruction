#include "include/scan.h"
#include "include/global.h"

namespace dh{

scanner::scanner()
{
	_fp = NULL;
	scanflag = false;
	_tokens.clear();
	line = 1;
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
				if ( cur == '\t' || cur == ' '
						|| cur == '\n' || cur == '\r')
				{
					state = STATE::START;
				} else if ( cur == '+')
				{
					state = STATE::INPLUS;
					tmp += cur;
				} else if ( cur == '-')
				{
					state = STATE::INMINUS;
					tmp += cur;
				} else if ( cur >= '0' && cur <= '9')
				{
					state = STATE::INFIDECI;
					tmp += cur;
				} else if ( ( cur >= 'a' && cur <= 'z')
						|| ( cur >= 'A' && cur <= 'Z'))
				{
					state = STATE::INID;
					tmp += cur;
				} else if ( cur == ':') 
				{
					state = STATE::INSP;
					tmp += cur;
				} else if ( cur == '=') 
				{
					state = STATE::INEQ;

					tmp += cur;
				} else 
				{
					state = STATE::DONE;
					assign = cur;
					continue;
				}
				break;
			case STATE::INID:
				if ( cur == '_')
				{
					state = STATE::INID;
					tmp += cur;
				} else if ( cur >= '0' && cur <= '9')
				{
					state = STATE::INID;
					tmp += cur;
				} else if ( ( cur >= 'a' && cur <= 'z')
						|| ( cur >= 'A' && cur <= 'Z'))
				{
					state = STATE::INID;
					tmp += cur;
				} else 
				{
					state = STATE::DONE;

					_tokens.push_back( token( tmp, TYPE::IDENTIFIER));
					tmp.clear();

					assign = cur;
					continue;
				}
				break;
			case STATE::INFIDECI:
				if ( cur <= '9' && cur >= '0')
				{
					state = STATE::INFIDECI;
					tmp += cur;
				} else if ( cur == '.')
				{
					state = STATE::INP;
					tmp += cur;
				} else 
				{
					state = STATE::START;
					tmp += cur;

					::std::cerr << "[ERROR] Unexpected Token: "
						<< tmp << " in line " << line
						<< ::std::endl;
					tmp.clear();
				}
				break;
			case STATE::INSEDECI:
				if ( cur <= '9' && cur >= '0')
				{
					state = STATE::INSEDECI;
					tmp += cur;
				} else 
				{
					state = STATE::DONE;
					assign = cur;

					_tokens.push_back( token( tmp, TYPE::DECIMAL));
					tmp.clear();

					continue;
				}
				break;
			case STATE::INPLUS:
				if ( cur <= '9' && cur >= '0')
				{
					state = STATE::INFIDECI;
					tmp += cur;
				} else if ( cur == '=')
				{
					state = STATE::INEQ;
					tmp += cur;
				} else 
				{
					state = STATE::START;
					tmp += cur;

					::std::cerr << "[ERROR] Unexpected Token: "
						<< tmp << " in line " << line
						<< ::std::endl;
					tmp.clear();
				}
				break;
			case STATE::INEQ:
				if ( cur == '>')
				{
					state = STATE::START;
					tmp += cur;

					_tokens.push_back( token( tmp, TYPE::ASSIGN));
					tmp.clear();
				} else 
				{
					state = STATE::START;
					tmp += cur;

					::std::cerr << "[ERROR] Unexpected Token: "
						<< tmp << " in line " << line
						<< ::std::endl;
					tmp.clear();
				}
				break;
			case STATE::INP:
				if ( cur >= '0' && cur <= '9')
				{
					state = STATE::INSEDECI;
					tmp += cur;
				} else
				{
					state = STATE::START;
					tmp += cur;

					::std::cerr << "[ERROR] Unexpected Token: "
						<< tmp << " in line " << line
						<< ::std::endl;
					tmp.clear();
				}
				break;
			case STATE::INMINUS:
				if ( cur == '>')
				{
					state = STATE::START;
					tmp += cur;

					_tokens.push_back( token( tmp, TYPE::ASSIGN));
				} else if ( cur >= '0' && cur <= '9')
				{
					state = STATE::INFIDECI;
					tmp += cur;
				} else 
				{
					state = STATE::START;
					tmp += cur;

					::std::cerr << "[ERROR] Unexpected Token: "
						<< tmp << " in line " << line
						<< ::std::endl;
					tmp.clear();
				}
				break;
			case STATE::INSP:
				if ( cur == ':')
				{
					state = STATE::START;
					tmp += cur;

					_tokens.push_back( token( tmp, TYPE::ASSIGN));
					tmp.clear();
				} else 
				{
					state = STATE::DONE;
					assign = cur;

					_tokens.push_back( token( tmp, TYPE::ASSIGN));

					tmp.clear();
					continue;
				}
				break;
			case STATE::DONE:
				if ( assign == ' ' || assign == '\t'
						|| assign == '\n' || assign == '\r')
				{
					state = STATE::START;
				} else if ( assign == ':') 
				{
					state = STATE::START;
					continue;
				} else if (assign == '{'
						|| assign == '}' || assign == ';')
				{
					state = STATE::START;

					::std::string str = "";
					str += assign;

					_tokens.push_back( token( str, TYPE::ASSIGN));
				} else if ( assign <= '9' && assign >= '0')
				{
					state = STATE::START;
					continue;
				} else if ( ( assign >= 'a' && assign <= 'z')
						|| ( assign >= 'A' && assign <= 'Z'))
				{
					state = STATE::START;
					continue;
				} else 
				{
					state = STATE::START;

					::std::cerr << "[ERROR] Unexpected Token: "
						<< assign << " in line " << line
						<< ::std::endl;
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
	::std::cout << "[INFO] Token Sries: " << ::std::endl;
	::std::cout << "[size]:" << _tokens.size() << ::std::endl;
	for(auto i = _tokens.begin(); i != _tokens.end(); ++i)
	{
		::std::cout << "[token VAL] "  << i->getVal();
		::std::cout << " [token TYPE] " << i->getType();
		::std::cout << ::std::endl;
	}
	::std::cout << "[INFO] Token Sries End" << ::std::endl;
}

}
