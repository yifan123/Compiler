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
	string name;  //���ŵ�Ψһ��ʶ
	TabObj obj;   //������������
	TabType type; //���ŵ���������
	int adr;      
	/*adrȡֵ��
	  ����				value
      ��ͨ����,�������	������ջ�е���Ե�ַ
      ����				�ӷ��ű�������ָ��ú������ӷ��ű�
	*/
	int number; //������������Ԫ�صĸ���
	int func_size; //��������ջ��С

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