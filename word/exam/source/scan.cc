#include "include/scan.h"
#include "include/global.h"
#include "include/json.hpp"
#include <fstream>

using json = ::nlohmann::json;

namespace dh{

scanner::scanner()
{
	_fp = NULL;
	scanflag = false;
	rightflag = true;
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
		return token( "", TYPE::NONES);
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
					assign = cur;
					TYPE type_tmp = TYPE::IDENTIFIER;

					if ( tmp == "thread")
					{
						type_tmp = TYPE::THREAD;
					} else if ( tmp == "features")
					{
						type_tmp = TYPE::FEATURES;
					} else if ( tmp == "flows")
					{
						type_tmp = TYPE::FLOWS;
					} else if ( tmp == "properties")
					{
						type_tmp = TYPE::PROPERTIES;
					} else if ( tmp == "end")
					{
						type_tmp = TYPE::END;
					} else if ( tmp == "none")
					{
						type_tmp = TYPE::NONE;
					} else if ( tmp == "data") 
					{
						type_tmp = TYPE::DATA;
					} else if ( tmp == "out")
					{
						type_tmp = TYPE::OUT;
					} else if ( tmp == "port")
					{
						type_tmp = TYPE::PORT;
					} else if ( tmp == "event")
					{
						type_tmp = TYPE::EVENT;
					} else if ( tmp == "parameter")
					{
						type_tmp = TYPE::PARAMETER;
					} else if ( tmp == "flow")
					{
						type_tmp = TYPE::FLOW;
					} else if ( tmp == "source")
					{
						type_tmp = TYPE::SOURCE;
					} else if ( tmp == "path")
					{
						type_tmp = TYPE::PATH;
					} else if ( tmp == "constant")
					{
						type_tmp = TYPE::CONSTANT;
					} else if ( tmp == "access")
					{
						type_tmp = TYPE::ACCESS;
					} else if ( tmp == "sink") 
					{
						type_tmp = TYPE::SINK;
					} else 
					{
						type_tmp = TYPE::IDENTIFIER;
					}

					_tokens.push_back( token( tmp, type_tmp));
					tmp.clear();

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

					::std::ofstream outf("../tokenOut.txt", ::std::ios::app);
					outf << "[ERROR] Unexpected Token: \""
						<< tmp << "\" in line " << line
						<< ::std::endl;
					tmp.clear();
					rightflag = false;
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

					::std::ofstream outf("../tokenOut.txt", ::std::ios::app);
					outf << "[ERROR] Unexpected Token: \""
						<< tmp << "\" in line " << line
						<< ::std::endl;
					tmp.clear();
					rightflag = false;
				}
				break;
			case STATE::INEQ:
				if ( cur == '>')
				{
					state = STATE::START;
					tmp += cur;

					_tokens.push_back( token( tmp, TYPE::SYMBOL));
					tmp.clear();
				} else 
				{
					state = STATE::START;
					tmp += cur;

					::std::ofstream outf("../tokenOut.txt", ::std::ios::app);
					outf << "[ERROR] Unexpected Token: \""
						<< tmp << "\" in line " << line
						<< ::std::endl;
					tmp.clear();
					rightflag = false;
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

					::std::ofstream outf("../tokenOut.txt", ::std::ios::app);
					outf << "[ERROR] Unexpected Token: \""
						<< tmp << "\" in line " << line
						<< ::std::endl;
					tmp.clear();
					rightflag = false;
				}
				break;
			case STATE::INMINUS:
				if ( cur == '>')
				{
					state = STATE::START;
					tmp += cur;

					_tokens.push_back( token( tmp, TYPE::SYMBOL));
					tmp.clear();
				} else if ( cur >= '0' && cur <= '9')
				{
					state = STATE::INFIDECI;
					tmp += cur;
				} else 
				{
					state = STATE::START;
					tmp += cur;

					::std::ofstream outf("../tokenOut.txt", ::std::ios::app);
					outf << "[ERROR] Unexpected Token: \""
						<< tmp << "\" in line " << line
						<< ::std::endl;
					tmp.clear();
					rightflag = false;
				}
				break;
			case STATE::INSP:
				if ( cur == ':')
				{
					state = STATE::START;
					tmp += cur;

					_tokens.push_back( token( tmp, TYPE::SYMBOL));
					tmp.clear();
				} else 
				{
					state = STATE::DONE;
					assign = cur;

					_tokens.push_back( token( tmp, TYPE::SYMBOL));

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

					_tokens.push_back( token( str, TYPE::SYMBOL));
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

					::std::ofstream outf("../tokenOut.txt", ::std::ios::app);
					outf << "[ERROR] Unexpected Token: \""
						<< assign << "\" in line " << line
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
	json j;

	if ( !rightflag)
	{
		return;
	}
	::std::ofstream outf("../tokenOut.txt", ::std::ios::app);

	outf << "[INFO] Token Sries: " << ::std::endl;
	outf << "[INFO] Size: " << _tokens.size() << ::std::endl;

	int counter = 0;
	for(auto i = _tokens.begin(); i != _tokens.end(); ++i)
	{
		j[counter]["Token Val"] = i->getVal();

		::std::string type_str = "";
		switch( i->getType())
		{
			case THREAD:
				type_str = "THREAD";
				break;
			case FEATURES:	
				type_str = "FEATURES";
				break;
			case FLOWS:
				type_str = "FLOWS";
				break;
			case PROPERTIES:
				type_str = "PROPERTIES";
				break;
			case END:
				type_str = "END";
				break;
			case NONE:
				type_str = "NONE";
				break;
			case NONES:
				type_str = "NONES";
				break;
			case OUT:
				type_str = "OUT";
				break;
			case DATA:
				type_str = "DATA";
				break;
			case PORT:
				type_str = "PORT";
				break;
			case EVENT:
				type_str = "EVENT";
				break;
			case PARAMETER:
				type_str = "PARAMETER";
				break;
			case FLOW:
				type_str = "FLOW";
				break;
			case SOURCE:
				type_str = "SOURCE";
				break;
			case PATH:
				type_str = "PATH";
				break;
			case CONSTANT:
				type_str = "CONSTANT";
				break;
			case ACCESS:
				type_str = "ACCESS";
				break;
			case IDENTIFIER:
				type_str = "IDENTIFIER";
				break;
			case DECIMAL:
				type_str = "DECIMAL";
				break;
			case SYMBOL:
				type_str = "SYMBOL";
				break;
			case SINK:
				type_str = "SINK";
				break;
		}

		j[counter++]["Token Type"] = type_str;
	}
	outf << ::std::setw(4) << j << std::endl;

	outf << "[INFO] Token End" << ::std::endl;
}

}
