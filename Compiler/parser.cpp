#include"parser.h"
#include"SymTable.h"
//#include"SymTableItem.h"
#include"QuadRuple.h"
using namespace std;
//#define __DEBUG
#ifdef __DEBUG
//#define DEBUG(info)    printf(info)
//#define DEBUG (printf("(%d)-<%s>: ",__LINE__, __FUNCTION__), printf)
#define DEBUG printf("(%d)---: ",lexer.line);printf
#else
#define DEBUG(info)
#endif
#define HAS_RETURN 1
extern vector<QuadRuple> quadtable;
//extern SymTable symtable;
extern vector<SymTable>symtables;
//extern int subsymtables_index;
extern map<int, string>stringtab;
extern int string_index;
extern int global;
int runstack_index = 0;
//int quadtable_index = 0;
typedef set<Symbol> symset;
symset relation_operation={ LSS, LEQ, GTR, GEQ, NEQ, EQL, BECOMES };
symset exprbegsys = {PLUS,MINUS,IDENT,LPARENT,UINTEGERT,CHARACTER};
symset statbegsys = { IF,WHILE,IDENT,SCANF,PRINTF,SWITCH,RETURN,SEMICOLON,LBRACE };
string cur_fucname;  //��ǰ������
int cur_fun_symtab = 0;//0����������

//int value=0;

TabType type;
int number;
int tem_symbol_index = 0,lab_index=0,tvar_count=0;
//�����ڷ��ű����һ�ǰ�������ű���symtbles�е�index����û��Ҫ�����ﲹ��symtbles_index��
int find_sym_table(string sym, SymTableItem& item,int &aglobal,int *symtbles_index=NULL) {  
	//�Ҳ���������-1
	vector<SymTableItem> items = symtables[cur_fun_symtab].items;
	for (int i = 0; i < items.size(); i++) {
		if (items[i].name == sym) {
			item = items[i];
			aglobal = 0;
			if (symtbles_index) {
				*symtbles_index = cur_fun_symtab;
			}
			return i;
		}
	}
	vector<SymTableItem> items1 = symtables[0].items;
	for (int i = 0; i < items1.size(); i++) {
		if (items1[i].name == sym) {
			item = items1[i];
			aglobal = 1;
			if (symtbles_index) {
				*symtbles_index = cur_fun_symtab;
			}
			return i;
		}
	}
	return -1;
}
//��������
int find_sym_table_declare(string sym, SymTableItem& item) {
	//�Ҳ���������-1
	vector<SymTableItem> items = symtables[cur_fun_symtab].items;
	for (int i = 0; i < items.size(); i++) {
		if (items[i].name == sym) {
			item = items[i];
			return i;
		}
	}
	return -1;
}

string newlab() {
	string a = "Lab";
	string b = to_string(lab_index++);
	return a + b;
}
string newtvar() {   //������ջ���½���ʱ����
	string a = "@";
	string b = to_string(lab_index++);
	string name = a + b;
	symtables[cur_fun_symtab].enter(name, varobj, inttype, runstack_index, 0);
	runstack_index += 4;
	return name;
}
string newpvar() {
	string a = "#";
	string b = to_string(lab_index++);
	string name = a + b;
	symtables[cur_fun_symtab].enter(name, varobj, inttype, runstack_index, 0);
	runstack_index += 4;
	return name;
}
string newtconst(int value) {   //�ڷ��ű����½���ʱ����
	string a = "@";
	string b = to_string(lab_index++);
	string name = a + b;
	symtables[cur_fun_symtab].enter(name, constantobj, inttype, value, 0);
	return name;
}


/*	inttype		0
	chartype	1
	voidtype	2
*/
string TabType2str(TabType type) {
	type == inttype ? 0 : (type == chartype ? 1 : 2);
	return to_string(type);
}
//void enter_quadtable(QuadRuple quadruple) {
//	quadtable.push_back(quadruple);
//	quadtable_index++;
//}

