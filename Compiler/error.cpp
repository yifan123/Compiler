#include"error.h"
extern int error_num;
void error() {

}
void error(int line, int error_code) {
	error_num++;
	cout << "at line "<< line << " : "<< "*******error!*****error_code: " <<error_code<< endl;
}
void skip() {

}
void test() {

}