#pragma once
#include<vector>
#include"QuadRuple.h"
#include<map>
#include<set>
#include"error.h"

class conflict_graph_node{
public:
	int color;
	int priority;
	set<string> next;
};

class basic_block {
public:
	vector<QuadRuple> quadtable; //存储一个基本块内的四元组
	set<string> next; //记录后继基本块的名字，该vector的size一般为1或2
	set<string> succ; // 记录前驱基本块的名字，该vector的size一般为1或2
	set<string> def, use, ain, aout; //用于活跃变量分析
	set<string> gen, kill, rin, rout = {};//用于到达定义分析
};

class fun_blocks {
public:
	map<string, basic_block> basic_blocks; //存储一个函数的所有基本块
	vector<string> block_sequence;    //存储一个函数基本块出现的顺序，在生成目标代码时使用
	string func_name; //记录函数名，为跨过程优化做准备,改成map结构后，这个成员已废弃 Fix
	map<string, map< string, set<string> > >du_chain;
	map<string, conflict_graph_node> conflict_graph;
};

class Optimizer {
public:
	vector<QuadRuple> *src_quadtable;
	vector<QuadRuple> quadtable;
	vector<fun_blocks> blocks;  
	Optimizer() {}
	Optimizer(vector<QuadRuple> *aquadtable){
		src_quadtable = aquadtable;
	}	
	void build_blocks();
	void graphviz_fun_block();
	void work();
	void DAG();  //使用DAG图删除基本块内部的公共子表达式
	void global_reg_alloc(fun_blocks &fb);//使用图着色算法进行全局寄存器分配
	void graphviz_global_reg_alloc(fun_blocks &fb);
	void register_pool();
	void dataflow();//通过活跃变量分析，或利用定义-使用链建网等方法建立冲突图，进行数据流分析
	void temp_reg_pool();//代码生成时合理利用临时寄存器（临时寄存器池），并能生成较高质量的目标代码
	//后续将加入其他优化方法
	void const_fold();  //常数折叠虽然不能直接减少指令，但它为常数传播创造了条件
	void look_hole();
	void live_var_analysis();
	void reach_definiton();
	void print_reach_definiton();
	void print_live_var_analysis();
	//void build_conflict_graph();
};




