#include<iostream>
#include<fstream>
#include<vector>
#include"QuadRuple.h"
#include"optimizer.h"
#include"SymTableItem.h"
#include"SymTable.h"
using namespace std;
extern int lab_index;
extern int cur_fun_symtab;
extern vector<SymTable> symtables;
extern int find_sym_table(string sym, SymTableItem& item, int &aglobal, int *symtbles_index = NULL);
int basic_block_count=0;
string basic_block_name;
string newtconst_o(int value) {   //在符号表中新建临时常量
	string a = "%";  //%代表在优化过程中新加的常量或变量
	string b = to_string(lab_index++);
	string name = a + b;
	symtables[cur_fun_symtab].enter(name, constantobj, inttype, value, 0);
	return name;
}
string newB() {
	string a = "B";  //代表在优化过程中新加的常量或变量
	string b = to_string(basic_block_count++);
	string name = a + b;
	return name;
}
QuadRuple const_fold_quad(QuadRuple quad) {
	int result;
	if (quad.op == "FUNC") {
		SymTableItem symitem3;
		int g3;
		find_sym_table(quad.result, symitem3, g3);
		cur_fun_symtab = symitem3.adr;
	}
	else if (quad.op == "ADD" | quad.op == "SUB" | quad.op == "MUL"
		| quad.op == "DIV") {
		SymTableItem symitem1;
		SymTableItem symitem2;
		SymTableItem symitem3;
		int g1;
		int g2;
		int g3;
		find_sym_table(quad.arg1, symitem1, g1);
		find_sym_table(quad.arg2, symitem2, g2);
		find_sym_table(quad.result, symitem3, g3);
		if (symitem1.obj == constantobj && symitem2.obj == constantobj) {
			if (quad.op == "ADD") {	//加法
				result = symitem1.adr + symitem2.adr;
			}
			else if (quad.op == "SUB") { //减法
				result = symitem1.adr - symitem2.adr;
			}
			else if (quad.op == "MUL") {	//乘法
				result = symitem1.adr * symitem2.adr;
			}
			else if (quad.op == "DIV") {	//乘法
				result = symitem1.adr / symitem2.adr;  //整除
			}
			string arg1 = newtconst_o(result);
			return QuadRuple("ASS", arg1, "", quad.result);
		}	
	}
	else if (quad.op == "NEG") {
		SymTableItem symitem1;
		SymTableItem symitem3;
		int g1, g3;
		int cur_symtbles_index = 0;
		int i = find_sym_table(quad.arg1, symitem1, g1, &cur_symtbles_index);
		find_sym_table(quad.result, symitem3, g3);
		if (symitem1.obj == constantobj) {
			result = -symitem1.adr;
			string arg1 = newtconst_o(result);
			return QuadRuple("ASS", arg1, "", quad.result);
		}
	}
	return quad;
}

void Optimizer::const_fold() {
	for (vector<QuadRuple>::iterator it = src_quadtable->begin(); it != src_quadtable->end(); it++) {
		QuadRuple quad = *it;
		quadtable.push_back(const_fold_quad(quad));
	}
}

