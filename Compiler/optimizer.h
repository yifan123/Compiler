#pragma once
#include<vector>
#include"QuadRuple.h"
#include<map>
#include<set>
class basic_block {
public:
	vector<QuadRuple> quadtable; //�洢һ���������ڵ���Ԫ��
	set<string> next; //��¼��̻���������֣���vector��sizeһ��Ϊ1��2
};

class fun_blocks {
public:
	map<string, basic_block> basic_blocks; //�洢һ�����������л�����
	string func_name; //��¼��������Ϊ������Ż���׼��,�ĳ�map�ṹ�������Ա�ѷ��� Fix
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
	void global_reg_alloc();//ʹ�����ü�������ɫ�㷨����ȫ�ּĴ�������
	void dataflow();//ͨ����Ծ���������������ö���-ʹ���������ȷ���������ͻͼ����������������
	void temp_reg_pool();//��������ʱ����������ʱ�Ĵ�������ʱ�Ĵ����أ����������ɽϸ�������Ŀ�����
	//���������������Ż�����
	void const_fold();  //�����۵���Ȼ����ֱ�Ӽ���ָ�����Ϊ������������������
};


