# C Minus(c-)

拥有简化c语法的类c语言

样本程序放在本目录下叫做"minus.c"

### 待办列表

- [X] 词法分析器	完成: 2018.5.20
- [ ] 语法分析器
- [ ] 语义分析
- [ ] 类型检查
- [ ] 中间代码生成
- [ ] 目标代码生成

### C Minus 词法规则 完成: 2018.5.20

1. 关键字: if, else, int, return, void, while

2. 专用符号: + - * / > >= <= > == != = . ( ) [ ] { } /* */ ;

3. 正规表达式:
	- ID = [a-zA-Z]+
	- NUM = [0-9]+

4. 空白符包括空格，换行，制表符

5. 注释由/*...*/围起来

### C Minus 语法分析器 开始: 2018.5.20

program --> declaration_list

declaration_list --> declaration{declaration}

declaration --> var_declaration | fun_declaration

var_declaration --> type_specifier **ID** | type_specifier **ID**[**NUM**]

fun_declaration --> type_specifier **ID** ( params ) compound_stmt

params --> param_list | **VOID**

param_list --> param {,param}

param -> type_specifier **ID** | type_specifier **ID** []

compound_stmt --> { loacl_declarations statement_list }

local_declarations --> { var_declaration }

statement_list --> { statement }

statement --> expression_stmt | compound_stmt | if_stmt | while_stmt | return_stmt

expression_stmt --> expression; | ;

if_stmt --> **IF**(expression)statement[**ELSE** statement]

while_stmt --> **WHILE**(expression) statement

return_stmt --> **RETURN** | **RETURN** expression

expression --> var = expressoin|simple_expression

var --> **ID** | **ID**[expression]

simple_expression --> additive_expression [relop additive_expression]

additive_expression --> term { addop term}

addop --> + | -

term --> term { mulop factor }

mulop --> * | /

factor --> (expression) | var | call | **NUM**

call -> ID(args)

args --> args_list | empty

args_list --> expression { , expression}
