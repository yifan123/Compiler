#pragma once
#include<string>
#include"token.h"
using namespace std;

typedef enum {
	constantobj, varobj, arrayobj,funcobj
}TabObj;
typedef enum {
	inttype, chartype, voidtype
}TabType;
/*
Symbol table :
---- + ---- - +---- + ---- + ---- + ---- + ---- + ---- + ----
.. | name | obj | type | adr | number | func_size | reg
---- + ---- - +---- + ---- + ---- + ---- + ---- + ---- + ----
| < ----- one single identifier ---- > |
*/
class SymTableItem {
public:
	string name;  //符号的唯一标识
	TabObj obj;   //符号所属种类
	TabType type; //符号的数据类型
	int adr;      
	/*adr取值：
	  常数				value
      普通变量,数组变量	在运行栈中的相对地址
      函数				子符号表索引，指向该函数的子符号表
	*/
	int number; //数组或参数表中元素的个数
	int func_size; //函数运行栈大小
	int reg; //记录变量在哪个寄存器中，如果不在寄存器中，值为-1
	bool is_adr; //变量是否在内存中，可以理解为dirty位，代表寄存器和内存中的值是否一致

	SymTableItem(){}
	SymTableItem(string _name, TabObj _obj, TabType _type, int _adr, int _number, int _func_size=0, int _reg = -1) {
		name = _name;
		obj = _obj;
		type = _type;
		adr = _adr;
		number = _number;
		func_size = _func_size;
		reg = _reg;
		//subsymtable_index = _subsymtable_index;
	}
};