#include "map"
#include <string>
#include<iostream>
#include"vector"
#include"parser.h"
#include"QuadRuple.h"
#include"SymTable.h"
#include"generator.h"
#include"optimizer.h"
#define OPT 0   //优化开关

using namespace std;

vector<string> printsymbol{
	"INT", "CHAR", "VOID", "IDENT", "STRING",
	"UINTEGERT", "CHARACTER",
	"PLUS", "MINUS", "MULT", "DIV",
	"LSS", "LEQ", "GTR", "GEQ", "NEQ", "EQL", "BECOMES",
	"LPARENT", "RPARENT", "LBRACK", "RBRACK", "LBRACE", "RBRACE", "COMMA", "SEMICOLON", "COLON",
	"CONST", "IF", "WHILE", "SWITCH", "CASE", "DEFAULT", "RETURN", "SCANF", "PRINTF",
	"NOTYPE"
};
int error_num = 0;
//int subsymtable_index = 0;
vector<QuadRuple> quadtable;
//SymTable symtable;
vector<SymTable> symtables;
map<int, string> stringtab;
int string_index = 0;
int global = 0;
//词法分析使用的main函数
/*
int main() {
	string fin_name;
	cout << "please input the file name of source program :";
	cin >> fin_name;
	Lexer lexer(fin_name);
	cout << left << setw(15) << "symbol(符号)" << left << setw(20) << "token(词)" << endl;
	while (!lexer.to_buffer_end())
	{
		lexer.getsym();
		switch (lexer.token.sym) {
		case UINTEGERT: {
			cout << std::left << std::setw(15) << printsymbol[lexer.token.sym] << std::left << std::setw(20) << lexer.token.num <<lexer.line<<std::endl;
			break;
		}
		default: cout << std::left << std::setw(15) << printsymbol[lexer.token.sym] << std::left << std::setw(20) << lexer.token.id  << lexer.line<<std::endl;
		}
	}
	system("pause");
	return 0;
}*/

//语法分析使用的main函数
int main()
{
	string fin_name;
	ofstream quadfile;
	cout << "please input the file name of source program :";
	cin >> fin_name;
	Parser parser(fin_name);
	parser.lexer.getsym();
	parser.program();
	if (error_num == 0)
	{
		//打印优化前部分
		quadfile.open("quad.txt");
		cout << "Syntax Analyze Complete!" << endl;
		cout << "error num:" << error_num << endl;
		//map<string, int> resultop;
		for (int i = 0; i < quadtable.size(); i++)   //输出优化前中间代码到文件
		{
			QuadRuple q = quadtable[i];
			quadfile << q.op << " " << q.arg1 << " " << q.arg2 << " " << q.result << endl;
			//resultop[q.op] = 1;
			//打印出所有使用过的中间指令
		}
		/*map<string, int>::iterator iter;
		for (iter = resultop.begin(); iter != resultop.end(); iter++) {
			cout << iter->first << endl;
		}*/
		Generator generator1("mips.txt");
		generator1.quad2mips(quadtable);
		generator1.close();
		quadfile.close();

		//打印优化后部分
		Optimizer optimizer(&quadtable);
		optimizer.work();
		quadfile.open("quad_after_optimize.txt");
		for (int i = 0; i < optimizer.quadtable.size(); i++)   //输出优化前中间代码到文件
		{
			QuadRuple q = optimizer.quadtable[i];
			quadfile << q.op << " " << q.arg1 << " " << q.arg2 << " " << q.result << endl;
		}
		Generator generator2("mips_after_optimize.txt");
		generator2.quad2mips(optimizer.quadtable);
		generator2.close();
		quadfile.close();
	}
	else
	{
		cout << "error number : " << error_num << endl;
		cout << "Found some errors! Syntax Analyze abort!" << endl;
	}
	parser.lexer.close();
	system("pause");
	return 0;
}