/*
入口：LAB
出口：LSS,LEQ,GTR,GEQ,NEQ,EQL,JMP
FUNC  RET
*/
void Optimizer::build_blocks() {
	//string cur_func_name = "";
	string cur_basic_block;
	int cur_fun_block = -1;
	string lst_basic_block="";
	for (int i = 0; i < quadtable.size(); i++) {   
		QuadRuple quad = quadtable[i];
		if (quad.op == "FUNC") {
			if (cur_fun_block != -1) {
				basic_block bb;
				blocks[cur_fun_block].basic_blocks["exit"] = bb;
				blocks[cur_fun_block].basic_blocks[cur_basic_block].next.insert("exit");
			}
			cur_fun_block++;
			fun_blocks fb;
			fb.func_name = quad.result;
			basic_block bb;
			fb.basic_blocks["enter"]=bb;
			blocks.push_back(fb);
			/*string bb_name = newB();
			cur_basic_block = bb_name;*/
			lst_basic_block = "enter";
			basic_block bb1;
			string bb_name = newB();
			blocks[cur_fun_block].basic_blocks[bb_name] = bb1;
			blocks[cur_fun_block].basic_blocks["enter"].next.insert(bb_name);
			cur_basic_block = bb_name;
		}
		else {
			if (quad.op == "LAB") {
				if (lst_basic_block == "enter") {
					blocks[cur_fun_block].basic_blocks.erase(cur_basic_block);
					cur_basic_block = "enter";
				}
				basic_block bb;
				blocks[cur_fun_block].basic_blocks[quad.result] = bb;  //新建基本块
				blocks[cur_fun_block].basic_blocks[cur_basic_block].next.insert(quad.result);
				cur_basic_block = quad.result;
				blocks[cur_fun_block].basic_blocks[cur_basic_block].quadtable.push_back(quad);
			}
			else if (quad.op == "LSS" | quad.op == "LEQ" | quad.op == "GTR" | quad.op == "GEQ" | quad.op == "NEQ" | quad.op == "EQL") {
				basic_block bb;
				string bb_name = newB();
				blocks[cur_fun_block].basic_blocks[bb_name] = bb;
				blocks[cur_fun_block].basic_blocks[cur_basic_block].next.insert(bb_name);
				blocks[cur_fun_block].basic_blocks[cur_basic_block].next.insert(quad.result);
				blocks[cur_fun_block].basic_blocks[cur_basic_block].quadtable.push_back(quad);
				cur_basic_block = bb_name;
			}
			else if (quad.op == "JMP") {
				basic_block bb;
				string bb_name = newB();
				blocks[cur_fun_block].basic_blocks[bb_name] = bb;
				blocks[cur_fun_block].basic_blocks[cur_basic_block].next.insert(quad.result);
				blocks[cur_fun_block].basic_blocks[cur_basic_block].quadtable.push_back(quad);
				cur_basic_block = bb_name;
			}
			else if (quad.op == "RET") {
				blocks[cur_fun_block].basic_blocks[cur_basic_block].quadtable.push_back(quad);
				blocks[cur_fun_block].basic_blocks[cur_basic_block].next.insert("exit");
			}
			else {
				blocks[cur_fun_block].basic_blocks[cur_basic_block].quadtable.push_back(quad);
			}
			lst_basic_block = "";
		}
	}
	basic_block bb;
	blocks[cur_fun_block].basic_blocks["exit"] = bb;
	blocks[cur_fun_block].basic_blocks[cur_basic_block].next.insert("exit");
	//去除空白块
	for (int i = 0; i < blocks.size(); i++) {
		vector<string> del_blocks;
		for (map<string, basic_block>::iterator iter = blocks[i].basic_blocks.begin(); iter != blocks[i].basic_blocks.end(); iter++) {
			if (iter->second.quadtable.size() == 0 && iter->first != "enter" && iter->first != "exit") {			
				del_blocks.push_back(iter->first);
			}
		}
		for (int j = 0; j < del_blocks.size(); j++) {
			 blocks[i].basic_blocks.erase(del_blocks[j]);
		}
	}
}

string basic_block2str(basic_block b,string bblock_name) {
	string out = "\""+ bblock_name +":\\n";
	vector<QuadRuple> qtable = b.quadtable;
	
	for (int i = 0; i < qtable.size(); i++) {
		QuadRuple q = qtable[i];
		out = out + q.op + "," + q.arg1 + "," + q.arg2 + "," + q.result + "\\n";
	}
	out = out + "\"";
	return out;
}
void graphviz_basic_block(fun_blocks &fb) {
	ofstream dotfile;
	dotfile.open("basic_block/dot/"+fb.func_name + ".dot");
	map<string, basic_block> bblocks = fb.basic_blocks;
	dotfile << "digraph " << fb.func_name << "{" << endl;
	dotfile << "label =" << fb.func_name << endl;
	for (map<string, basic_block>::iterator iter = bblocks.begin(); iter != bblocks.end(); iter++) {
		dotfile << iter->first << "["<<"label="<< basic_block2str(iter->second, iter->first)<<" shape=box]\n";
	}
	for (map<string, basic_block>::iterator iter = bblocks.begin(); iter != bblocks.end(); iter++) {
		set<string>::iterator iter1 = iter->second.next.begin();
		while (iter1 != iter->second.next.end()){
			dotfile << iter->first << "->" << *iter1 << endl;
			iter1++;
		}
		/*for (int i = 0; i < iter->second.next.size(); i++) {
			dotfile << iter->first << "->" << iter->second.next[i] << endl;
		}*/
	}
	dotfile << "}" << endl;
	dotfile.close();
}
void Optimizer::graphviz_fun_block() {
	for (int i = 0; i < blocks.size(); i++){
		graphviz_basic_block(blocks[i]);
	}
}

void Optimizer::work() {
	const_fold();
	build_blocks();
	graphviz_fun_block();
}