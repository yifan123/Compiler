#pragma once
#include <string>
using namespace std;

class QuadRuple {
public:
	string op;
	string arg1;
	string arg2;
	string result;
	QuadRuple();
	QuadRuple(string a, string b, string c, string d) {
		op = a;
		arg1 = b;
		arg2 = c;
		result = d;
	}
	void fill(string op, string arg1, string arg2, string result) {
		this->op = op;
		this->arg1 = arg1;
		this->arg2 = arg2;
		this->result = result;
	}
private:
};