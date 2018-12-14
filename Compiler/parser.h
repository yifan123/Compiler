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
	void const_declare(); //处理常量声明
	void const_def();     //处理常量定义
	void var_declare();   //处理变量声明
	void var_def();		  //处理变量定义
	void fun_def();		  //处理函数定义
	int parmeter_list(int subsymtable_index);  //处理参数表
	void main_def();     //处理主函数定义
	void compound_statement(); //处理复合语句
	int statement_list();//处理语句列
	int statement();//处理语句
	void if_statement();//处理条件语句
	void while_statement();//处理循环语句
	void callfun_statement(string &fun_value, TabType  &fun_type);//处理函数调用语句
	void assign_statement();//处理赋值语句
	void read_statement();//处理scanf语句
	void write_statement();//处理printf语句
	void switch_statement();//处理分支语句
	void return_statement();//处理返回语句
	void condition(string);       //处理条件
	//void value_parmeterlist();  //处理值参数表
	void case_list(string exp_name, string labout);    //处理case,建立casetab
	void expression(string &,TabType&);//处理表达式
	void item(string &, TabType&);//处理项
	void factor(string &, TabType&);
	//处理因子
	void integer(int &value); //处理整数
	void default_list();
	void constant(string &con_name, TabType &con_type);

public:
	Parser(string filename):lexer(filename) {}
	Lexer lexer;
	void program();      //处理整个程序,语法分析的入口
};