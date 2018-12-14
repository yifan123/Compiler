#pragma once
#pragma once
#include<stdio.h>
#include<ctype.h>
#include <map>
#include <string> 
#include<iostream>
#include<vector>
#include<iomanip>

using namespace std;
//LPARENT:'(', LBRACK:'[', LBRACE:'{', COMMA:',', SEMICOLON:';',COLON:':'
typedef enum {
	INT, CHAR, VOID, IDENT, STRING,
	UINTEGERT, CHARACTER,
	PLUS, MINUS, MULT, DIV,
	LSS, LEQ, GTR, GEQ, NEQ, EQL, BECOMES,
	LPARENT, RPARENT, LBRACK, RBRACK, LBRACE, RBRACE, COMMA, SEMICOLON, COLON,
	CONST, IF, WHILE, SWITCH, CASE, DEFAULT, RETURN, SCANF, PRINTF,
	NOTYPE
}Symbol;

class Token {
public:
	Symbol sym;
	int num;
	string id;
	Token(Symbol _sym=NOTYPE, int _num = 0, string _id = "") {
		sym = _sym;
		num = _num;
		id = _id;
	}
};