void Parser::program() {
	//int tem_runstack_index;
	//int tem_debug = 0;
	runstack_index = 0;
	//����������
	SymTable symtab;
	cur_fun_symtab = 0;//������
	symtables.push_back(symtab);
	//subsymtables_index = -1;
	if (lexer.token.sym == CONST) {
		const_declare();
	}
	//�����������
	if (lexer.token.sym == INT || lexer.token.sym == CHAR) {
		var_declare();
	}
	if (symtables[0].enter("@program", funcobj, voidtype, 0, 0, runstack_index) < 0) {  //��ҪĿ���ǵǼ�ȫ�ֱ�����ռ����ջ��С
		error(lexer.line, MULTI_ID_DEF);
		skip();
	}
	//����������
	int tem_debug = 0;
	while (lexer.token.sym == INT || lexer.token.sym == CHAR || lexer.token.sym == VOID) {
		if (lexer.token.sym == VOID) {
			lexer.save_scene();
			lexer.getsym();
			if (lexer.token.id == "main") {
				lexer.retrive_scene();
				break;
			}
			lexer.retrive_scene();
		}
		fun_def();
		tem_debug ++ ;
	}
	//����������
	if (lexer.token.sym != VOID) {
		error(lexer.line, 0);
		skip();
	}
	main_def();
	if (!lexer.to_buffer_end()) {
		error(lexer.line, 1);
		skip();
	}
	DEBUG("This is a program��\n");
}
void Parser::main_def() {
	test();
	if (lexer.token.sym != VOID) {
		error(lexer.line, 0);
		skip();
	}
	/*
	else {
		lexer.getsym();
		if (lexer.token.sym != IDENT || lexer.token.id != "main") {
			error(lexer.line, );
		}
		else {
			lexer.getsym();
			if (lexer.token.sym != LPARENT) {
				error(lexer.line, );
			}
			else {
				lexer.getsym();
				if (lexer.token.sym != RPARENT) {
					error(lexer.line, );
				}
				else {
					lexer.getsym();
					if (lexer.token.sym != LBRACE) {
						error(lexer.line, );
					}
					else {
						lexer.getsym();
						if (lexer.token.sym != LBRACE) {
							error(lexer.line, );
						}
						else {
							lexer.getsym();
							compound_statement();
							if (lexer.token.sym != RBRACE) {
								error(lexer.line, );
							}
							else {
								cout << "paser success!" << endl;
							}
						}
					}
				}
			}
		}
	}
	*/
	lexer.getsym();
	if (lexer.token.id != "main") {
		error(lexer.line, 2);
		skip();
	}
	lexer.getsym();
	if (lexer.token.sym != LPARENT) {
		error(lexer.line, 3);
		skip();
	}
	lexer.getsym();
	if (lexer.token.sym != RPARENT) {
		error(lexer.line, 4);
		skip();
	}
	lexer.getsym();
	if (lexer.token.sym != LBRACE) {
		error(lexer.line, 5);
		skip();
	}
	cur_fun_symtab++;//Ϊ��ǰ���������µķ��ű�,��main����Ҳ�����Ӻ���
	SymTable symtab;
	symtables.push_back(symtab);
	cur_fucname = "main";
	int tem_symbol_index = symtables[0].offset;
	runstack_index = 0;
	runstack_index += 4;//Ϊ$ra���Ĵ��������������ַ
	if (symtables[0].enter("main", funcobj, voidtype, cur_fun_symtab, 0) < 0) {
		error(lexer.line, MULTI_ID_DEF);
		skip();
	}
	quadtable.push_back(QuadRuple("FUNC", "", "", cur_fucname));//��pascal-s��һ����C0����Ҫ���溯�������
	lexer.getsym();
	compound_statement();
	symtables[0].items[tem_symbol_index].func_size = runstack_index;
	if (lexer.token.sym != RBRACE) {
		error(lexer.line, 6);
		skip();
	}
	//lexer.getsym();
	DEBUG("This is a main_def��\n");
}
void Parser::const_declare() {
	test();
	do {
		if (lexer.token.sym == CONST) {
			lexer.getsym();
			const_def();
		}
		else {
			error(lexer.line, 8);
			skip();
		}
		if (lexer.token.sym == SEMICOLON) {
			lexer.getsym();
		}
		else {
			error(lexer.line, 7);
			skip();
		}
	} while (lexer.token.sym == CONST);
	test();
	DEBUG("This is a const_declare��\n");
}
void Parser::const_def() {
	test();
	int value;
	string name;
	if (lexer.token.sym == INT) {
		do {
			lexer.getsym();
			if (lexer.token.sym != IDENT) {
				error(lexer.line, 9);
				skip();
			}
			name = lexer.token.id;
			lexer.getsym();
			if (lexer.token.sym != BECOMES) {
				error(lexer.line, 10);
				skip();
			}
			lexer.getsym();
			integer(value);
			if (symtables[cur_fun_symtab].enter_declare(name, constantobj, inttype, value, 0) < 0) {
				error(lexer.line, MULTI_ID_DEF);
				skip();
			}
		} while (lexer.token.sym == COMMA);
	}
	else if (lexer.token.sym == CHAR) {
		do {
			lexer.getsym();
			if (lexer.token.sym != IDENT) {
				error(lexer.line, 9);
				skip();
			}
			name = lexer.token.id;
			lexer.getsym();
			if (lexer.token.sym != BECOMES) {
				error(lexer.line, 10);
				skip();
			}
			lexer.getsym();
			if (lexer.token.sym != CHARACTER) {
				error(lexer.line, 11);
				skip();
			}
			if (symtables[cur_fun_symtab].enter_declare(name, constantobj, chartype, lexer.token.id[0], 0) < 0) {
				error(lexer.line, MULTI_ID_DEF);
				skip();
			}
			lexer.getsym();
		} while (lexer.token.sym == COMMA);
	}
	else {
		error(lexer.line, 12);
		skip();
	}
	test();
	DEBUG("This is a const_def��\n");

}
void Parser::var_declare(){
	test();
	do{
		lexer.save_scene();
		var_def();
		if (lexer.token.sym == LPARENT) {
			lexer.retrive_scene(); //��������
			break;
		}
		if (lexer.token.sym == SEMICOLON) {
			lexer.getsym();
		}
		else {
			error(lexer.line, 7);
			skip();
		}
	} while (lexer.token.sym == INT|| lexer.token.sym == CHAR);
	test();
	DEBUG("This is a var_declare��\n");
}
void Parser::var_def() {
	test();
	string name;
	int value = 0;
	if (lexer.token.sym != INT && lexer.token.sym != CHAR) {
		error(lexer.line, 12);
		skip();
	}
	type = lexer.token.sym == INT ? inttype : chartype;
	do {
		lexer.getsym();
		if (lexer.token.sym != IDENT) {
			error(lexer.line,9 );
			skip();
		}
		name = lexer.token.id;
		lexer.getsym();
		if (lexer.token.sym == LPARENT) {  //��������
			return;
		}
		//��������
		if (lexer.token.sym == LBRACK) {
			lexer.getsym();
			if (lexer.token.sym != UINTEGERT) {
				error(lexer.line, 13);
				skip();
			}
			value = lexer.token.num;
			lexer.getsym();
			if (lexer.token.sym != RBRACK) {
				error(lexer.line, 14);
				skip();
			}
			if (symtables[cur_fun_symtab].enter_declare(name, arrayobj, type, runstack_index, value) < 0) {
				error(lexer.line, MULTI_ID_DEF);
				skip();
			}
			runstack_index += 4 * value;
			lexer.getsym();
		}
		else {  //������ͨ����
			if (symtables[cur_fun_symtab].enter_declare(name, varobj, type, runstack_index, 0) < 0) {
				error(lexer.line, MULTI_ID_DEF);
				skip();
			}
			runstack_index += 4;
		}
		
	} while (lexer.token.sym == COMMA);
	test();
	DEBUG("This is a var_def��\n");
}
void Parser::fun_def() {
	test();
	string name;
	if (lexer.token.sym != INT && lexer.token.sym != CHAR&& lexer.token.sym != VOID) {
		error(lexer.line, 15);
		skip();
	}
	cur_fun_symtab++;//Ϊ��ǰ���������µķ��ű�
	SymTable symtab;
	symtables.push_back(symtab);
	if (lexer.token.sym == INT) type = inttype;
	else if (lexer.token.sym == CHAR) type = chartype;
	else type = voidtype;
	lexer.getsym();
	if (lexer.token.sym != IDENT) {
		error(lexer.line,9 );
		skip();
	}
	name = lexer.token.id;
	cur_fucname = name;
	lexer.getsym();
	if (lexer.token.sym !=LPARENT) {
		error(lexer.line, 3);
		skip();
	}
	int tem_symbol_index = symtables[0].offset;
	if (symtables[0].enter(name, funcobj, type, cur_fun_symtab, 0) < 0) {
		error(lexer.line, MULTI_ID_DEF);
		skip();
	}
	lexer.getsym();
	runstack_index = 0;//����ջ���㣬�൱��Ϊ�Ӻ��������µ�����ջ
	runstack_index += 36;//Ϊ$ra���Ĵ��������������ַ
	number=parmeter_list(cur_fun_symtab);
	symtables[0].items[tem_symbol_index].number = number;
	
	if (lexer.token.sym != RPARENT) {
		error(lexer.line, 4);
		skip();
	}
	lexer.getsym();
	if (lexer.token.sym != LBRACE) {
		error(lexer.line, 5);
		skip();
	}
	lexer.getsym();
	quadtable.push_back(QuadRuple("FUNC", "", "", name));//��pascal-s��һ����C0����Ҫ���溯�������
	compound_statement();
	symtables[0].items[tem_symbol_index].func_size = runstack_index;
	if (lexer.token.sym != RBRACE) {
		error(lexer.line, 6);
		skip();
	}
	//subsymtables_index++;
	lexer.getsym();
	test();
	DEBUG("This is a fun_def��\n");
}
int Parser::parmeter_list(int subsymtable_index) {
	test();
	string name;
	int number = 0;
	if(lexer.token.sym == INT || lexer.token.sym == CHAR) {
		do {
			if (lexer.token.sym == COMMA) {
				lexer.getsym();
			}
			if (lexer.token.sym != INT && lexer.token.sym != CHAR) {
				error(lexer.line, 12);
				skip();
			}
			type = lexer.token.sym == INT ? inttype : chartype;
			lexer.getsym();
			if (lexer.token.sym != IDENT) {
				error(lexer.line, 9);
				skip();
			}
			name = lexer.token.id;
			if (symtables[subsymtable_index].enter_declare(name, varobj, type, runstack_index, 0) < 0) {
				error(lexer.line, MULTI_ID_DEF);
				skip();
			}
			runstack_index += 4;
			lexer.getsym();
			number++;
		} while (lexer.token.sym == COMMA);
	}
	test();
	DEBUG("This is a parmeter_list��\n");
	return number;
}
void Parser::compound_statement() {
	test();
	//����������
	if (lexer.token.sym == CONST) {
		const_declare();
	}
	//�����������
	if (lexer.token.sym == INT || lexer.token.sym == CHAR) {
		var_declare();
	}
	int has_return=statement_list();
	SymTableItem symitem;
	if (find_sym_table(cur_fucname, symitem,global) < 0) {   //�ڷ��ű��в鲻����ʶ��
		error(lexer.line, NON_REACH_FUNC);
		skip();
	}
	//����Ҫ����Ƿ���return��䣬ֱ�ӼӸ�return����
	//if (symitem.type != voidtype && has_return != HAS_RETURN) {
	//	error(lexer.line, MISSING_RETURN);
	//	skip();
	//}
	//else if (symitem.type == voidtype&&has_return != HAS_RETURN) {   //�޷���ֵ����������û��return���
	//	quadtable.push_back(QuadRuple("RET", "", "", ""));
	//}
	quadtable.push_back(QuadRuple("RET", "", "", ""));
	test();
	DEBUG("This is a compound_statement��\n");
}
int Parser::statement_list() {
	int has_return = HAS_RETURN-1;
	test();
	symset::iterator iter = statbegsys.find(lexer.token.sym);
	while (iter != statbegsys.end()) {
		if (statement() == HAS_RETURN) {
			has_return = HAS_RETURN;
		}
		iter = statbegsys.find(lexer.token.sym);
	}
	test();
	DEBUG("This is a statement_list��\n");
	return has_return;
}
//�����Ƿ���return���
int Parser::statement() {
	int has_return = HAS_RETURN - 1;
	string fun_name = "";
	TabType fun_type;
	switch (lexer.token.sym){
	case IF: {
		if_statement();
		break; 
	}
	case WHILE: {
		while_statement();
		break; 
	}
	case LBRACE: {
		lexer.getsym();
		statement_list();
		if (lexer.token.sym != RBRACE) {
			error(lexer.line, 6);
			skip();
		}
		lexer.getsym();
		break;
	}
	/*����ʶ����'('��ֵ������')'
	  ����ʶ�����������ʽ��
	  ����ʶ����'['�����ʽ��']'=�����ʽ��
	*/
	case IDENT: {
		lexer.save_scene();
		//int tem_symbol_index = symtable.find_sym_table(name);
		//if (tem_symbol_index == -1) {   //�ڷ��ű��в鲻����ʶ��
		//	error(lexer.line, UNDEF_ID);
		//	skip();
		//}
		//name = lexer.token.id;
		lexer.getsym();
		switch (lexer.token.sym) {
		case LPARENT: {
			/*enter_quadtable(QuadRuple("CALL", name, "", ""));
			lexer.getsym();
			value_parmeterlist();
			if (lexer.token.sym != RPARENT) {
				error(lexer.line, 4);
				skip();
			}
			lexer.getsym();
			if (lexer.token.sym != SEMICOLON) {
				error(lexer.line, 7);
				skip();
			}
			lexer.getsym();*/
			lexer.retrive_scene();
			callfun_statement(fun_name,fun_type);
			if (lexer.token.sym != SEMICOLON) {
				error(lexer.line, 7);
				skip();
			}
			lexer.getsym();
			break;
		}
		case BECOMES:
		case LBRACK:
		{/*
			lexer.getsym();
			expression();
			if (lexer.token.sym != SEMICOLON) {
				error(lexer.line, 7);
				skip();
			}
			lexer.getsym();
			break;*/
			lexer.retrive_scene();
			assign_statement();
			if (lexer.token.sym != SEMICOLON) {
				error(lexer.line, 7);
				skip();
			}
			lexer.getsym();
			break;
		}
		/*case LBRACK: {
			lexer.getsym();
			expression();
			if (lexer.token.sym != RBRACK) {
				error(lexer.line, 14);
				skip();
			}
			lexer.getsym();
			if (lexer.token.sym != BECOMES) {
				error(lexer.line, 10);
				skip();
			}
			lexer.getsym();
			expression();
			if (lexer.token.sym != SEMICOLON) {
				error(lexer.line, 7);
				skip();
			}
			lexer.getsym();
			break;
		}*/
		default: {
			error(lexer.line, 27);
			skip();
		}
		}
		break;
	}
	case SCANF: {
		read_statement();
		if (lexer.token.sym != SEMICOLON) {
			error(lexer.line, 7);
			skip();
		}
		lexer.getsym();
		break;
	}
	case PRINTF: {
		write_statement();
		if (lexer.token.sym != SEMICOLON) {
			error(lexer.line, 7);
			skip();
		}
		lexer.getsym();
		break;
	}
	case SWITCH: {
		switch_statement();
		break; 
	}
	case RETURN: {
		return_statement();
		if (lexer.token.sym != SEMICOLON) {
			error(lexer.line, 7);
			skip();
		}
		has_return = HAS_RETURN;
		lexer.getsym();
		break;
	}
	case SEMICOLON: {
		lexer.getsym();
		break;
	}
	default:{
		error(lexer.line, 16);
		skip();
	}
	}
	DEBUG("This is a statement��\n");
	return has_return;
}
void Parser::if_statement() {
	if (lexer.token.sym != IF) {
		error(lexer.line, 17);
		skip();
	}
	lexer.getsym();
	if (lexer.token.sym != LPARENT) {
		error(lexer.line, 3);
		skip();
	}
	lexer.getsym();
	string label = newlab();
	condition(label);
	if (lexer.token.sym != RPARENT) {
		error(lexer.line, 4);
		skip();
	}
	lexer.getsym();
	statement();
	quadtable.push_back(QuadRuple("LAB", "", "", label));
	DEBUG("This is a if_statement��\n");
}
void Parser::condition(string label) {
	TabType ctype;
	string exp1_name = "", exp2_name = "";
	expression(exp1_name,ctype);
	Symbol tem = lexer.token.sym;
	symset::iterator iter = relation_operation.find(tem);
	if (iter!=relation_operation.end()) {
		lexer.getsym();
		expression(exp2_name, ctype);
		switch (tem) {
		case LSS:quadtable.push_back(QuadRuple("GEQ", exp1_name, exp2_name, label)); break;
		case LEQ:quadtable.push_back(QuadRuple("GTR", exp1_name, exp2_name, label)); break;
		case GTR:quadtable.push_back(QuadRuple("LEQ", exp1_name, exp2_name, label)); break;
		case GEQ:quadtable.push_back(QuadRuple("LSS", exp1_name, exp2_name, label)); break;
		case NEQ:quadtable.push_back(QuadRuple("EQL", exp1_name, exp2_name, label)); break;
		case EQL:quadtable.push_back(QuadRuple("NEQ", exp1_name, exp2_name, label)); break;
		default:break;
		}
	}
	else {
		quadtable.push_back(QuadRuple("EQL", exp1_name, "$0", label));
	}
	DEBUG("This is a condition��\n");
}
void Parser::while_statement() {
	if (lexer.token.sym != WHILE) {
		error(lexer.line, 18);
		skip();
	}
	lexer.getsym();
	if (lexer.token.sym != LPARENT) {
		error(lexer.line, 3);
		skip();
	}
	lexer.getsym();
	string label1 = newlab();
	string label2 = newlab();
	quadtable.push_back(QuadRuple("LAB", "", "", label1));
	condition(label2);
	if (lexer.token.sym != RPARENT) {
		error(lexer.line, 4);
		skip();
	}
	lexer.getsym();
	statement();
	quadtable.push_back(QuadRuple("JMP", "", "", label1));
	quadtable.push_back(QuadRuple("LAB", "", "", label2));
	DEBUG("This is a while_statement��\n");
}
//����ʶ����'('��ֵ������')'
void Parser::callfun_statement(string &fun_value, TabType &fun_type) {
	string func_name = "", para_name = "",exp_name="";
	TabType ptype;
	int form_para_num = 0, real_para_num = 0;
	SymTableItem symitem;
	vector<string> save_para;
	func_name = lexer.token.id;
	if (find_sym_table(func_name, symitem,global)<0) {   //�ڷ��ű��в鲻����ʶ��
		error(lexer.line, UNDEF_ID);
		skip();
	}
	form_para_num = symitem.number;
	fun_type = symitem.type;
	lexer.getsym();
	if (lexer.token.sym != LPARENT) {
		error(lexer.line, 3);
		skip();
	}
	lexer.getsym();
	symset::iterator iter = exprbegsys.find(lexer.token.sym);
	if (iter != exprbegsys.end()) {
		do {
			if (lexer.token.sym == COMMA) {
				lexer.getsym();
			}
			expression(exp_name,ptype);
			//if (ptype != symtables[symtable.items[tem_symbol_index].adr].items[0].type) {
			if (ptype != symtables[symitem.adr].items[real_para_num].type) {
				error(lexer.line, FUNC_RPARA_TYPE_ERROR);
				skip();
			}
			save_para.push_back(exp_name);
			real_para_num++;
		} while (lexer.token.sym == COMMA);
	}
	if (form_para_num != real_para_num) {
		error(lexer.line, FUNC_RPARA_NUM_ERROR);
		skip();
	}
	else {
		for (int i = 0; i < save_para.size(); i++) {
			quadtable.push_back(QuadRuple("PARA", func_name, to_string(i), save_para[i]));
		}	
	}
	quadtable.push_back(QuadRuple("CALL", func_name, "", ""));
	if (fun_type != voidtype) {
		fun_value=newtvar();
		quadtable.push_back(QuadRuple("SW", "$v0","", fun_value));
	}
	if (lexer.token.sym != RPARENT) {
		error(lexer.line, 3);
		skip();
	}
	lexer.getsym();
}
//����ʶ�����������ʽ��
//����ʶ����'['�����ʽ��']' = �����ʽ��
void Parser::assign_statement() {
	string offset_name = "", exp_name = "",arg2="",op_name="ASS";
	TabType offset_type, exp_type;
	string name = lexer.token.id;
	SymTableItem symitem;
	if (find_sym_table(name,symitem,global) <0) {   //�ڷ��ű��в鲻����ʶ��
		error(lexer.line, UNDEF_ID);
		skip();
	}
	TabObj kind = symitem.obj;
	if (kind != varobj && kind != arrayobj) {
		error(lexer.line, ASSIGN_ERROR);	//���Ǳ������͸�ֵ
		skip();
	}
	lexer.getsym();
	//�������鸳ֵ
	if (lexer.token.sym == LBRACK) {
		lexer.getsym();
		expression(offset_name, offset_type); //�õ���������
		//Fix �����±����ͺ��Ƿ�Խ����
		if (lexer.token.sym != RBRACK) {
			error(lexer.line, 14);
			skip();
		}
		arg2 = offset_name;
		op_name = "ARAS";
		lexer.getsym();
	}
	if (lexer.token.sym != BECOMES) {
		error(lexer.line, 10);
		skip();
	}
	lexer.getsym();
	expression(exp_name, exp_type);
	quadtable.push_back(QuadRuple(op_name,exp_name,arg2,name));
}

