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
---- + ---- - +---- + ---- + ---- + ----
.. | name | obj | type | adr | number | 
---- + ---- - +---- + ---- + ---- + ----
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

	SymTableItem(){}
	SymTableItem(string _name, TabObj _obj, TabType _type, int _adr, int _number, int _func_size=0) {
		name = _name;
		obj = _obj;
		type = _type;
		adr = _adr;
		number = _number;
		func_size = _func_size;
		//subsymtable_index = _subsymtable_index;
	}
};