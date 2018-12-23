#pragma once
#include<iostream>
using namespace std;
//添加error_code的定义
//语义错误
#define	UNDEF_ID			    28		//未声明标识符
#define	MULTI_ID_DEF		    29		//重定义标识符
#define FUNC_RPARA_TYPE_ERROR   30      //函数调用时，传进的实参类型不匹配
#define FUNC_RPARA_NUM_ERROR    31      //函数调用时，传进的实参个数不匹配
#define VAR_TYPE_ERROR		    32		//变量类型错误，变量只允许是int或者char类型
#define OPRAND_TYPE_ERROR       33      //运算数类型只允许是int或者char类型
#define ARRAY_INDEX_TYPE_ERROR  34      //数组下标类型错误，必须为整型
#define	FUNC_NO_RET			    35		//无返回值函数不能在表达式中使用
#define VOID_RETURN_ERROR	    36		//void函数中return非空值
#define NON_REACH_FUNC		    37
#define	MISSING_RETURN		    38	    //丢失return语句
#define	OUT_OF_ARRAY		    39		//数组下标越界
#define RETURN_TYPE_ERROR       40      //函数返回值类型错误
#define ASS_TYPE_ERROR		    41      //赋值语句前后类型不一样
#define SWITCH_TYPE_ERROR	    42      //switch和case类型不一样
#define	CONDITION_TYPE_ERROR    43      //条件中的关系比较只能是整形之间，不能是字符型；如果条件是单个表达式，则必须是整型
#define	ASSIGN_ERROR		    44	    //赋值时发生错误

//语法错误
//因为刚开始写parser.cpp时，没用宏来表示错误类型，直接硬编码，所以就把这里注释掉了

//#define     0      //主函数返回值应为void型
//#define     1      //main函数应定义在程序末尾
//#define     2      //丢失main函数
//#define     3      //应是'('
//#define     4      //应是')'
//#define     5      //应是'{'
//#define     6      //应是'}'
//#define     7      //应是';'
//#define     8      //缺少CONST
//#define     9      //应是标识符
//#define     10     //应是=	
//#define     11     //应是字符	
//#define     12     //缺少类型标识符(int char)	
//#define     13     //应是无符号整数	
//#define     14     //应是']'	
//#define     15     //缺少函数类型标识符(int char void)	
//#define     16     //应为语句	
//#define     17     //应为if	
//#define     18     //应为while	
//#define     19     //应为scanf	
//#define     20     //应为printf	
//#define     21     //应为switch	
//#define     22     //应为case	
//#define     23     //应是':'	
//#define     24     //应为return	
//#define     25     //应是因子	
//#define     26     //应是字符串	
//#define     27     //未定义语法	


//词法分析时的错误
//词法分析遇到错误不会报，会接着往下读，直到读到一个合法的词法或成分

//一些之前写的但现在没用了的错误类型
//#define	ID_TOO_LONG			3		//标识符过长
//#define	PROGRAM_ERROR		4		//程序结构错误
//#define	STR_CONTENT_ERROR	5		//字符串内容错误
//#define	STR_TOO_LONG		6		//字符串超长
//#define	ZERO_HEAD_NUM		7		//非零数字以0开头
//#define	NUM_HEAD_IDENT		8		//数字开头的标识符
//#define	NUM_TOO_LARGE		9		//数字太大
////#define	UNDEF_INPUT			10		//不合法输入	
//#define	CHAR_MISS_QUOTE		11		//字符类型丢失后一个单引号'
//#define	CHAR_CONTENT_ERROR	12		//字符内容不合法
//#define	CHAR_OVERFLOW		13		//字符溢出	
//#define	NEQUAL_MISS			14		//错误使用不等号，只有！没有等号
//#define	MISSING_SEMI		15		//丢失分号
//#define	MISSING_RPARENT		16		//丢失右括号
//#define	MISSING_RBRACK		17		//丢失右中括号
//#define	MISSING_RBRACE		18		//丢失右大括号
//#define	MISSING_IDENT		19		//丢失标识符
//#define	ERROR_PARA_NUM		20		//参数数目错误
//#define	MISSING_LPARENT		21		//丢失左括号
//#define	MISSING_LBRACK		22		//丢失左中括号
//#define	MISSING_LBRACE		23		//丢失左大括号
//#define	MISSING_PLUS		24		//丢失加号或减号
//#define	MISSING_MULTI		25		//丢失乘号或除号
//
//#define	RETURN_ERROR		27		//返回值错误
//#define	MISSING_MAIN		28		//丢失main函数
//#define	EXPRESSION_ERROR	30		//表达式错误
////#define	CONST_NOT_INIT		31		//常量使用前未初始化
////#define	VAR_NOT_INIT		32		//变量使用前未初始化
//
//#define	OUT_OF_TABLE		34		//符号表满了
//#define	WRONG_TYPE			36		//声明类型错误
//#define	MISSING_ASSIGN		37		//丢失等号
//#define MISSING_COLON		38		//丢失冒号
//#define MISSING_VOID		39		//丢失VOID
//#define MISSING_DOUQUOTE	40		//丢失双引号
//#define CASE_TYPE_ERROR		41		//case的声明出错,标识符类型不正确
//#define CONST_TYPE_ERROR	42		//常量类型错误,常量只允许是int或者char类型
//#define FACTOR_ERROR		43		//因子发生错误，请检查是否错误使用运算符
//#define SCANF_ERROR			44		//SCANF使用错误
//
//#define UNDEF_FUNC			46		//使用了未定义的函数
//
//#define ARRAY_LENGTH_ERROR	48		//数组变量声明的时候发生错误
//
////不能够达到的错误分支
//#define NON_REACH_IF		100		
//#define NON_REACH_WHILE		101		
//#define NON_REACH_SWITCH	102
//#define NON_REACH_CASE		103
//#define NON_REACH_SCANF		104
//#define NON_REACH_PRINTF	105
//#define NON_REACH_RETURN	106
//#define NON_REACH_CON		108

void error();//在lexer.cpp中使用
void error(int line, int error_code);//在parser.cpp中使用，报被编译程序语法上的错误
void skip();
void test();