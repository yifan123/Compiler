#include"lexer.h"

int Lexer::isalpa(char a) {
	if (isalpha(a) || a == '_') {
		return 1;
	}
	return 0;
}

void Lexer::backspace() {
	index--;
	if (index < 0) {
		printf("some error happen\n");
		error();
	}
}

Lexer::Lexer(string filename) {
	infile.open(filename);
	index = 0;
	infile.get(ch);
	while (!infile.eof()) {  //将源代码一次性全部读入到buffer中
		if (index > max_source_length) {
			cout << "source file is too big!"<<endl;
			error();
		}
		buffer[index++] = ch;
		infile.get(ch);
	}
	index--;
	while (buffer[index] == '\n' || buffer[index] == ' ' || buffer[index] == '\t') {
		index--;
	}
	buffer[index + 1] = ' ';
	buffer[index + 2] = '\0';
	ch = ' ';
	index = -1;
	line = 1;
	infile.close();

	keyword={
	{"int",INT},{"char",CHAR},{"void",VOID},{"const",CONST},{"if",IF},{"while",WHILE},
	{"switch",SWITCH},{"case",CASE},{"default",DEFAULT},{"return",RETURN},{"scanf",SCANF},{"printf",PRINTF}
	};
	specail_sym={
	{'+',PLUS},{'-',MINUS},{'*',MULT},{'/',DIV},
	{'(',LPARENT},{')',RPARENT},{'[',LBRACK},{']',RBRACK},{'{',LBRACE},{'}',RBRACE},
	{',',COMMA},{';',SEMICOLON},{':',COLON}
	};

}

void Lexer::getch() {
	index++;
	if (buffer[index] == '\0') {
		printf("source programer is not complete\n");
		error();
	}
	ch = buffer[index];
}

void Lexer::getsym() {
	int i;
	char tem;
_next:
	while (ch == ' ' || ch == '\t' || ch == '\n') {
		if (ch == '\n') {
			line++;
		}
		getch();
	}
	token.id = "";
	token.num = 0;
	if (isalpa(ch)) {
		i = 0;
		do {
			token.id += ch;
			getch();
		} while (isalpa(ch) || isdigit(ch));
		if (keyword.count(token.id) > 0) {
			token.sym = keyword[token.id];
		}
		else {
			token.sym = IDENT;
		}
	}
	else if (isdigit(ch)) {
		token.sym = UINTEGERT;
		if (ch == '0') {
			token.num = 0;
			getch();
		}
		else {
			token.num = token.num * 10 + ch - '0';
			getch();
			while (isdigit(ch)) {
				token.num = token.num * 10 + ch - '0';
				getch();
			}
		}
		token.sym = UINTEGERT;
	}
	else if (ch == '\'') {
		getch();
		if (isalpa(ch) || isdigit(ch) || ch == '+' || ch == '-' || ch == '*' || ch == '/') {
			tem = ch;
			getch();
			if (ch == '\'') {
				token.sym = CHARACTER;
				token.id = tem;
				getch();
			}
			else {
				backspace();
				token.sym = NOTYPE;
				token.id = '\'';
			}
		}
		else {
			token.sym = NOTYPE;
			token.id = '\'';
		}
	}
	else if (specail_sym.count(ch) > 0) {
		token.sym = specail_sym[ch];
		token.id = ch;
		getch();
	}
	else if (ch == '<') {
		getch();
		if (ch == '=') {
			token.sym = LEQ;
			token.id = "<=";
			getch();
		}
		else {
			token.sym = LSS;
			token.id = "<";
		}
	}
	else if (ch == '>') {
		getch();
		if (ch == '=') {
			token.sym = GEQ;
			token.id = ">=";
			getch();
		}
		else {
			token.sym = GTR;
			token.id = ">";
		}
	}
	else if (ch == '=') {
		getch();
		if (ch == '=') {
			token.sym = EQL;
			token.id = "==";
			getch();
		}
		else {
			token.sym = BECOMES;
			token.id = "=";
		}
	}
	else if (ch == '!') {
		getch();
		if (ch == '=') {
			token.sym = NEQ;
			token.id = "!=";
			getch();
		}
		else {
			token.sym = NOTYPE;
			token.id = "!";
		}
	}
	else if (ch == '"') {
		getch();
		while (ch == 32 || ch == 33 || (ch >= 35 && ch <= 126)) {
			token.id += ch;
			if (ch == 92) {  //92是'\'，为了去掉mars的转义功能
				token.id += ch;
			}
			getch();
		}
		if (ch == '"') {
			token.sym = STRING;
			getch();
		}
		else {
			token.sym = NOTYPE;
		}
	}
	else {
		token.sym = NOTYPE;
		token.id = ch;
		getch();
	}
	if (token.sym == NOTYPE) {
		goto _next;
	}
	return;
}