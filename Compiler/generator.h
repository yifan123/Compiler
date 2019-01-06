#pragma once
#include <iostream>
#include <fstream>
#include "QuadRuple.h"
#include<vector>
#include"SymTableItem.h"
#include"optimizer.h"
#include"error.h"
using namespace std;

class Generator {
public:
	vector<vector<string>> reg_descripter; //寄存器描述符
	Generator() {};
	Generator(string mips_filename) {
		mipsfile.open(mips_filename);
		reg_descripter=vector<vector<string>>(reg_st_num);
	}
	void quad2mips(vector<QuadRuple> quadtable);  //将生成的四元式转mips汇编
	void quadruple2mips(QuadRuple quad);
	void mem_instr(string instr, string reg, SymTableItem symitem, int aglobal);
	void close() {
		mipsfile.close();
	}

	//优化部分
	void o_mem_instr(string instr, string reg, SymTableItem symitem, int aglobal);
	void o_quad2mips(vector<fun_blocks> &blocks);
	void o_fun2mips(fun_blocks &fb);
	void o_bblock2mips(basic_block &bb);
	int o_quadruple2mips(basic_block &bb,int quad_index);
	int get_reg(string var, int is_load, set<int> not_reg = {});
	int spill_reg(set<int> not_reg);
private:
	ofstream mipsfile;  //生成目标代码文件
};