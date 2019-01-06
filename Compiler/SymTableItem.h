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
	int reg; //��¼�������ĸ��Ĵ����У�������ڼĴ����У�ֵΪ-1
	bool is_adr; //�����Ƿ����ڴ��У��������Ϊdirtyλ������Ĵ������ڴ��е�ֵ�Ƿ�һ��

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