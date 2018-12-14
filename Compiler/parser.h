#pragma once
#include<stdio.h>
#include<ctype.h>
#include <map>
#include<set>
#include <string> 
#include<iostream>
#include<vector>
#include<iomanip>
#include"lexer.h"
#include"error.h"
#include"token.h"
//#include"SymTable.h"
#include"SymTableItem.h"
using namespace std;

class Parser {
private:
	void const_declare(); //����������
	void const_def();     //����������
	void var_declare();   //�����������
	void var_def();		  //�����������
	void fun_def();		  //����������
	int parmeter_list(int subsymtable_index);  //���������
	void main_def();     //��������������
	void compound_statement(); //���������
	int statement_list();//���������
	int statement();//�������
	void if_statement();//�����������
	void while_statement();//����ѭ�����
	void callfun_statement(string &fun_value, TabType  &fun_type);//�������������
	void assign_statement();//����ֵ���
	void read_statement();//����scanf���
	void write_statement();//����printf���
	void switch_statement();//�����֧���
	void return_statement();//���������
	void condition(string);       //��������
	//void value_parmeterlist();  //����ֵ������
	void case_list(string exp_name, string labout);    //����case,����casetab
	void expression(string &,TabType&);//������ʽ
	void item(string &, TabType&);//������
	void factor(string &, TabType&);
	//��������
	void integer(int &value); //��������
	void default_list();
	void constant(string &con_name, TabType &con_type);

public:
	Parser(string filename):lexer(filename) {}
	Lexer lexer;
	void program();      //������������,�﷨���������
};