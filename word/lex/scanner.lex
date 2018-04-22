%{
#include <stdio.h>
int wordCount = 0;
%}
letter [A-Za-z_]
digit [0-9]
identifier {letter}({letter}|{digit})*
%%

{identifier} {++wordCount;}
%%

int main(int argc, char** argv)
{
	yyin = fopen(argv[1],"r");
	yylex();
	printf("Words Number is: %d\n", wordCount);
	fclose(yyin);
	return 0;
}

int yywrap()
{
	return 1;
}


