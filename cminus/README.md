# C Minus(c-)

拥有简化c语法的类c语言

样本程序放在本目录下叫做"minus.c"

### 待办列表

- [X] 词法分析器
- [ ] 语法分析器
- [ ] 语义分析
- [ ] 类型检查
- [ ] 中间代码生成
- [ ] 目标代码生成

### C Minus 词法规则

1. 关键字: if, else, int, return, void, while

2. 专用符号: + - * / > >= <= > == != = . ( ) [ ] { } /* */ ;

3. 正规表达式:
	- ID = [a-zA-Z]+
	- NUM = [0-9]+

4. 空白符包括空格，换行，制表符

5. 注释由/*...*/围起来

### C Minus 语法分析器

TODO