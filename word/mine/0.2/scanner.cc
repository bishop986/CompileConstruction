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

// 使用枚举类型约束状态量
enum STATE{
	START, INCOMMENT, INNUM, INID, INASSIGN, DONE
};

// 使用枚举类型约束token类型
enum TYPE{
	NUM, ID, ASSIGN, ERROR, RESERVED
};

// 符号表数据结构
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

	::std::printf("TINY COMPILATION: %s\n", argv[1]);
	::std::printf("\t%d: ", ++line);
	process_char = ::std::fgetc(fp);
	if ( EOF == process_char)
	{
		return 0;
	}

	// DFA 参考自 《编译原理及实践》中文版 p54
	// Done 状态集中处理本行所有token的保存工作，因此需要立即跳转
	// 考虑到书写习惯，换行符，空格，运算符都可能成为标识符的终止符
	// 使用状态转移量控制过程，没有使用转换表
	// 考虑到转换表也许还可能会用系数矩阵压缩方法
	// 线性匹配保留字，没有二分，没有构建杂凑表
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
						|| process_char == ')' || process_char == ';' || process_char == '=')
				{
					tmpTYPE = ASSIGN;
					state = DONE;
					tmpWORD += process_char;
					continue;
				} else 
				{
					tmpTYPE = ERROR;
					state = DONE;
					tmpWORD += process_char;
					continue;
				}
				break;
			case INCOMMENT:
				if ( '}' == process_char)
				{
					state = START;
				} else 
				{
					state = INCOMMENT;
				}
				break;
			case INNUM:
				if ( process_char >= '0' && process_char <= '9')
				{
					state = INNUM;
					tmpWORD += process_char;
				} else if ( ' ' == process_char || '\n' == process_char)
				{
					state = DONE;
					continue;
				} else if ( process_char == '+' || process_char == '-' || process_char == '*'
						|| process_char == '/' || process_char == '<' || process_char == '('
						|| process_char == ')' || process_char == ';' || process_char == '=')
				{
					state = DONE;

					::std::string str = "";
					str += process_char;
					wordtype.push_back(WORDTYPE(str, ASSIGN));
					continue;
				} else 
				{
					tmpTYPE = ERROR;
					state = DONE;
					tmpWORD += process_char;
					continue;
				}
				break;
			case INID:
				if ( (process_char >= 'a' && process_char <= 'z')
						|| (process_char >= 'A' && process_char <= 'Z'))
				{
					state = INID;
					tmpWORD += process_char;
				} else if ( ' ' == process_char || '\n' == process_char)
				{
					state = DONE;
					if ( tmpWORD == "if" || tmpWORD == "then" || tmpWORD == "else"
							|| tmpWORD == "end" || tmpWORD == "repeat" || tmpWORD == "until"
							|| tmpWORD == "until" || tmpWORD == "read" || tmpWORD == "write")
					{
						tmpTYPE = RESERVED;
					}
					continue;

				} else if ( process_char == '+' || process_char == '-' || process_char == '*'
						|| process_char == '/' || process_char == '<' || process_char == '('
						|| process_char == ')' || process_char == ';' || process_char == '=')
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
					continue;
				} else 
				{
					tmpTYPE = ERROR;
					state = DONE;
					tmpWORD += process_char;
					continue;
				}
				break;
			case INASSIGN:
				if ( '=' == process_char)
				{
					state = DONE;
					tmpWORD += process_char;
					continue;
				} else if ( ' ' == process_char || '\n' == process_char)
				{
					state = DONE;
					continue;
				} else if ( process_char == '+' || process_char == '-' || process_char == '*'
						|| process_char == '/' || process_char == '<' || process_char == '('
						|| process_char == ')' || process_char == ';' || process_char == '=')
				{
					state = DONE;

					::std::string str = "";
					str += process_char;
					wordtype.push_back(WORDTYPE(str, ASSIGN));
					continue;
				} else 
				{
					tmpTYPE = ERROR;
					state = DONE;
					tmpWORD += process_char;
					continue;
				}
				break;
			case DONE:
				wordtype.push_back(WORDTYPE( tmpWORD, tmpTYPE));
				tmpWORD = "";
				state = START;
		}

		if ( '\n' == process_char)
		{
			::std::printf("%c", process_char);
			if( wordtype.size() != 0)
			{
				for ( auto x : wordtype)
				{
					::std::printf("\t\t%d: ", line);
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

		process_char = ::std::fgetc(fp);
	}

	::std::fclose(fp);
	return 0;
}
