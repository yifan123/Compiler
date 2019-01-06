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
	vector<QuadRuple> quadtable; //�洢һ���������ڵ���Ԫ��
	set<string> next; //��¼��̻���������֣���vector��sizeһ��Ϊ1��2
	set<string> succ; // ��¼ǰ������������֣���vector��sizeһ��Ϊ1��2
	set<string> def, use, ain, aout; //���ڻ�Ծ��������
	set<string> gen, kill, rin, rout = {};//���ڵ��ﶨ�����
};

class fun_blocks {
public:
	map<string, basic_block> basic_blocks; //�洢һ�����������л�����
	vector<string> block_sequence;    //�洢һ��������������ֵ�˳��������Ŀ�����ʱʹ��
	string func_name; //��¼��������Ϊ������Ż���׼��,�ĳ�map�ṹ�������Ա�ѷ��� Fix
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
	void DAG();  //ʹ��DAGͼɾ���������ڲ��Ĺ����ӱ��ʽ
	void global_reg_alloc(fun_blocks &fb);//ʹ��ͼ��ɫ�㷨����ȫ�ּĴ�������
	void graphviz_global_reg_alloc(fun_blocks &fb);
	void register_pool();
	void dataflow();//ͨ����Ծ���������������ö���-ʹ���������ȷ���������ͻͼ����������������
	void temp_reg_pool();//��������ʱ����������ʱ�Ĵ�������ʱ�Ĵ����أ����������ɽϸ�������Ŀ�����
	//���������������Ż�����
	void const_fold();  //�����۵���Ȼ����ֱ�Ӽ���ָ�����Ϊ������������������
	void look_hole();
	void live_var_analysis();
	void reach_definiton();
	void print_reach_definiton();
	void print_live_var_analysis();
	//void build_conflict_graph();
};




