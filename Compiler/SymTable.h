#pragma once
#include"SymTableItem.h"
#include<vector>
using namespace std;


class SymTable {
public:
	vector<SymTableItem>items;  //符号表表项
	int offset;  //符号在符号表中的偏移

	
	int enter(string name, TabObj obj, TabType type, int adr, int number, int func_size = 0);
	int enter_declare(string name, TabObj obj, TabType type, int adr, int number, int func_size = 0);
	SymTable();
};