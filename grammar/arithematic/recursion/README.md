# 简单整型算数表达式文法分析

### 简要说明

采用EBNF书写文法，使用递归下降分析法

- G(exp)
-	exp->term{ addop term}
-	addop->+|-
-	term->factor{ mulop factor}
-	mulop->*
-	factor->(exp)|number

number 是一个正规表达式名
number = [0-9][0-9]*
