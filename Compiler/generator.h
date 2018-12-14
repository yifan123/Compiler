#pragma once
#include <iostream>
#include <fstream>
#include "QuadRuple.h"
#include<vector>
#include"SymTableItem.h"
using namespace std;

class Generator {
public:
	Generator() {};
	Generator(string mips_filename) {
		mipsfile.open(mips_filename);
	}
	void quad2mips(vector<QuadRuple> quadtable);  //将生成的四元式转mips汇编
	void quadruple2mips(QuadRuple quad);
	void mem_instr(string instr, string reg, SymTableItem symitem, int aglobal);
	void close() {
		mipsfile.close();
	}
private:
	ofstream mipsfile;  //生成目标代码文件
};