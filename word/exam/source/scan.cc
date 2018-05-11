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
		switch(state)
		{
			case STATE::START:

				if ( cur == '\n')
				{
					++line;
				}

				if ( cur == ' ' || cur == '\t' || cur == '\n'
						|| cur == '\r')
				{
					state = STATE::START;
				} else if ( (cur >= 'a' && cur <= 'z' ) 
						|| (cur >= 'A' && cur <= 'Z'))
				{
					state = STATE::INID;
					tmp += cur;
				} else if ( cur == '+')
				{
					tmp += cur;
					cur = fgetc(fp);
					if ( cur == '=')
					{
						tmp += cur;
						cur = fgetc(fp);
						if ( cur == '>') 
						{
							tmp += cur;
							state =  STATE::START;

							_tokens.push_back( token( token( tmp, TYPE::ASSIGN)));
							tmp.clear();

							break;
						} else 
						{
							tmp += cur;

							::std::cerr << "[ERROR] Unexpected token: " << tmp;
							::std::cerr << " in line " << line;
							::std::cerr << ::std::endl;

							tmp.clear();
							state = STATE::START;
						}
					} else if ( cur <= '9' && cur >= '0')
					{
						state = STATE::INDECIMAL;
						tmp += cur;
					} else 
					{
						tmp += cur;

						::std::cerr << "[ERROR] Unexpected token: " << tmp;
						::std::cerr << " in line " << line;
						::std::cerr << ::std::endl;

						tmp.clear();
						state = STATE::START;
					}
				} else if ( cur == '-') 
				{
					tmp += cur;
					cur = fgetc(fp);

					if ( cur == '>')
					{
						state = STATE::START;
						tmp += cur;
						_tokens.push_back(token( tmp, TYPE::ASSIGN));
						tmp.clear();
					} else if ( cur >= '0' && cur <= '9')
					{
						state = STATE::INDECIMAL;
						tmp += cur;
					} else
					{
						tmp += cur;

						::std::cerr << "[ERROR] Unexpected token: " << tmp;
						::std::cerr << " in line " << line;
						::std::cerr << ::std::endl;

						tmp.clear();
						state = STATE::START;

					}
				} else if ( cur >= '0' && cur <= '9')
				{
					state = STATE::INDECIMAL;
					tmp += cur;
				} else if ( cur == '=')
				{
					state = STATE::START;
					tmp += cur;

					cur = fgetc(fp);
					if ( cur == '>')
					{
						tmp += cur;
						_tokens.push_back( token( tmp, TYPE::ASSIGN));
						tmp.clear();
						break;
					} else 
					{
						::std::cerr << "[ERROR] Unexpected token: " << tmp;
						::std::cerr << " in line " << line;
						::std::cerr << ::std::endl;

						tmp.clear();
						state = STATE::START;

					}
				} else if ( cur == ':')
				{
					state = STATE::START;
					tmp += cur;

					cur = fgetc(fp);
					if ( cur == ':')
					{
						tmp += cur ;
						_tokens.push_back( token( tmp, TYPE::ASSIGN));
						tmp.clear();

						break;
					} else 
					{
						_tokens.push_back( token( tmp, TYPE::ASSIGN));

						tmp.clear();
						continue;
					}
				}else 
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
				} else if ( (cur >= 'a' && cur <= 'z')
						|| (cur >= 'A' && cur <= 'Z'))
				{
					state = STATE::INID;
					tmp += cur;
				} else {
					state = STATE::DONE;

					assign = cur;

					_tokens.push_back( token( tmp, TYPE::IDENTIFIER));
					tmp.clear();
					continue;
				}
				break;
			case STATE::INDECIMAL:
				if ( cur >= '0' && cur <= '9')
				{
					state = STATE::INDECIMAL;
					tmp += cur;
				} else if ( cur == '.')
				{
					state = STATE::INDECIMAL;
					tmp += cur;
				} else 
				{
					if ( tmp.at(tmp.size()-1) == '.')
					{
						::std::cerr << "[ERROR] Unexpected token: " << tmp;
						::std::cerr << " in line " << line;
						::std::cerr << ::std::endl;

						tmp.clear();
						state = STATE::START;

					}

					state = STATE::DONE;
					assign = cur;

					_tokens.push_back(token( tmp, TYPE::DECIMAL));
					tmp.clear();

					continue;
				}
				break;
			case STATE::DONE:
				if ( assign == '\n' || assign == ' ' 
						|| assign == '\r' || assign == '\t')
				{
					state = STATE::START;
				} else if ( assign == ';' || assign == '{'
						|| assign == '}')
				{
					state = STATE::START;
					
					::std::string str = "";
					str += assign;
					_tokens.push_back( token( str, TYPE::ASSIGN));

				} else if ( assign == '=' || assign == '+' 
						|| assign == '-' || assign == ':')
				{
					state = STATE::START;
					continue;
				} else
				{
					::std::cerr << "[ERROR] Unexpected token: " << assign;
					::std::cerr << " in line " << line;
					::std::cerr << ::std::endl;

					tmp.clear();
					state = STATE::START;

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
