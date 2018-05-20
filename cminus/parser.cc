#include "include/parser.h"
#include "include/global.h"
#include <fstream>

namespace dh{

parser::parser()
{
	_fp = NULL;
	scanflag = false;
	rightflag = true;
	_tokens.clear();
	line = 1;
}

void parser::destroy()
{
	if ( _fp != NULL)
	{
		::std::fclose(NULL);
		_fp = NULL;
	}
	_tokens.clear();
}

bool parser::isScanned()
{
	return scanflag;
}

bool parser::isRight()
{
	return rightflag;
}
parser& parser::operator=(const parser& eq)
{
	this->_tokens.assign( eq._tokens.begin(), eq._tokens.end());
	this->_fp = eq._fp;
	this->scanflag = eq.scanflag;
	_it = _tokens.begin();;

	return *this;
}

void parser::setBegin()
{
	_it = _tokens.begin();
}

token parser::getToken()
{
	if ( scanflag == false || _tokens.end() == _it)
	{
		return token( "", TYPE::NONES, 0);
	}

	//::std::cout << "[SS] " << _it->getVal() << std::endl;
	token tmp(_it->getVal(),_it->getType(), 0);
	++_it;
	//::std::cout << "[DEBUG] " << tmp.getVal() << std::endl;

	return tmp;
}

bool parser::ungetToken()
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
bool parser::scan(::std::FILE *fp)
{
	_tokens.clear();
	rightflag = true;
	STATE state = STATE::START;
	char cur = 0;
	char lastch = 0;
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
				if ( cur == ' ' || cur == '\r' || cur == '\n' || cur == '\t')
				{
					state = STATE::START;
				}
				else if ( ( cur >= 'a' && cur <= 'z')
						|| ( cur >= 'A' && cur <= 'Z'))
				{
					state = STATE::INID;

					tmp += cur;
				} else if ( cur <= '9' && cur >= '0')
				{
					state = STATE::INID;

					tmp += cur;
				} else 
				{
					state = STATE::DONE;

					lastch = cur;
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
					TYPE type_id = TYPE::ID;

					if ( tmp == "if")
					{
						type_id = TYPE::IF;
					} else if ( tmp == "else")
					{
						type_id = TYPE::ELSE;
					} else if ( tmp == "int")
					{
						type_id = TYPE::INT;
					} else if ( tmp == "void")
					{
						type_id = TYPE::VOID;
					} else if ( tmp == "return")
					{
						type_id = TYPE::RETURN;
					} else if ( tmp == "while")
					{
						type_id = TYPE::WHILE;
					} else 
					{
						type_id = TYPE::ID;
					}

					_tokens.push_back( token( tmp, type_id, line));

					tmp.clear();
					lastch = cur;
				}
				break;
			case STATE::INCOMMENT:
				if ( cur == '*')
				{
					state = STATE::INCOMMENT;

					lastch = cur;
				} else if ( cur == '/' && lastch == '*')
				{
					state = STATE::START;
				} else 
				{
					state = STATE::INCOMMENT;
				}
				break;
			case STATE::INNUM:
				if ( cur <= '9' && cur >= '0')
				{
					state = STATE::INNUM;

					tmp += cur;
				} else 
				{
					state = STATE::DONE;

					_tokens.push_back( token( tmp, TYPE::NUM, line));

					tmp.clear();
					lastch = cur;
				}
				break;
			case STATE::DONE:
				::std::string tmpstr = "";
				switch(lastch)
				{
					case '/':
						if ( cur == '*')
						{
							state = STATE::INCOMMENT;
							cur = ::std::fgetc(fp);
							continue;
						} else 
						{
							tmpstr = "/";
							_tokens.push_back( token( tmpstr, TYPE::OP, line));
						}
						break;
					case '=':
						state = STATE::START;
						if ( cur == '=')
						{
							tmpstr = "==";
							cur = ::std::fgetc(fp);
						} else 
						{
							tmpstr = "=";
						}
						_tokens.push_back( token( tmpstr, TYPE::OP, line));
						break;
					case '!':
						state = STATE::START;
						if ( cur == '=')
						{
							tmpstr = "!=";
							cur = ::std::fgetc(fp);
							_tokens.push_back( token( tmpstr, TYPE::OP, line));
						} else 
						{
							::std::cerr << "[ERROR] Unexpected token \'!\'"
								<< " in line " << line << ::std::endl;
							rightflag = false;
						}
						break;
					case '<':
						if ( cur == '=')
						{
							tmpstr = "<=";
							cur = ::std::fgetc(fp);
						} else 
						{
							tmpstr = "<";
						}
						_tokens.push_back( token( tmpstr, TYPE::OP, line));
						break;
					case '>':
						if ( cur == '=')
						{
							tmpstr = ">=";
							cur = ::std::fgetc(fp);
						} else 
						{
							tmpstr = ">";
						}
						_tokens.push_back( token( tmpstr, TYPE::OP, line));
						break;
					case '+':
					case '-':
					case '*':
					case ',':
					case '[':
					case ']':
					case '{':
					case '}':
					case '(':
					case ')':
					case ';':
						tmpstr += lastch;
						_tokens.push_back( token( tmpstr, TYPE::OP, line));
						break;
					case '\r':
					case '\n':
					case '\t':
					case ' ':
						break;
					default:
						rightflag = false;
						std::cerr << "[ERROR] Unexpected token \"" << lastch << "\""
							<< " in line " << line << ::std::endl;
				}
				tmpstr.clear();
				state = STATE::START;

				continue;
		}
		cur = ::std::fgetc(fp);
	}
	_it = _tokens.begin();
	scanflag = true;;
	return true;
}

void parser::debug()
{

	if ( !rightflag)
	{
		return;
	}

	::std::cout << "[INFO] Token Sries: " << ::std::endl;
	::std::cout << "[INFO] Size: " << _tokens.size() << ::std::endl;

	for(auto i = _tokens.begin(); i != _tokens.end(); ++i)
	{
		::std::string typestr;
		switch(i->getType())
		{
			case TYPE::ID:
				typestr = "ID";
				break;
			case TYPE::OP:
				typestr = "OP";
				break;
			case TYPE::IF:
				typestr = "IF";
				break;
			case TYPE::WHILE:
				typestr = "WHILE";
				break;
			case TYPE::NUM:
				typestr = "NUM";
				break;
			case TYPE::ELSE:
				typestr = "ELSE";
				break;
			case TYPE::RETURN:
				typestr = "RETURN";
				break;
			case TYPE::VOID:
				typestr = "VOID";
				break;
			case TYPE::INT:
				typestr = "INT";
				break;
			case TYPE::NONES:
				break;
		}
		::std::cout << "{\t" << i->getVal() 
			<< ",\t" << typestr << "}\n";
	}

	::std::cout << "[INFO] Token End" << ::std::endl;
}

}
