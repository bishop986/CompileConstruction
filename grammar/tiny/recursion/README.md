# 开发日志

### 待办事项

1. [x] 完成符号表元素设计编写
2. [x] 完成词法分析器设计编写生成符号表
3. [x] 完成文法分析器设计编写生成语法树
4. [x] 完成语义分析器设计编写
5. [x] 完成中间代码生成
6. [X] 完成目标代码生成

### 推导

##### Tiny 词法DFA
```
other=[^+-*/<=]
T1( START, { )=INCOMMENT 
T2( START, [0-9])=INNUMBER
T3( START, [a-zA-Z])=INID
T4( START, :)=INASSGIN
T5( START, [+-*/<=other])=DONE
T6( START, [' ''\t''\n''\r'])=START
T7( INNUMBER, [0-9])=INNUMBER
T8( INNUMBER, [^0-9])=DONE
T9( INID, [a-zA-Z])=INID
T10( INID, [^a-zA-Z])=DONE
T11( INCOMMENT, })=START
T12( INCOMMENT, ^})=INCOMMENT
T13( INCOMMENT, =)=DONE
T14( INCOMMENT, ^=)=DONE
```

### Tiny EBNF
program --> stmt-sequence
stmt-sequence-> statement{;statement}
statement --> if-stmt|repeat-stmt|assgin-stmt|read-stmt|write-stmt
if-stmt --> **if** exp **then** stmt-sequence [ **else** stmt-sequence ] **end**
repeat-stmt --> **repeat** stmt-sequence **until** exp
assgin-stmt --> **identifier** := exp
read-stmt --> **read** **identifier**
write-stmt --> **write** exp
exp --> simple-exp [comparison-op simple-exp]
comparison-op --> < | =
simple-exp --> term { addop term}
addop --> + | -
term --> factor { mulop factor}
mulop --> * | /
factor --> ( exp ) | **number** | **identifier**

