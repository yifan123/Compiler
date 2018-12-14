#pragma once
#include<stdio.h>
#include<ctype.h>
#include <map>
#include <string> 
#include<iostream>
#include<vector>
#include<iomanip>
#include<fstream>
#include "token.h"
#include "error.h"
using namespace std;
#define max_source_length 100000

class Lexer{
private:
	char ch;
	int index;
	char buffer[max_source_length];
	struct {
		char ch;
		int index;
		Token token;
	}backup;
	ifstream infile;
	map<std::string, Symbol>keyword;
	map<char, Symbol>specail_sym;
	int isalpa(char a);
	void backspace();
public:
	int line;
	Token token;
	Lexer(string filename);
	void close() {
		infile.close();
	}
	void getch();
	void getsym();
	int to_buffer_end() {
		return buffer[index + 1] == '\0';
	}
	void save_scene() {
		backup.ch = ch;
		backup.index = index;
		backup.token.id = token.id;
		backup.token.num = token.num;
		backup.token.sym = token.sym;
	}
	void retrive_scene() {
		ch = backup.ch;
		index = backup.index;
		token.id = backup.token.id;
		token.num = backup.token.num;
		token.sym = backup.token.sym;
	}
};
