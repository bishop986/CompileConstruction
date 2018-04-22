/* 
 * Time: 2018.4.22
 * Author: bishop986
 * Instruction:
 * Get Tokens by TINY DFA
 */

#include <cstdio>
#include <iostream>
#include <string>
#include <vector>

enum STATE{
	START, INCOMMENT, INNUM, INID, INASSIGN, DONE
};

enum TYPE{
	NUM, ID, ASSIGN, ERROR, RESERVED
};

struct WORDTYPE{

	WORDTYPE(){}

	WORDTYPE( ::std::string val, TYPE type)
	{
		this->val = val;
		this->type = type;
	}

	::std::string val;
	TYPE type;

};

int main( int argc, char** argv)
{
	int line = 0;
	STATE state = START;
	char process_char = ' ';
	::std::FILE *fp = NULL;
	::std::vector< ::WORDTYPE > wordtype;
	::std::string tmpWORD = "";
	TYPE tmpTYPE = ID;

	if ( argc < 2)
	{
		::std::printf("[ERROR] Usage: Scanner target.tny\n");
		return 1;
	}

	fp = ::std::fopen( argv[1], "r");
	if ( NULL == fp)
	{
		::std::perror( argv[1]);
		return 1;
	}

	process_char = ::std::fgetc(fp);
	if ( EOF == process_char)
	{
		return 0;
	}
	::std::printf("\t%d: %c", ++line, process_char);
	if ( process_char == '\n')
	{
		::std::printf("\t%d: ", ++line);
	}

	while( EOF != process_char)
	{
		switch(state)
		{
			case START:
				if ( '\t' == process_char || ' ' == process_char || '\n' == process_char)
				{
					state = START;
				} else if ( '{' == process_char)
				{
					state = INCOMMENT;
				} else if ( process_char >= '0' && process_char <= '9')
				{
					tmpTYPE = NUM;
					state = INNUM;
					tmpWORD += process_char;
				} else if ( (process_char >= 'a' && process_char <= 'z') 
						|| (process_char >= 'A' && process_char <= 'Z')
						|| process_char == '_')
				{
					tmpTYPE = ID;
					state = INID;
					tmpWORD += process_char;
				} else if ( process_char == ':')
				{
					tmpTYPE = ASSIGN;
					state = INASSIGN;
					tmpWORD += process_char;
				} else if ( process_char == '+' || process_char == '-' || process_char == '*'
						|| process_char == '/' || process_char == '<' || process_char == '('
						|| process_char == ')' || process_char == ';')
				{
					tmpTYPE = ASSIGN;
					state = DONE;
					tmpWORD += process_char;
				} else 
				{
					tmpTYPE = ERROR;
					state = DONE;
					tmpWORD += process_char;
				}
				break;
			case INCOMMENT:
				if ( '}' == process_char)
				{
					state = START;
				}
				break;
			case INNUM:
				if ( process_char >= '0' && process_char < '9')
				{
					state = INNUM;
					tmpWORD += process_char;
				} else if ( process_char == '+' || process_char == '-' || process_char == '*'
						|| process_char == '/' || process_char == '<' || process_char == '('
						|| process_char == ')' || process_char == ';')
				{
					state = DONE;

					::std::string str = "";
					str += process_char;
					wordtype.push_back(WORDTYPE(str, ASSIGN));
				} else 
				{
					tmpTYPE = ERROR;
					state = DONE;
					tmpWORD += process_char;
				}
				break;
			case INID:
				if ( (process_char >= 'a' && process_char <= 'z')
						|| (process_char >= 'A' && process_char <= 'Z'))
				{
					state = INID;
					tmpWORD += process_char;
				} else if ( process_char == '+' || process_char == '-' || process_char == '*'
						|| process_char == '/' || process_char == '<' || process_char == '('
						|| process_char == ')' || process_char == ';')
				{
					state = DONE;

					if ( tmpWORD == "if" || tmpWORD == "then" || tmpWORD == "else"
							|| tmpWORD == "end" || tmpWORD == "repeat" || tmpWORD == "until"
							|| tmpWORD == "until" || tmpWORD == "read" || tmpWORD == "write")
					{
						tmpTYPE = RESERVED;
					}

					::std::string str = "";
					str += process_char;
					wordtype.push_back(WORDTYPE(str, ASSIGN));
				} else 
				{
					tmpTYPE = ERROR;
					state = DONE;
					tmpWORD += process_char;
				}
				break;
			case INASSIGN:
				if ( '=' == process_char)
				{
					state = DONE;
					tmpWORD += process_char;
				} else if ( process_char == '+' || process_char == '-' || process_char == '*'
						|| process_char == '/' || process_char == '<' || process_char == '('
						|| process_char == ')' || process_char == ';')
				{
					state = DONE;

					::std::string str = "";
					str += process_char;
					wordtype.push_back(WORDTYPE(str, ASSIGN));
				} else 
				{
					tmpTYPE = ERROR;
					state = DONE;
					tmpWORD += process_char;
				}
				break;
			case DONE:
				wordtype.push_back(WORDTYPE( tmpWORD, tmpTYPE));
				tmpWORD = "";
				break;
		}

		process_char = ::std::fgetc(fp);
		if ( '\n' == process_char)
		{
			if( wordtype.size() != 0)
			{
				for ( auto x : wordtype)
				{
					::std::printf("\t\t%d: ", ++line);
					if ( x.type == ERROR)
					{
						::std::printf("error word: %s\n", x.val.c_str());
					} else if ( x.type == ASSIGN)
					{
						::std::printf("assign signal: %s\n", x.val.c_str());
					} else if ( x.type == NUM)
					{
						::std::printf("NUM, val= %s\n", x.val.c_str());
					} else if ( x.type == ID)
					{
						::std::printf("ID, name= %s\n", x.val.c_str());
					} else if ( x.type == RESERVED)
					{
						::std::printf("reserved word: %s\n", x.val.c_str());
					}
				}
				wordtype.clear();
			}
			::std::printf("\t%d: ", ++line);
		} else 
		{
			::std::printf("%c", process_char);
		}
	}

	::std::fclose(fp);
	return 0;
}
