/*
 * Time: 2018.4.19
 * Author: bishop986
 * Instruction:
 * 简单词法分析，匹配[a-zA-Z_]([a-zA-Z_]|[0-9a-zA-Z_])*
 */

#include <iostream>
#include <string>
#include <vector>
#include <cstdio>

int main( int argc, char** argv)
{
	// 图方便直接用了容器
	// 据说流操作比较慢，所以用了文件指针，不过我觉得小文件应该感觉不出来
	::std::FILE *fp = NULL;
	::std::string tmp_Identifier = "";
	// 统计正确的标识符
	::std::vector< ::std::string > right_identifiers;
	// 转移状态存储
	unsigned int process_state = 0;
	// 待处理字符
	char process_char = ' ';

	if ( argc < 2)
	{
		::std::cerr << "\e[1;31m[ERROR] Need a File Path\e[0m" << ::std::endl;
		return 1;
	}

	fp = ::std::fopen( argv[1], "r+");
	if ( NULL == fp)
	{
		::std::cerr << "\e[1;31m[ERROR] No Such File\e[0m" << ::std::endl;
		return 1;
	}

	// 0状态为s0，1状态为s1，2状态为终止态s2
	// 转换函数如下:
	// T(s0,EOF)=s2;T(s1,EOF)=s2
	// T(s0,[a-zA-Z_])=s1;T(s0,^[a-zA-Z_])=s0
	// T(s1,[0-9a-zA-Z_])=s1;T(s0,^[0-9a-zA-Z_])=s0
	while( process_state != 2) 
	{
		switch( process_state)
		{
			case 0:
				process_char = ::std::fgetc(fp);
				if ( EOF == process_char)
				{
					process_state = 2;
				} else if ( (process_char >= 'A' && process_char <= 'Z') || process_char == '_'
						|| (process_char >= 'a' && process_char <= 'z'))
				{
					process_state = 1;
					tmp_Identifier += process_char;
				} else 
				{
					process_state = 0;
				}
				break;
			case 1:
				process_char = ::std::fgetc(fp);
				if ( EOF == process_char)
				{
					process_state = 2;
				} else if ( (process_char >= 'A' && process_char <= 'Z') || process_char == '_' 
						|| (process_char >= '0' && process_char <= '9')
						|| (process_char >= 'a' && process_char <= 'z'))

				{
					process_state = 1;
					tmp_Identifier += process_char;
				} else 
				{
					process_state = 0;
					right_identifiers.push_back(tmp_Identifier);
					tmp_Identifier.clear();
				}
				break;
			case 2:
				break;
		}
	}

	::std::cout << "\e[1;34m[RESULT] Matching as following:\n\e[0m";
	for ( auto i : right_identifiers)
	{
		::std::cout << i << ::std::endl;
	}

	::std::fclose(fp);
	return 0;
}

