/*
 * Time: 2018.4.22
 * Author: bishop986
 * Instruction:
 * 编程练习 2.20，测试用例名为test.dat
 */

#include <stdio.h>

int main( int argc, char** argv)
{
	FILE *fp = NULL;
	char process_char = ' ';
	int state = 0;

	if ( argc < 2)
	{
		printf("\e[1;31m[INFO] Usage: main target.c\e[0m\n");
		return 1;
	}

	// 用读写方式打开，在原文件的基础上修改文件
	fp = fopen( argv[1], "r+");
	if ( NULL == fp)
	{
		perror(argv[1]);
		return 0;
	}

	// 将单行注释和多行注释分情况讨论
	// 单行注释用换行符结束
	// 多行注释用*/结束
	// 注意'//'与'/*'的正则表达式为连接
	process_char = fgetc(fp);
	while( EOF != process_char)
	{
		switch( state)
		{
			case 0:
				if ( '/' == process_char)
				{
					state = 1;
				}
				break;
			case 1:
				if ( '/' == process_char)
				{
					state = 2;
				} else if ( '*' == process_char)
				{
					state = 3;
				} else 
				{
					state = 1;
				}
				break;
			case 2:
				if ( '\n' == process_char)
				{
					state = 0;
				} else 
				{
					if (process_char >= 'a' && process_char <= 'z')
					{
						process_char -= 32;
						// 在文件基础上原地修改
						fseek( fp, -1, SEEK_CUR);
						fputc( process_char, fp);
					}
				}
				break;
			case 3:
				if ( '*' == process_char)
				{
					state = 4;
				} else 
				{	
					if ( process_char >= 'a' && process_char <= 'z')
					{
						process_char -= 32;
						fseek( fp, -1, SEEK_CUR);
						fputc( process_char, fp);
					}

				}
				break;
			case 4:
				if ( '/' == process_char)
				{
					state = 0;
				} else 
				{
					if ( process_char >= 'a' && process_char <= 'z')
					{
						process_char -= 32;
						fseek( fp, -1, SEEK_CUR);
						fputc( process_char, fp);
					}
					state = 3;
				}
				break;
		}
		process_char = fgetc(fp);
	}

	fclose(fp);
	printf("\e[1;32m[INFO] Done\e[0m\n");

	return 0;
}

