#include"SymTable.h"
#include"error.h"
using namespace std;
extern int find_sym_table(string sym, SymTableItem& item, int &aglobal, int *symtbles_index = NULL);
extern int find_sym_table_declare(string sym, SymTableItem& item);

int SymTable::enter(string name, TabObj obj, TabType type, int adr, int number,int func_size) {
	SymTableItem tem;
	int gtem;
	if (find_sym_table(name,tem,gtem)>=0) {
		return -1;  //标识符重定义
	}
	items.push_back(SymTableItem(name, obj, type, adr, number,func_size));
	offset++;
	return 0;
}

int SymTable::enter_declare(string name, TabObj obj, TabType type, int adr, int number,int func_size) {
	SymTableItem tem;
	if (find_sym_table_declare(name, tem) >=0) {
		return -1;  //标识符重定义
	}
	items.push_back(SymTableItem(name, obj, type, adr, number, func_size));
	offset++;
	return 0;
}

SymTable::SymTable() {
	offset = 0;
};