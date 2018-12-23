#include<iostream>
#include<fstream>
#include<vector>
#include"QuadRuple.h"
#include"optimizer.h"
#include"SymTableItem.h"
#include"SymTable.h"
#include<algorithm>
#include<set>
#include <iterator>

#define reg_s_num 8
#define reg_t_num 10


using namespace std;
extern int lab_index;
extern int cur_fun_symtab;
extern vector<SymTable> symtables;
extern int find_sym_table(string sym, SymTableItem& item, int &aglobal, int *symtbles_index = NULL);
int basic_block_count=0;
string basic_block_name;
string newtconst_o(int value) {   //在符号表中新建临时常量
	string a = "%";  //%代表在优化过程中新加的常量
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
			else if (quad.op == "CALL") {
				basic_block bb;
				string bb_name = newB();
				blocks[cur_fun_block].basic_blocks[bb_name] = bb;
				blocks[cur_fun_block].basic_blocks[cur_basic_block].next.insert(bb_name);
				cur_basic_block = bb_name;
				blocks[cur_fun_block].basic_blocks[cur_basic_block].quadtable.push_back(quad);
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

	//补填succ
	for (int i = 0; i < blocks.size(); i++) {
		for (map<string, basic_block>::iterator iter = blocks[i].basic_blocks.begin(); iter != blocks[i].basic_blocks.end(); iter++) {
			set<string> next = iter->second.next;
			for (set<string>::iterator iter1 =next.begin() ; iter1 != next.end(); iter1++) {
				blocks[i].basic_blocks[*iter1].succ.insert(iter->first);
			}
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

//C=A union B
void my_union(set<string> A, set<string> B, set<string>& C) {
	set_union(A.begin(), A.end(), B.begin(), B.end(), inserter(C, C.begin()));
}
//C=A intersection B
void my_intersection(set<string> A, set<string> B, set<string>& C) {
	set_intersection(A.begin(), A.end(), B.begin(), B.end(), inserter(C, C.begin()));
}
//C = A - B
void my_difference(set<string> A, set<string> B, set<string>& C) {
	set_difference(A.begin(), A.end(), B.begin(), B.end(), inserter(C, C.begin()));
}

set<string> find_reach_bblock(string define_pos, fun_blocks &fb) {
	set<string> ret_set = {};
	for (map<string, basic_block>::iterator iter = fb.basic_blocks.begin(); iter != fb.basic_blocks.end(); iter++) {
		if (iter->second.rin.find(define_pos) != iter->second.rin.end()) {
			ret_set.insert(iter->first);
		}
	}
	return ret_set;
}

int belong(string use_pos, set<string> reach_bblock) {
	//分割use_pos，提取它所在的基本块
	string use_block="";
	for (int i = 1; use_pos[i] != ','; i++) {
		use_block += use_pos[i];
	}
	if (reach_bblock.find(use_block) != reach_bblock.end()) {
		return 1;
	}
	return 0;
}

void reach_definiton_fun(fun_blocks &fb) {
	//计算gen,kill
	map<string, set<string>> var_def_position;//遍历函数内所有的基本块，建立每个变量的定义点，与书上不同，直接使用<B,1>,<B,2>这样的形式来代替d1,d2
	map<string, set<string>> var_use_position;
	//map<string, map< string, set<string> > >fb.du_chain;
	for (map<string, basic_block>::iterator iter = fb.basic_blocks.begin(); iter != fb.basic_blocks.end(); iter++) {
		//处理一个基本块
		for (int i = 0; i < iter->second.quadtable.size(); i++) {
			if (iter->second.quadtable[i].op == "ADD" ||
				iter->second.quadtable[i].op == "SUB" ||
				iter->second.quadtable[i].op == "MUL" ||
				iter->second.quadtable[i].op == "DIV" ||
				iter->second.quadtable[i].op == "NEG" ||
				iter->second.quadtable[i].op == "ASS" ||
				iter->second.quadtable[i].op == "ASAR" ||
				iter->second.quadtable[i].op == "SW") {
				var_def_position[iter->second.quadtable[i].result].insert("<" + iter->first + "," + to_string(i) + ">");
				fb.du_chain[iter->second.quadtable[i].result]["<" + iter->first + "," + to_string(i) + ">"] = {};
			}
			if (iter->second.quadtable[i].arg1 != "") {  //虽然可能有arg不代表变量，但我们只是从该集合中挑选，所以多了没什么关系
				var_use_position[iter->second.quadtable[i].arg1].insert("<" + iter->first + "," + to_string(i) + ">");
			}
			if (iter->second.quadtable[i].arg2 != "") {
				var_use_position[iter->second.quadtable[i].arg2].insert("<" + iter->first + "," + to_string(i) + ">");
			}
		}
	}
	//打印变量定义点信息，用于debug
	/*for (map<string, set<string>>::iterator iter2 = var_def_position.begin(); iter2 != var_def_position.end(); iter2++) {
		cout << iter2->first<<": ";
		cout << "{";
		for (set<string>::iterator iter3 = iter2->second.begin(); iter3 != iter2->second.end(); iter3++) {
			cout << *iter3 << ", ";
		}
		cout << "}"<<endl;
	}*/
	map<string, set<string>> gen = {}, kill = {};//每一条语句的gen,kill
	//计算gen,kill集合，处理一个函数所有的基本块
	for (map<string, basic_block>::iterator iter = fb.basic_blocks.begin(); iter != fb.basic_blocks.end(); iter++) {
		//处理一个基本块
		for (int i = 0; i < iter->second.quadtable.size(); i++) {
			string tem= "<" + iter->first + "," + to_string(i) + ">";
			set<string> tem1 = var_def_position[iter->second.quadtable[i].result];
			set<string>::iterator iter1 = tem1.find(tem);
			if (iter1 != tem1.end()) {
				gen[tem] = { tem };
				tem1.erase(iter1);
				kill[tem] = tem1;
			}
			else {
				gen[tem] = {};
				kill[tem] = {};
			}
		}
	}
	/*打印每条语句的gen,kill信息，用于debug
	cout << "gen: "<<endl;
	for (map<string, set<string>>::iterator iter = gen.begin(); iter != gen.end(); iter++) {
		cout << iter->first <<": ";
		cout << "{";
		for (set<string>::iterator iter1 = iter->second.begin(); iter1 != iter->second.end(); iter1++) {
			cout << *iter1 << ", ";
		}
		cout << "}" << endl;
	}
	cout << "kill: "<<endl;
	for (map<string, set<string>>::iterator iter = kill.begin(); iter != kill.end(); iter++) {
		cout << iter->first << ": ";
		cout << "{";
		for (set<string>::iterator iter1 = iter->second.begin(); iter1 != iter->second.end(); iter1++) {
			cout << *iter1 << ", ";
		}
		cout << "}" << endl;
	}*/
	    
	//计算基本块的gen,kill
	for (map<string, basic_block>::iterator iter = fb.basic_blocks.begin(); iter != fb.basic_blocks.end(); iter++) {
		//计算kill
		for (int i = 0; i < iter->second.quadtable.size(); i++) {
			string di = "<" + iter->first + "," + to_string(i) + ">";
			my_union(kill[di], iter->second.kill, iter->second.kill);
		}
		/*string dn = "<" + iter->first + "," + to_string(iter->second.quadtable.size()-1) + ">";
		set_union(gen[dn].begin, gen[dn].end, iter->second.gen.begin, iter->second.gen.end, iter->second.gen.begin);*/
		//计算gen
		for (int i = iter->second.quadtable.size() - 1; i >= 0; i--) {
			string di = "<" + iter->first + "," + to_string(i) + ">";
			set<string> gkdi = gen[di];
			for (int j = i + 1; j < iter->second.quadtable.size(); j++) {
				string dj = "<" + iter->first + "," + to_string(j) + ">";
				my_difference(gkdi,kill[dj], gkdi);
			}
			my_union(gkdi, iter->second.gen, iter->second.gen);
		}
		//打印基本块gen,kill信息，用于debug
		/*cout << iter->first << endl;
		cout << "gen: ";
		cout << "{";
		for (set<string>::iterator iter1 = iter->second.gen.begin(); iter1 != iter->second.gen.end(); iter1++) {
			cout << *iter1 << ", ";
		}
		cout << "}" << endl;
		cout << "kill: ";
		cout << "{";
		for (set<string>::iterator iter1 = iter->second.kill.begin(); iter1 != iter->second.kill.end(); iter1++) {
			cout << *iter1 << ", ";
		}
		cout << "}" << endl;*/
	}


	//计算rin,rout
	int flag=1;
	while (flag) {
		int flag1 = 1;
		for (map<string, basic_block>::iterator iter = fb.basic_blocks.begin(); iter != fb.basic_blocks.end(); iter++) {
			//cout << "基本块：" << iter->first << endl;
			//计算in
			for (set<string>::iterator iter1 = iter->second.succ.begin(); iter1 != iter->second.succ.end(); iter1++) {
				my_union(iter->second.rin, fb.basic_blocks[*iter1].rout, iter->second.rin);
			}
			//计算out
			set<string> in_kill;
			map<set<string>, string> mTemp;
			mTemp[iter->second.rout] = "1";
			
			my_difference(iter->second.rin, iter->second.kill, in_kill);
			my_union(iter->second.gen, in_kill, iter->second.rout);
			if (mTemp.find(iter->second.rout) == mTemp.end()) {
				flag1 = 0;
			}
		}
		if (flag1 == 1) {
			flag = 0;
		}
	}

	//构建du链
	for (map<string, map< string, set<string> > >::iterator iter = fb.du_chain.begin(); iter != fb.du_chain.end(); iter++) {
		//处理单个变量
		string var_name = iter->first;
		set<string> var_use_pos = var_use_position[var_name];
		for (map<string, set<string> >::iterator iter1 = fb.du_chain[var_name].begin(); iter1 != fb.du_chain[var_name].end(); iter1++) {
			//处理每个变量的一处定义点
			string define_pos = iter1->first;
			//查找该定义点到达的基本块
			set<string> reach_bblock=find_reach_bblock(define_pos, fb);
			for (set<string>::iterator iter2 = var_use_pos.begin(); iter2 != var_use_pos.end(); iter2++) {
				if (belong(*iter2, reach_bblock)) {
					iter1->second.insert(*iter2);
				}
			}
		}
	}
	return;
}
//到达定义分析
void Optimizer::reach_definiton() {
	for (int i = 0; i < blocks.size(); i++) {
		reach_definiton_fun(blocks[i]);
	}
}

//打印
void print_string_set(set<string> string_set,ofstream &f) {
	f << "{";
	for (set<string>::iterator iter = string_set.begin(); iter != string_set.end(); iter++) {
		f << *iter << ", ";
	}
	f << "}" << endl;
}
void print_du_chain_fun(fun_blocks &fb, ofstream &f) {
	for (map<string, map< string, set<string> > >::iterator iter = fb.du_chain.begin(); iter!= fb.du_chain.end(); iter++) {
		f << iter->first << ": " << endl;
		for (map< string, set<string>>::iterator iter1 = iter->second.begin(); iter1 != iter->second.end(); iter1++) {
			f << iter1->first << ": ";
			print_string_set(iter1->second, f);
		}
	}
}
void Optimizer::print_reach_definiton() {
	for (int i = 0; i < blocks.size(); i++) {
		ofstream reach_definiton_file;
		reach_definiton_file.open("dataflow_analysis/reach_definiton/"+ blocks[i].func_name);
		reach_definiton_file << "reach definiton information" << endl;
		for (map<string, basic_block>::iterator iter = blocks[i].basic_blocks.begin(); iter != blocks[i].basic_blocks.end(); iter++) {
			reach_definiton_file << "basic block name: " << iter->first << endl;
			reach_definiton_file << "gen: ";
			print_string_set(iter->second.gen,reach_definiton_file);
			reach_definiton_file << "kill: ";
			print_string_set(iter->second.kill, reach_definiton_file);
			reach_definiton_file << "rin: ";
			print_string_set(iter->second.rin, reach_definiton_file);
			reach_definiton_file << "rout: ";
			print_string_set(iter->second.rout, reach_definiton_file);
			reach_definiton_file << endl;
		}
		reach_definiton_file << "du_chain" << endl;
		print_du_chain_fun(blocks[i], reach_definiton_file);
	}
}

void compute_bblock_def_use(basic_block &basic_block) {
	//统计基本块中的所有变量
	map<string,int[2]> var_set; //[0,0]分别代表变量当前是否已被定义，是否已被使用
	for (int i = 0; i < basic_block.quadtable.size(); i++) {
		string op = basic_block.quadtable[i].op;
		string arg1 = basic_block.quadtable[i].arg1;
		string arg2 = basic_block.quadtable[i].arg2;
		string result = basic_block.quadtable[i].result;
		if (op == "ADD" || op == "SUB" || op == "MUL" || op == "DIV") {
			var_set[arg1][0] = 0;
			var_set[arg1][1] = 0;
			var_set[arg2][0] = 0;
			var_set[arg2][1] = 0;
			var_set[result][0] = 0;
			var_set[result][1] = 0;
		}
		else if (op == "NEG" || op == "ASS" || op == "ASAR") {
			var_set[arg1][0] = 0;
			var_set[arg1][1] = 0;
			var_set[result][0] = 0;
			var_set[result][1] = 0;
		}
		else if (op == "LSS" || op == "LEQ" || op == "GTR" || op == "GEQ" || op == "NEQ" || op == "EQL"|| op == "ARAS") {
			var_set[arg1][0] = 0;
			var_set[arg1][1] = 0;
			var_set[arg2][0] = 0;
			var_set[arg2][1] = 0;
		}
		else if (op == "PARA"|| op == "SCF"|| op == "SW") {
			var_set[result][0] = 0;
			var_set[result][1] = 0;
		}
		else if (op == "RET"|| op == "PRF") {
			var_set[arg1][0] = 0;
			var_set[arg1][1] = 0;
		}
	}
	
	//计算def,use集合
	for (int i = 0; i < basic_block.quadtable.size(); i++) {
		string op = basic_block.quadtable[i].op;
		string arg1 = basic_block.quadtable[i].arg1;
		string arg2 = basic_block.quadtable[i].arg2;
		string result = basic_block.quadtable[i].result;
		if (op == "ADD" || op == "SUB" || op == "MUL" || op == "DIV") {
			if (var_set[arg1][0] == 0) {//还没被定义
				basic_block.use.insert(arg1);
				var_set[arg1][1] = 1; //
			}
			if (var_set[arg2][0] == 0) {//还没被定义
				basic_block.use.insert(arg2);
				var_set[arg2][1] = 1; //
			}
			if (var_set[result][1] == 0) {//还没被使用
				basic_block.def.insert(result);
				var_set[result][0] = 1; //
			}
		}
		else if (op == "NEG" || op == "ASS" || op == "ASAR") {
			if (var_set[arg1][0] == 0) {//还没被定义
				basic_block.use.insert(arg1);
				var_set[arg1][1] = 1; //
			}
			if (var_set[result][1] == 0) {//还没被使用
				basic_block.def.insert(result);
				var_set[result][0] = 1; //
			}
		}
		else if (op == "LSS" || op == "LEQ" || op == "GTR" || op == "GEQ" || op == "NEQ" || op == "EQL" || op == "ARAS") {
			if (var_set[arg1][0] == 0) {//还没被定义
				basic_block.use.insert(arg1);
				var_set[arg1][1] = 1; //
			}
			if (var_set[arg2][0] == 0) {//还没被定义
				basic_block.use.insert(arg2);
				var_set[arg2][1] = 1; //
			}
		}
		else if (op == "PARA" || op == "SCF" || op == "SW") {
			if (var_set[result][1] == 0) {//还没被使用
				basic_block.def.insert(result);
				var_set[result][0] = 1; //
			}
		}
		else if (op == "RET"&& arg1 !="" ) {
			if (var_set[arg1][0] == 0) {//还没被定义
				basic_block.use.insert(arg1);
				var_set[arg1][1] = 1; //
			}
		}
		else if (op == "PRF"&&arg2 != "str") {
			if (var_set[arg1][0] == 0) {//还没被定义
				basic_block.use.insert(arg1);
				var_set[arg1][1] = 1; //
			}
		}
	}
	set<string> use_tem = basic_block.use;
	for (set<string>::iterator iter = basic_block.use.begin(); iter != basic_block.use.end(); iter++) {
		if ((*iter)[0] == '$') {   //是寄存器
			use_tem.erase(*iter);
		}
	}
	basic_block.use = use_tem;

}
void live_var_analysis_fun(fun_blocks &fb) {
	//计算def,use集合，处理一个函数所有的基本块
	for (map<string, basic_block>::iterator iter = fb.basic_blocks.begin(); iter != fb.basic_blocks.end(); iter++) {
		//处理一个基本块
		compute_bblock_def_use(iter->second);
	}
	//计算ain,aout
	int flag = 1;
	while (flag) {
		int flag1 = 1;
		for (map<string, basic_block>::reverse_iterator iter = fb.basic_blocks.rbegin(); iter != fb.basic_blocks.rend(); iter++) {
			//cout << "基本块：" << iter->first << endl;
			//计算out
			for (set<string>::iterator iter1 = iter->second.next.begin(); iter1 != iter->second.next.end(); iter1++) {
				my_union(iter->second.aout, fb.basic_blocks[*iter1].ain, iter->second.aout);
			}
			//计算out
			set<string> out_def;
			map<set<string>, string> mTemp;
			mTemp[iter->second.ain] = "1";

			my_difference(iter->second.aout, iter->second.def, out_def);
			my_union(iter->second.use, out_def, iter->second.ain);
			if (mTemp.find(iter->second.ain) == mTemp.end()) {
				flag1 = 0;
			}
		}
		if (flag1 == 1) {
			flag = 0;
		}
	}
	return;
}
//必须先划分基本块
void Optimizer::live_var_analysis() {
	for (int i = 0; i < blocks.size(); i++) {
		live_var_analysis_fun(blocks[i]);
	}
}

//打印活跃变量信息
void Optimizer::print_live_var_analysis() {
	for (int i = 0; i < blocks.size(); i++) {
		ofstream live_var_file;
		live_var_file.open("dataflow_analysis/live_var_analysis/" + blocks[i].func_name+".txt");
		for (map<string, basic_block>::iterator iter = blocks[i].basic_blocks.begin(); iter != blocks[i].basic_blocks.end(); iter++) {
			live_var_file << "basic block name: " << iter->first << endl;
			live_var_file << "def: ";
			print_string_set(iter->second.def, live_var_file);
			live_var_file << "use: ";
			print_string_set(iter->second.use, live_var_file);
			live_var_file << "ain: ";
			print_string_set(iter->second.ain, live_var_file);
			live_var_file << "aout: ";
			print_string_set(iter->second.aout, live_var_file);
			live_var_file << endl;
		}
	}
}

int is_var_conflict(string a,string b, fun_blocks &fb) {
	// Fix 这里简单地定义：活跃范围重合的变量之间冲突
	// 可以使用du-chain来改进
	set<string> bblock_a, bblock_b,tem;
	for (map<string, basic_block>::iterator iter = fb.basic_blocks.begin(); iter != fb.basic_blocks.end(); iter++) {
		if (iter->second.ain.find(a) != iter->second.ain.end()) {
			bblock_a.insert(iter->first);
		}
		if (iter->second.ain.find(b) != iter->second.ain.end()) {
			bblock_b.insert(iter->first);
		}
	}
	my_intersection(bblock_a, bblock_b, tem);
	if (tem.size() == 0) {
		return 0;
	}
	else {
		return 1;
	}
}
//< <node1,color1,next1>, <node2,color2,next2>... >
void build_conflict_graph(fun_blocks &fb) {
	vector<string> conflict_graph_nodes;
	map<string, basic_block> basic_blocks = fb.basic_blocks;
	//只有跨越基本块仍活跃的变量才能分配到全局寄存器
	for (map<string, basic_block>::iterator iter = basic_blocks.begin(); iter != basic_blocks.end(); iter++) {
		if (iter->first != "enter") {
			for (set<string>::iterator iter1 = iter->second.ain.begin(); iter1 != iter->second.ain.end(); iter1++) {
				if (iter->second.aout.find(*iter1) != iter->second.aout.end()) {
					conflict_graph_node tem;
					tem.color = -2; //-2代表白色，即未被染色
					tem.priority = 1;//Fix 这只是初步将所有点进memory的优先级设为相等的值
					fb.conflict_graph[*iter1] = tem;
				}
			}
		}
	}
	for (map<string, conflict_graph_node>::iterator iter = fb.conflict_graph.begin(); iter != fb.conflict_graph.end(); iter++) {
		conflict_graph_nodes.push_back(iter->first);
	}
	for (int i = 0; i < conflict_graph_nodes.size(); i++) {
		string a = conflict_graph_nodes[i];
		for (int j = i + 1; j < conflict_graph_nodes.size(); j++) {
			string b = conflict_graph_nodes[j];
			if (is_var_conflict(a, b,fb)) {  //两个变量冲突
				fb.conflict_graph[a].next.insert(b);
				fb.conflict_graph[b].next.insert(a);
			}
		}
	}
}

int find_color(set<int> &node_color_num) {
	int i = 0;
	for (set<int>::iterator iter = node_color_num.begin(); iter != node_color_num.end(); iter++) {
		if (*iter != i) {
			return i;
		}
		i++;
	}
	return i;
}
void find_memory_node(fun_blocks &fb) {
	//Fix
	map<string, conflict_graph_node>::iterator max_node_p;
	int max_node_v = -1;
	for (map<string, conflict_graph_node>::iterator iter = fb.conflict_graph.begin(); iter != fb.conflict_graph.end(); iter++) {
		if (iter->second.color == -2&& iter->second.priority>max_node_v) {//-2代表白色，即未被染色
			max_node_v = iter->second.priority;
			max_node_p = iter;
		}
	}
	max_node_p->second.color = -1; //-1代表灰色，即该点将放入内存中
	return;
}
void graph_coloring(fun_blocks &fb,int color_num) {
	int node_num = fb.conflict_graph.size(); //还没染色的点
	while (node_num>0) {
		for (map<string, conflict_graph_node>::iterator iter = fb.conflict_graph.begin(); iter != fb.conflict_graph.end(); iter++) {
			set<int> node_color_num;
			for (set<string>::iterator iter1 = iter->second.next.begin(); iter1 != iter->second.next.end(); iter1++) {
				if (fb.conflict_graph[*iter1].color>=0&&node_color_num.find(fb.conflict_graph[*iter1].color) == node_color_num.end()) {
					node_color_num.insert(fb.conflict_graph[*iter1].color);
				}
			}
			if (node_color_num.size() < reg_s_num) {
				node_num--;
				fb.conflict_graph[iter->first].color = find_color(node_color_num);
			}
		}
		if (node_num > 0) {
			find_memory_node(fb);
			node_num--;
		}
	}
}
void Optimizer::global_reg_alloc(fun_blocks &fb) {
	build_conflict_graph(fb); //首先建立冲突图
	//开始图染色算法
	graph_coloring(fb, reg_s_num);
}
string convert2ident(string a) {
	string b = a;
	if (b[0] == '@'){
		b = "t" + a.substr(1, a.size());
	}
	return b;
}
void Optimizer::graphviz_global_reg_alloc(fun_blocks &fb) {
	ofstream dotfile;
	dotfile.open("basic_block/conflict_graph/" + fb.func_name + ".dot");
	map<string, conflict_graph_node> conflict_graph=fb.conflict_graph;
	dotfile << "graph " << fb.func_name << "{" << endl;
	dotfile << "label =" << fb.func_name << endl;
	for (map<string, conflict_graph_node>::iterator iter = conflict_graph.begin(); iter != conflict_graph.end(); iter++) {
		string color= "White";
		if (iter->second.color == -1) {
			color = "Red";
		}
		else if (iter->second.color == -1) {
			color = "Turquoise";
		}
		dotfile << convert2ident(iter->first) << "[" << "label=" << "\""<< convert2ident(iter->first) << "\"" << ", shape=circle"<<", style=filled, fillcolor="<<color<<"]\n";
	}
	set<string> edge;
	for (map<string, conflict_graph_node>::iterator iter = conflict_graph.begin(); iter != conflict_graph.end(); iter++) {
		set<string>::iterator iter1 = iter->second.next.begin();
		while (iter1 != iter->second.next.end()) {
			string a, b;
			if (convert2ident(iter->first) < convert2ident(*iter1)) {
				a = convert2ident(iter->first);
				b = convert2ident(*iter1);
			}
			else {
				b = convert2ident(iter->first);
				a = convert2ident(*iter1);
			}
			edge.insert(a + "--" + b);
			iter1++;
		}
	}
	for (set<string>::iterator iter = edge.begin(); iter != edge.end(); iter++) {
		dotfile << *iter << endl;
	}
	dotfile << "}" << endl;
	dotfile.close();
}

void Optimizer::work() {
	//为了提高测试时的速度，先把优化部分注释掉一部分  Fix
	const_fold();
	/*build_blocks();
	graphviz_fun_block();
	reach_definiton();
	live_var_analysis();
	print_reach_definiton();
	print_live_var_analysis();
	//其实前面的这几个函数都没必要单独写一个带fun的，一起处理即可，但为了前面的不出bug，前面的就不改了，从后面开始改
	for (int i = 0; i < blocks.size(); i++) {
		global_reg_alloc(blocks[i]);
		graphviz_global_reg_alloc(blocks[i]);
	}
	*/
}