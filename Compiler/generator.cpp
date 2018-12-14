#include "generator.h"
#include<vector>
#include <map>
#include<set>
#include <string> 
#include<iostream>
#include"SymTableItem.h"
#include"SymTable.h"
using namespace std;
extern map<int, string>stringtab;
extern int find_sym_table(string sym, SymTableItem& item, int &aglobal, int *symtbles_index = NULL);
extern vector<SymTable>symtables;
string cur_fun;
extern int cur_fun_symtab;
extern int global;
void Generator::mem_instr(string instr, string reg, SymTableItem symitem, int aglobal) { //Ϊ�˷ֱ�ȫ�ֺ;ֲ������������ͱ������ر�д���������
	if (symitem.obj == constantobj) {   //������ȫ�ֻ�ֲ�������Ҳ��������ʱ����
		mipsfile << "li " << reg << "," << symitem.adr << endl;
	}
	else {
		if (aglobal==1) {
			mipsfile << instr <<" "<< reg << "," << symitem.adr << "($gp)" << endl;
		}
		else {
			mipsfile << instr << " " <<reg << "," << symitem.adr << "($sp)" << endl;
		}
	}
}
void Generator::quad2mips(vector<QuadRuple> quadtable) {
	map<int, string>::iterator iter;
	SymTableItem symitem;
	//��Դ�����е������ַ���ȫ������.data��
	mipsfile << ".data" << endl;
	mipsfile << "enter: .asciiz \"\\n\"" << endl;
	for (iter = stringtab.begin(); iter != stringtab.end(); iter++) {
		mipsfile << "string" + to_string(iter->first) + ": .asciiz\"" + iter->second + "\"" << endl;
	}
	mipsfile << ".text" << endl;

	find_sym_table("@program", symitem, global);
	int func_size = symitem.func_size;
	mipsfile << "add $gp, $sp, $0" << endl;
	mipsfile << "subi $sp, $sp," << func_size << endl;	//����ȫ�ֱ����ռ�
	mipsfile << "jal main" << endl;
	for (int i = 0; i < quadtable.size(); i++) {
		quadruple2mips(quadtable[i]);
	}
}
void Generator::quadruple2mips(QuadRuple quad) {
	if (quad.op == "ADD" | quad.op == "SUB" | quad.op == "MUL" | quad.op == "DIV") {
		string arg1 = quad.arg1;
		string arg2 = quad.arg2;
		string result = quad.result;
		SymTableItem symitem1;
		SymTableItem symitem2;
		SymTableItem symitem3;
		int g1;
		int g2;
		int g3;
		find_sym_table(arg1, symitem1,g1);
		find_sym_table(arg2, symitem2,g2);
		find_sym_table(result, symitem3,g3);
		mem_instr("lw", "$t0", symitem1, g1);
		mem_instr("lw", "$t1", symitem2, g2);
		if (quad.op == "ADD") {	//�ӷ�
			mipsfile << "add $t2,$t0,$t1" << endl;
		}
		else if (quad.op == "SUB") { //����
			mipsfile << "sub $t2,$t0,$t1" << endl;
		}
		else if (quad.op == "MUL") {	//�˷�
			mipsfile << "mult $t0,$t1" << endl;
			mipsfile << "mflo $t2" << endl;
		}
		else if (quad.op == "DIV") {	//�˷�
			mipsfile << "div $t0,$t1" << endl;
			mipsfile << "mflo $t2" << endl;
		}
		mem_instr("sw", "$t2", symitem3, g3);
	}
	else if (quad.op == "NEG") {
		string arg1 = quad.arg1;
		string result = quad.result;
		SymTableItem symitem1;
		SymTableItem symitem3;
		int g1,g3;
		find_sym_table(arg1, symitem1,g1);
		find_sym_table(result, symitem3,g3);
		mem_instr("lw", "$t0", symitem1, g1);
		mipsfile << "sub $t2,$0,$t0" << endl;
		mem_instr("sw", "$t2", symitem3, g3);
	}
	else if (quad.op == "ASS") {
		string arg1 = quad.arg1;
		string result = quad.result;
		SymTableItem symitem1;
		SymTableItem symitem3;
		int g1, g3;
		find_sym_table(arg1, symitem1, g1);
		find_sym_table(result, symitem3, g3);
		mem_instr("lw", "$t0", symitem1, g1);
		mem_instr("sw", "$t0", symitem3, g3);
	}
	else if (quad.op == "LSS" | quad.op == "LEQ" | quad.op == "GTR" | quad.op == "GEQ" | quad.op == "NEQ" | quad.op == "EQL") {
		string arg1 = quad.arg1;
		string arg2 = quad.arg2;
		string label = quad.result;
		SymTableItem symitem1;
		SymTableItem symitem2;
		int g1, g2;
		find_sym_table(arg1, symitem1,g1);
		find_sym_table(arg2, symitem2,g2);
		mem_instr("lw", "$t0", symitem1, g1);
		if (arg2 != "$0") {
			mem_instr("lw", "$t1", symitem2, g2);
		}
		else {
			mipsfile << "add $t1,$0,$0" << endl;
		}
		if (quad.op == "LSS") {
			mipsfile << "blt $t0,$t1," << label << endl;
		}
		else if (quad.op == "LEQ") {
			mipsfile << "ble $t0,$t1," << label << endl;
		}
		else if (quad.op == "GTR") {
			mipsfile << "bgt $t0,$t1," << label << endl;
		}
		else	if (quad.op == "GEQ") {
			mipsfile << "bge $t0,$t1," << label << endl;
		}
		else if (quad.op == "NEQ") {
			if (arg2 == "$0") {
				mipsfile << "bne $t0,$0," << label << endl;
			}
			else {
				mipsfile << "bne $t0,$t1," << label << endl;
			}
		}
		else if (quad.op == "EQL") {
			mipsfile << "beq $t0,$t1," << label << endl;
		}
	}
	else if (quad.op == "LAB") {
		string label = quad.result + ":";
		mipsfile << label << endl;
	}
	/*else if (quad.op == "LODI") {
		string arg1 = quad.arg1;
		string result = quad.result;
		SymTableItem symitem3;
		int g3;
		find_sym_table(result, symitem3,g3);
		mipsfile << "li $t0," << arg1 << endl;
		mem_instr("sw", "$t0", symitem3, g3);
	}*/
	else if (quad.op == "SW") {
		SymTableItem symitem3;
		int g3;
		find_sym_table(quad.result, symitem3,g3);
		mem_instr("sw", quad.arg1, symitem3, g3);
	}
	else if (quad.op == "ARAS") {
		SymTableItem symitem1;
		SymTableItem symitem2;
		SymTableItem symitem3;
		int g1, g2, g3;
		find_sym_table(quad.arg1, symitem1,g1);
		find_sym_table(quad.arg2, symitem2,g2);
		find_sym_table(quad.result, symitem3,g3);
		mem_instr("lw", "$t0", symitem1, g1);
		mem_instr("lw", "$t1", symitem2, g2);
		//mem_instr("lw", "$t2", symitem3, g3);
		mipsfile << "sll $t1,$t1,2" << endl;
		if (g3 == 1) {
			mipsfile << "add $t3,$t1,$gp" << endl;
		}
		else {
			mipsfile << "add $t3,$t1,$sp" << endl;
		}
		mipsfile << "sw $t0, "<< symitem3.adr<<"($t3)" << endl;
	}
	else if (quad.op == "ASAR") {
		SymTableItem symitem1;
		SymTableItem symitem2;
		SymTableItem symitem3;
		int g1, g2, g3;
		find_sym_table(quad.arg1, symitem1, g1);
		find_sym_table(quad.arg2, symitem2, g2);
		find_sym_table(quad.result, symitem3, g3);
		mem_instr("lw", "$t0", symitem1, g1);
		mem_instr("lw", "$t1", symitem2, g2);
		//mem_instr("lw", "$t2", symitem3, g3);
		mipsfile << "sll $t0,$t0,2" << endl;
		if (g2 == 1) {
			mipsfile << "add $t3,$t0,$gp" << endl;
		}
		else {
			mipsfile << "add $t3,$t0,$sp" << endl;
		}
		mipsfile << "lw $t2, " << symitem2.adr << "($t3)" << endl;
		mem_instr("sw", "$t2", symitem3, g3);
	}
	else if (quad.op == "JMP") {
		string label = quad.result;
		mipsfile << "j " << label << endl;
	}
	else if (quad.op == "SCF") {
		SymTableItem symitem3;
		int g3;
		find_sym_table(quad.result, symitem3,g3);
		if (symitem3.type == inttype) {
			mipsfile << "li $v0,5" << endl;
		}
		else {
			mipsfile << "li $v0,12" << endl;
		}
		mipsfile << "syscall" << endl;
		mem_instr("sw", "$v0", symitem3, g3);//�ֲ��������޸ı�����ջ��	
	}
	else if (quad.op == "PRF") {
		string type = quad.arg2;  //int 0, char	1 ,void 2 Fix����void������bug
		string ptype = quad.result;  //0�����У�1����
		if (type == "0" || type == "1") {	//exp
			if (type == "0") {
				mipsfile << "li $v0, 1" << endl;
			}
			else {
				mipsfile << "li $v0, 11" << endl;
			}
			SymTableItem symitem1;
			int g1;
			find_sym_table(quad.arg1, symitem1,g1);
			mem_instr("lw", "$a0", symitem1, g1);
			mipsfile << "syscall" << endl;
		}
		else if (type == "str") {	//string
			mipsfile << "li $v0, 4" << endl;
			mipsfile << "la $a0," << "string" + quad.arg1 << endl;
			mipsfile << "syscall" << endl;
		}
		if (ptype == "1") {
			mipsfile << "li $v0, 4" << endl;
			mipsfile << "la $a0," << "enter" << endl;
			mipsfile << "syscall" << endl;
		}
	}
	else if (quad.op == "FUNC") {
		string label = quad.result + ":";
		SymTableItem func_name;
		find_sym_table(quad.result, func_name,global);
		/*vector<SymTableItem>items = symtables[func_name.adr].items;
		int func_size;
		if (items.size() == 0) {
			func_size = 0;
		}
		else {
			int begin = items[0].adr;
			int end = items[symtables[func_name.adr].offset - 1].adr;
			func_size = end - begin;
		}*/
		int func_size = func_name.func_size;
		cur_fun = quad.result;
		cur_fun_symtab = func_name.adr;

		mipsfile << label << endl;
		mipsfile << "subi $sp, $sp," << func_size << endl;	//����ջ�ռ�
		mipsfile << "sw $ra,0($sp)" << endl;		//����ra	
		//Todo �ָ�����Ա�Ĵ���
	}
	else if (quad.op == "PARA") {
		SymTableItem func_name;
		string real_para_num = quad.arg2;
		SymTableItem para;
		int g1;
		int offset = atoi(real_para_num.c_str()) * 4+36;//36��$ra���Ĵ�����������ռ�ռ�
		find_sym_table(quad.arg1, func_name,global);
		find_sym_table(quad.result, para,g1);
		/*vector<SymTableItem>items = symtables[func_name.adr].items;
		int func_size;
		if (items.size() == 0) {
			func_size = 0;
		}
		else {
			int begin = items[0].adr;
			int end = items[symtables[func_name.adr].offset - 1].adr;
			func_size = end - begin;
		}*/
		int func_size = func_name.func_size;
		mem_instr("lw", "$t0", para, g1);
		mipsfile << "subi $sp,$sp," << func_size << endl;
		mipsfile << "sw $t0," << offset << "($sp)" << endl;
		mipsfile << "addi $sp,$sp," << func_size << endl;
	}
	else if (quad.op == "CALL") {
		mipsfile << "jal " << quad.arg1 << endl;
	}
	else if (quad.op == "RET") {
		SymTableItem symitem1;
		int g1;
		find_sym_table(quad.arg1, symitem1,g1);
		SymTableItem func_name;
		find_sym_table(cur_fun, func_name,global);
		vector<SymTableItem>items = symtables[func_name.adr].items;
		/*int func_size;
		if (items.size() == 0) {
			func_size = 0;
		}
		else {
			int begin = items[0].adr;
			int end = items[symtables[func_name.adr].offset - 1].adr;
			func_size = end - begin;
		}*/
		int func_size = func_name.func_size;
		if (cur_fun == "main") {
			mipsfile << "li $v0," << 10 << endl;
			mipsfile << "syscall" << endl;
		}
		else {
			if (quad.arg1 != "") {
				mem_instr("lw", "$v0", symitem1, g1);
			}
			mipsfile << "lw $ra, 0($sp)" << endl;
			//�ָ�����Ա�Ĵ�����֮��� Todo
			mipsfile << "addi $sp, $sp," << func_size << endl;
			mipsfile << "jr $ra" << endl;
		}
	}
}
