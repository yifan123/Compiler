#pragma once
#include"SymTableItem.h"
#include<vector>
using namespace std;


class SymTable {
public:
	vector<SymTableItem>items;  //���ű����
	int offset;  //�����ڷ��ű��е�ƫ��

	
	int enter(string name, TabObj obj, TabType type, int adr, int number, int func_size = 0);
	int enter_declare(string name, TabObj obj, TabType type, int adr, int number, int func_size = 0);
	SymTable();
};