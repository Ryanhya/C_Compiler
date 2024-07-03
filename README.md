# C_Compiler
本项目只支持简单的C语言语法，目前只实现了词法分析模块与语法分析模块。其中语法分析基于LR1分析。（语义分析仅有雏形，并未完全实现）如果对你有帮助，给个star吧~
## 本项目支持的简单语法
1. **数据类型**：支持整型`int`，字符型`char`，布尔型`bool`。支持相同类型多个变量同时声明
2. **表达式**：包括基本四则运算、关系运算和逻辑运算以及自增自减运算（`++` 与 `--`）
3. **标识符**：标识符规则满足————只由字母和数字构成，且标识符第一位必须为字母，字母区分大小写，标识符不能与关键字产生冲突
4. **注释**：支持使用注释。包括单行注释`//`以及多行注释`/*   */`
5. **函数**：支持无返回值类型void函数调用，不支持其他过程函数调用。其中主程序为`void main(){<函数体>}`
6. **分支语句与循环语句**：支持`if(条件){<函数体>}else{<函数体>}`和`if(条件){ <函数体>}`的双路条件分支语句，条件分支语句支持内层多重嵌套。支持for循环语句和while循环语句，循环体也支持内层多重嵌套
7. **输入输出语句**：支持C语言的输入输出函数`scanf()`和`printf()`，函数的参数均为格式参数以及对应的标识符变量名

## 文法形式化定义
+ **文法**: G[S]={Vn，Vt，S，P}
+ **非终结符号集Vn**：{S  ,  Y  ,  O  ,  D  ,  C  ,  A  ,  B  ,  L  ,  T  ,  P  ,  Q  ,  E  ,  G  ,  I  ,  F  ,  J  , K  ,  H  ,  U  ,  V  ,  R}
+ **终结符号集Vt**：{id  ,  num  ,  character  ，void  ,  main   ， (  ,  )  ,  '  ,  ;  ,  ','  ,  {  ,  }  ,  +  ,  -  ,  *  ,  /  ,  ++  ,  --  ,  >  ,  <  ,  =  ,  >=  ,  <=  ,  ==  ,  !=  ,  &&  ,  ||  ,  !  ,  int  ,  char  ,  bool  ,  true  ,  false  ,  if  ,  else  ,  while  ,  for  ,  $}
其中id为标识符，num为数字，character为单个字符
+ **开始符号**：S (S')
+ **产生式集合P**：具体见source/grammar.txt
+ **关于产生式符号的一些说明**：大写字母为非终结符号，小写字母为终结符号。（O为其他，Y为语句体集合statement，D为声明语句，C为控制语句，A为赋值语句，B为条件分支语句，L为循环语句，T为变量类型，P和Q用于多个同类型变量同时声明，E为算术表达式，G为布尔表达式，F为因式，I为项，H为逻辑运算符，J和K用于布尔表达式计算，R表示for循环初始化有无声明类型，U为if分支，V为else分支或空）

## 程序流程

1. **词法分析流程**：
![avater](/figures/lexical_analysis.png)


2. **LR1分析表构造流程**：
![avater](/figures/syntax_analysis1.png)

3. **语法分析流程**：
![avater](/figures/syntax_analysis2.png)


## 示例输入程序
```
void main() {
    int a, b, d;
    char c;
    bool flag = false;
    c = 'c';
    if (c == 'c') {
        b = 10;
        d = b;
        /*a = b*/
        for (a = 0; a < b; a = a + 1) {
            if (d <= 5) {
                flag = true;
            }
            //d<=5 
            if (flag) {
                c = 'a';
            }
            d--;
        }        
    }
    else {
        c = 'b';
    }   
}
```

