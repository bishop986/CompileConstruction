/*
 * Time: 2018.4.19
 * Author: bishop986
 * Instruction:
 * 简单词法分析，用于统计符合要求的正确标识符的数量
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
	// 统计错误的标识符
	::std::vector< ::std::string > wrong_identifiers;
	bool error_flag = false;
	// 转移状态存储
	unsigned int process_state = 0;
	// 待处理字符
	char process_char = ' ';

	::std::cout << "[INFO] Calculation Begin" << ::std::endl;

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

	// 目前还没有包含辅助字符如+,=等
	// 0状态为s0，1状态为s1，2状态为终止态s2
	// 转换函数如下:
	// T(s0,EOF)=s2;T(s1,EOF)=s2
	// T(s0,[a-zA-Z])=s1;T(s0,[' '|'\t'|'\n'])=s0;T(s0,除开前面的情况)=不变
	// T(s1,[0-9a-zA-Z])=s1;T(s1,[' '|'\t'|'\n'])=s0;T(s1,除开前面的情况)=不变
	while( process_state != 2) 
	{
		switch( process_state)
		{
			case 0:
				process_char = ::std::getc(fp);
				if ( EOF == process_char)
				{
					process_state = 2;
				} else if ( (process_char >= 'A' && process_char <= 'z') || process_char == '_')
				{
					process_state = 1;
					tmp_Identifier += process_char;
				} else if ( process_char == ' ' || process_char == '\t' || process_char == '\n')
				{
					process_state = 0;
				} else 
				{
					error_flag = true;
					tmp_Identifier += process_char;
				}
				break;
			case 1:
				process_char = ::std::getc(fp);
				if ( EOF == process_char)
				{
					process_state = 2;
				} else if ( (process_char >= 'A' && process_char <= 'z') || process_char == '_' ||
						(process_char >= '0' && process_char <= '9'))
				{
					process_state = 1;
					tmp_Identifier += process_char;
				} else if ( process_char == ' ' || process_char == '\t' || process_char == '\n')
				{
					process_state = 0;
					if ( !error_flag )
					{
						right_identifiers.push_back(tmp_Identifier);
					} else 
					{
						wrong_identifiers.push_back(tmp_Identifier);
					}
					tmp_Identifier = "";
					error_flag = false;
				} else 
				{
					error_flag = true;
					tmp_Identifier += process_char;
				}
				break;
			case 2:
				break;
		}
	}

	::std::cout << "\e[1;32m[INFO] Calculation Finish\e[0m" << ::std::endl;

	::std::cout << "\e[1;32m[INFO] There's " << right_identifiers.size() << " leagal identifiers\e[0m" << ::std::endl;
	::std::cout << "\e[1;31m[INFO] There's " << wrong_identifiers.size() << " illeagal identifiers\e[0m" << ::std::endl;

	::std::cout << ::std::endl;

	::std::cout << "\e[1;34m[RESULT] leagal as following:\n\e[0m";
	for ( auto i : right_identifiers)
	{
		::std::cout << i << ::std::endl;
	}

	::std::cout << ::std::endl;

	::std::cout << "\e[1;34m[RESULT] illeagal as following:\n\e[0m";
	for ( auto i : wrong_identifiers)
	{
		::std::cout << i << ::std::endl;
	}

	::fclose(fp);
	return 0;
}