void Parser::read_statement() {
	SymTableItem sym;
	if (lexer.token.sym != SCANF) {
		error(lexer.line, 19);
		skip();
	}
	lexer.getsym();
	if (lexer.token.sym != LPARENT) {
		error(lexer.line, 3);
		skip();
	}
	do {
		lexer.getsym();
		if (lexer.token.sym != IDENT) {
			error(lexer.line, 9);
			skip();
		}
		string name = lexer.token.id;
		if (find_sym_table(name,sym,global) <0) {   //�ڷ��ű��в鲻����ʶ��
			error(lexer.line, UNDEF_ID);
			skip();
		}
		TabObj kind = sym.obj;
		if (kind != varobj) {
			error(lexer.line, ASSIGN_ERROR);	//ֻ���Ǳ������ͣ�������Ҳ����
			skip();
		}
		quadtable.push_back(QuadRuple("SCF", "","" , name));
		lexer.getsym();
	} while (lexer.token.sym == COMMA);
	if (lexer.token.sym != RPARENT) {
		error(lexer.line, 4);
		skip();
	}
	lexer.getsym();
	DEBUG("This is a read_statement��\n");
}
void Parser::write_statement() {
	string exp_name = "";
	TabType exp_type;
	if (lexer.token.sym != PRINTF) {
		error(lexer.line, 20);
		skip();
	}
	lexer.getsym();
	if (lexer.token.sym != LPARENT) {
		error(lexer.line, 3);
		skip();
	}
	/*��д��䣾 ::= printf '(' ���ַ�����,�����ʽ�� ')'|
					printf '('���ַ����� ')'| 
					printf '('�����ʽ��')'*/
	lexer.getsym();
	symset::iterator iter = exprbegsys.find(lexer.token.sym);
	if (iter != exprbegsys.end()) {
		expression(exp_name, exp_type);
		quadtable.push_back(QuadRuple("PRF", exp_name, TabType2str(exp_type), "1")); //�������
	}
	else if (lexer.token.sym == STRING) {
		stringtab[string_index++] = lexer.token.id;
		lexer.getsym();
		if (lexer.token.sym == COMMA) {
			quadtable.push_back(QuadRuple("PRF", to_string(string_index-1), "str", "0")); //���������
			lexer.getsym();
			expression(exp_name, exp_type);
			quadtable.push_back(QuadRuple("PRF", exp_name, TabType2str(exp_type), "1")); //�������
		}
		else {
			quadtable.push_back(QuadRuple("PRF", to_string(string_index - 1), "str", "1")); //�������
		}
	}
	else {
		error(lexer.line, 26);
		skip();
	}
	if (lexer.token.sym != RPARENT) {
		error(lexer.line, 4);
		skip();
	}
	lexer.getsym();
	DEBUG("This is a write_statement��\n");
}
/*
�������䣾  ::=  switch '('�����ʽ��')' '{'���������ȱʡ�� '}'
�������   ::=  ���������䣾{���������䣾}
���������䣾  ::=  case��������������䣾
��ȱʡ��   ::=  default : ����䣾|���գ�
*/
void Parser::switch_statement() {
	string exp_name = "";
	TabType exp_type;
	string labout = newlab();
	if (lexer.token.sym != SWITCH) {
		error(lexer.line, 21);
		skip();
	}
	lexer.getsym();
	if (lexer.token.sym != LPARENT) {
		error(lexer.line, 3);
		skip();
	}
	lexer.getsym();
	expression(exp_name, exp_type);
	if(exp_type!=inttype&& exp_type != chartype) {
		error(lexer.line, VAR_TYPE_ERROR);
		skip();
	}
	if (lexer.token.sym != RPARENT) {
		error(lexer.line, 4);
		skip();
	}
	lexer.getsym();
	if (lexer.token.sym != LBRACE) {
		error(lexer.line, 5);
		skip();
	}
	lexer.getsym();
	case_list(exp_name,labout);
	default_list();
	if (lexer.token.sym != RBRACE) { 
		error(lexer.line, 6);
		skip();
	}
	quadtable.push_back(QuadRuple("LAB", "", "", labout));
	lexer.getsym();
	DEBUG("This is a switch_statement��\n");
}
/*
�������   ::=  ���������䣾{���������䣾}
���������䣾  ::=  case��������������䣾
*/
void Parser::case_list(string exp_name, string labout) {
	string caselab = "", convalue = "", con_name = "", label = "";
	TabType con_type;
	do {
		if (lexer.token.sym != CASE) {
			error(lexer.line, 22);
			skip();
		}
		lexer.getsym();
		constant(con_name, con_type);
		if (lexer.token.sym != COLON) {
			error(lexer.line, 23);
			skip();
		}
		label = newlab();
		quadtable.push_back(QuadRuple("NEQ", exp_name, con_name, label));
		lexer.getsym();
		statement();
		quadtable.push_back(QuadRuple("JMP", "", "", labout));
		quadtable.push_back(QuadRuple("LAB", "", "", label));
	} while (lexer.token.sym == CASE);
	DEBUG("This is a case_list��\n");
}
//��ȱʡ�� ::= default : ����䣾|���գ�
void Parser::default_list() {
	if (lexer.token.sym == DEFAULT) {
		lexer.getsym();
		if (lexer.token.sym != COLON) {
			error(lexer.line, 23);
			skip();
		}
		lexer.getsym();
		statement();
	}
	DEBUG("This is a default_list��\n");
}
//��������䣾 ::= return['('�����ʽ��')']
void Parser::return_statement() {
	string exp_name = "";
	TabType exp_type;
	SymTableItem symitem;
	if (lexer.token.sym != RETURN) {
		error(lexer.line, 24);
		skip();
	}
	lexer.getsym();
	if (lexer.token.sym == LPARENT) {
		lexer.getsym();
		expression(exp_name, exp_type);
		if (lexer.token.sym != RPARENT) {
			error(lexer.line, 4);
			skip();
		}
		//Fix ��Ҫ��return�Ĳ��������Ƿ�ƥ����м��
		find_sym_table(cur_fucname, symitem,global);
		if (symitem.type == voidtype) {
			error(lexer.line, VOID_RETURN_ERROR);//void �������ܴ�����ֵ
			skip();
		}
		quadtable.push_back(QuadRuple("RET", exp_name, "", ""));
		lexer.getsym();
	}
	else {
		quadtable.push_back(QuadRuple("RET", "", "", ""));
	}
	DEBUG("This is a return_statement��\n");
}
//�����ʽ�� ::=�ۣ������ݣ��{���ӷ�����������}
void Parser::expression(string &exp_name, TabType &exp_type) {
	string item_name = "", tem_name = "", dst_name = "";
	TabType item_type;
	string op;
	Symbol tem_sym = PLUS;
	SymTableItem item_sym;
	int item_g;
	int type_flag = 0;//ֻҪ���ʽ�г��������㣬flag����Ϊ1���������ձ��ʽ����Ϊint
	if (lexer.token.sym == PLUS || lexer.token.sym == MINUS) {
		type_flag = 1;
		tem_sym = lexer.token.sym;
		lexer.getsym();
	}
	item(item_name, item_type);
	if (item_type != inttype && item_type != chartype) {
		error(lexer.line, OPRAND_TYPE_ERROR);
		skip();
	}
	else{
		dst_name = item_name;
		if (tem_sym == MINUS) {
			type_flag = 1;
			find_sym_table(dst_name, item_sym, item_g);
			if (item_sym.obj == constantobj) {
				dst_name = newtconst(-item_sym.adr);
			}
			else {
				dst_name = newtvar();
				quadtable.push_back(QuadRuple("NEG", item_name, "", dst_name));
			}
		}
	}
	tem_name = dst_name;
	while (lexer.token.sym == PLUS || lexer.token.sym == MINUS)
	{
		type_flag = 1;
		op = lexer.token.sym == PLUS ? "ADD" : "SUB";
		lexer.getsym();
		item(item_name, item_type);
		if (item_type != inttype && item_type != chartype) {
			error(lexer.line, OPRAND_TYPE_ERROR);
			skip();
		}
		SymTableItem symitem1;
		SymTableItem symitem2;
		SymTableItem symitem3;
		int g1;
		int g2;
		int g3;
		int result=0;
		find_sym_table(tem_name, symitem1, g1);
		find_sym_table(item_name, symitem2, g2);
		if (symitem1.obj == constantobj && symitem2.obj == constantobj) {
			if (op == "ADD") {	//�ӷ�
				result = symitem1.adr + symitem2.adr;
			}
			else if (op == "SUB") { //����
				result = symitem1.adr - symitem2.adr;
			}
			dst_name = newtconst(result);
		}
		else {
			dst_name = newtvar();
			quadtable.push_back(QuadRuple(op, tem_name, item_name, dst_name));
		}
		tem_name = dst_name;
	}
	exp_name = dst_name;
	exp_type = type_flag == 1 ? inttype : item_type;
	DEBUG("This is a expression��\n");
}
//��� ::= �����ӣ�{���˷�������������ӣ�}
void Parser::item(string &item_name, TabType &item_type) {
	string factor_name = "", tem_name = "", dst_name = "";
	TabType factor_type;
	TabObj factor_obj;
	string op;
	int type_flag = 0;//ֻҪ�����г��������㣬flag����Ϊ1������������������Ϊint
	factor(factor_name, factor_type);
	tem_name = factor_name;
	dst_name = factor_name;
	SymTableItem symitem1;
	SymTableItem symitem2;
	int g1;
	int g2;
	int result=0;
	while (lexer.token.sym == MULT || lexer.token.sym == DIV)
	{
		type_flag = 1;
		op = lexer.token.sym == MULT ? "MUL" : "DIV";
		lexer.getsym();
		factor(factor_name, factor_type);
		if (factor_type != inttype && factor_type != chartype) {
			error(lexer.line, OPRAND_TYPE_ERROR);
			skip();
		}
		find_sym_table(tem_name, symitem1, g1);
		find_sym_table(factor_name, symitem2, g2);
		if (symitem1.obj == constantobj && symitem2.obj == constantobj) {
			if (op == "MUL") {	//�ӷ�
				result = symitem1.adr * symitem2.adr;
			}
			else if (op == "DIV") { //����
				result = symitem1.adr / symitem2.adr;
			}
			dst_name = newtconst(result);
		}
		else {
			dst_name = newtvar();
			quadtable.push_back(QuadRuple(op, tem_name, factor_name, dst_name));
		}
		tem_name = dst_name;
	}
	item_name = dst_name;
	item_type = type_flag == 1 ? inttype : factor_type;
	DEBUG("This is a item��\n");
}
/*�����ӣ� ::= ����ʶ������
			  ����ʶ����'['�����ʽ��']'|
			  ���з���ֵ����������䣾
			  '('�����ʽ��')'��
			  ��������|
			  ���ַ�����
*/
void Parser::factor(string &fac_name, TabType &fac_type) {
	string tem_name = "", dst_name = "", exp_name = "", ident_name = "";
	TabType tem_type = inttype, exp_type = inttype;
	SymTableItem symitem;
	int value = 0;
	switch (lexer.token.sym) {
	case LPARENT: {
		lexer.getsym();
		expression(exp_name, exp_type);
		if (lexer.token.sym != RPARENT) {
			error(lexer.line, 4);
			skip();
		}
		fac_name = exp_name;
		fac_type = inttype;
		lexer.getsym();
		break;
	}
				 /*����ʶ����'('��ֵ������')'
				   ����ʶ����
				   ����ʶ����'['�����ʽ��']'
				 */
	case IDENT: {
		lexer.save_scene();
		ident_name = lexer.token.id;
		if (find_sym_table(ident_name, symitem,global) < 0) {
			error(lexer.line, UNDEF_ID);
			skip();
		}
		else {
			fac_name = lexer.token.id;
			fac_type = symitem.type;
		}
		lexer.getsym();
		switch (lexer.token.sym) {
		case LPARENT: {
			lexer.retrive_scene();
			callfun_statement(tem_name, tem_type);
			if (tem_type == voidtype) {
				error(lexer.line, FUNC_NO_RET);
				skip();
			}
			else{
				fac_name = tem_name;
				fac_type = tem_type;
			}
			break;
		}
		case LBRACK: {
			lexer.getsym();
			expression(tem_name, tem_type);
			if (tem_type != inttype) {
				error(lexer.line, ARRAY_INDEX_TYPE_ERROR);
				skip();
			}
			//Fix �����±������ͳ���ʱ����Խ����
			if (lexer.token.sym != RBRACK) {
				error(lexer.line, 14);
				skip();
			}
			dst_name = newtvar();
			quadtable.push_back(QuadRuple("ASAR", tem_name, ident_name, dst_name));
			fac_name = dst_name;
			lexer.getsym();
			break;
		}
		}
		break;
	}
	/*��������
	  ���ַ���*/
	case PLUS:
	case MINUS:
	case UINTEGERT:
	case CHARACTER:{	
		constant(fac_name, fac_type);
		break;
	}
	default: {
		error(lexer.line, 25);
		skip();
	}
	}
	DEBUG("This is a factor��\n");
}
void Parser::integer(int &value) {
	int flag = 1;
	if (lexer.token.sym == PLUS || lexer.token.sym == MINUS) {
		flag = lexer.token.sym == PLUS ? 1 : -1;
		lexer.getsym();
	}
	if (lexer.token.sym != UINTEGERT) {
		error(lexer.line, 13);
		skip();
	}
	value = flag * lexer.token.num;
	lexer.getsym();
	DEBUG("This is a integer��\n");
}
//��������   ::=  ��������|���ַ���
//���ַ��� ::=  '���ӷ��������'��'���˷��������'��'����ĸ��'��'�����֣�'
void Parser::constant(string &con_name,TabType &con_type) {
	int value = 0;
	if (lexer.token.sym == PLUS || lexer.token.sym == MINUS|| lexer.token.sym == UINTEGERT) {
		integer(value);
		con_name = newtconst(value);
		con_type = inttype;
	}
	else if(lexer.token.sym == CHARACTER) {
		value = int(lexer.token.id[0]);//�����ַ���ֱ�Ӽ�����asc�룻���õ��ַ�����ʱ��ֱ���ж������ͼ��ɡ�
		con_name = newtconst(value);
		con_type = chartype;
		lexer.getsym();
	}
	else {
		error(lexer.line, 11);
		skip();
	}
	DEBUG("This is a constant��\n");
}