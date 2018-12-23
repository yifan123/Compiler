#include"error.h"
extern int error_num;
void error() {

}
void error(int line, int error_code) {
	error_num++;
	cout << "at line "<< line << ": error_code: "<< error_code<<endl;
	switch (error_code) {
	case 0: cout << "主函数返回值应为void型" << endl;	break;
	case 1: cout << "main函数应定义在程序末尾" << endl;	break;
	case 2: cout << "丢失main函数" << endl;	break;
	case 3: cout << "丢失'('" << endl;	break;
	case 4: cout << "丢失')'" << endl;	break;
	case 5: cout << "丢失'{'" << endl;	break;
	case 6: cout << "丢失'}'" << endl;	break;
	case 7: cout << "丢失';'" << endl;	break;
	case 8: cout << "缺少CONST" << endl;	break;
	case 9: cout << "应是标识符" << endl;	break;
	case 10: cout << "应是=	" << endl;	break;
	case 11: cout << "应是字符	" << endl;	break;
	case 12: cout << "缺少类型标识符(int char)	" << endl;	break;
	case 13: cout << "应是无符号整数	" << endl;	break;
	case 14: cout << "丢失']'	" << endl;	break;
	case 15: cout << "缺少函数类型标识符(int char void)	" << endl;	break;
	case 16: cout << "应为语句	" << endl;	break;
	case 17: cout << "应为if	" << endl;	break;
	case 18: cout << "应为while	" << endl;	break;
	case 19: cout << "应为scanf	" << endl;	break;
	case 20: cout << "应为printf	" << endl;	break;
	case 21: cout << "应为switch	" << endl;	break;
	case 22: cout << "应为case	" << endl;	break;
	case 23: cout << "丢失':'	" << endl;	break;
	case 24: cout << "应为return	" << endl;	break;
	case 25: cout << "应是因子	" << endl;	break;
	case 26: cout << "应是字符串	" << endl;	break;
	case 27: cout << "未定义语法	" << endl;	break;
	case UNDEF_ID:cout << "未声明标识符" << endl;	break;
	case MULTI_ID_DEF:cout << "重定义标识符" << endl;	break;
	case FUNC_RPARA_TYPE_ERROR:cout << "函数调用时，传进的实参类型不匹配" << endl;	break;
	case FUNC_RPARA_NUM_ERROR:cout << "函数调用时，传进的实参个数不匹配" << endl;	break;
	case VAR_TYPE_ERROR:cout << "变量类型错误，变量只允许是int或者char类型" << endl;	break;
	case OPRAND_TYPE_ERROR:cout << "运算数类型只允许是int或者char类型" << endl;	break;
	case ARRAY_INDEX_TYPE_ERROR:cout << "数组下标类型错误，必须为整型" << endl;	break;
	case FUNC_NO_RET:cout << "无返回值函数不能在表达式中使用" << endl;	break;
	case VOID_RETURN_ERROR:cout << "void函数中return非空值" << endl;	break;
	case NON_REACH_FUNC:cout << "该分支不会到达" << endl;	break;
	case MISSING_RETURN:cout << "丢失return语句" << endl;	break;
	case OUT_OF_ARRAY:cout << "数组下标越界" << endl;	break;
	case RETURN_TYPE_ERROR:cout << "函数返回值类型错误" << endl;	break;
	case ASS_TYPE_ERROR:cout << "赋值语句前后类型不一样" << endl;	break;
	case SWITCH_TYPE_ERROR:cout << "switch和case类型不一样" << endl;	break;
	case CONDITION_TYPE_ERROR:cout << "条件中的关系比较只能是整形之间，不能是字符型；如果条件是单个表达式，则必须是整型" << endl;	break;
	case ASSIGN_ERROR:cout << "赋值时发生错误" << endl;	break;
	default:cout << "未知错误" << endl;
	}
}
void skip() {

}
void test() {

}