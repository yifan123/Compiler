#include"error.h"
extern int error_num;
void error() {

}
void error(int line, int error_code) {
	error_num++;
	cout << "at line "<< line << ": error_code: "<< error_code<<endl;
	switch (error_code) {
	case 0: cout << "����������ֵӦΪvoid��" << endl;	break;
	case 1: cout << "main����Ӧ�����ڳ���ĩβ" << endl;	break;
	case 2: cout << "��ʧmain����" << endl;	break;
	case 3: cout << "��ʧ'('" << endl;	break;
	case 4: cout << "��ʧ')'" << endl;	break;
	case 5: cout << "��ʧ'{'" << endl;	break;
	case 6: cout << "��ʧ'}'" << endl;	break;
	case 7: cout << "��ʧ';'" << endl;	break;
	case 8: cout << "ȱ��CONST" << endl;	break;
	case 9: cout << "Ӧ�Ǳ�ʶ��" << endl;	break;
	case 10: cout << "Ӧ��=	" << endl;	break;
	case 11: cout << "Ӧ���ַ�	" << endl;	break;
	case 12: cout << "ȱ�����ͱ�ʶ��(int char)	" << endl;	break;
	case 13: cout << "Ӧ���޷�������	" << endl;	break;
	case 14: cout << "��ʧ']'	" << endl;	break;
	case 15: cout << "ȱ�ٺ������ͱ�ʶ��(int char void)	" << endl;	break;
	case 16: cout << "ӦΪ���	" << endl;	break;
	case 17: cout << "ӦΪif	" << endl;	break;
	case 18: cout << "ӦΪwhile	" << endl;	break;
	case 19: cout << "ӦΪscanf	" << endl;	break;
	case 20: cout << "ӦΪprintf	" << endl;	break;
	case 21: cout << "ӦΪswitch	" << endl;	break;
	case 22: cout << "ӦΪcase	" << endl;	break;
	case 23: cout << "��ʧ':'	" << endl;	break;
	case 24: cout << "ӦΪreturn	" << endl;	break;
	case 25: cout << "Ӧ������	" << endl;	break;
	case 26: cout << "Ӧ���ַ���	" << endl;	break;
	case 27: cout << "δ�����﷨	" << endl;	break;
	case UNDEF_ID:cout << "δ������ʶ��" << endl;	break;
	case MULTI_ID_DEF:cout << "�ض����ʶ��" << endl;	break;
	case FUNC_RPARA_TYPE_ERROR:cout << "��������ʱ��������ʵ�����Ͳ�ƥ��" << endl;	break;
	case FUNC_RPARA_NUM_ERROR:cout << "��������ʱ��������ʵ�θ�����ƥ��" << endl;	break;
	case VAR_TYPE_ERROR:cout << "�������ʹ��󣬱���ֻ������int����char����" << endl;	break;
	case OPRAND_TYPE_ERROR:cout << "����������ֻ������int����char����" << endl;	break;
	case ARRAY_INDEX_TYPE_ERROR:cout << "�����±����ʹ��󣬱���Ϊ����" << endl;	break;
	case FUNC_NO_RET:cout << "�޷���ֵ���������ڱ��ʽ��ʹ��" << endl;	break;
	case VOID_RETURN_ERROR:cout << "void������return�ǿ�ֵ" << endl;	break;
	case NON_REACH_FUNC:cout << "�÷�֧���ᵽ��" << endl;	break;
	case MISSING_RETURN:cout << "��ʧreturn���" << endl;	break;
	case OUT_OF_ARRAY:cout << "�����±�Խ��" << endl;	break;
	case RETURN_TYPE_ERROR:cout << "��������ֵ���ʹ���" << endl;	break;
	case ASS_TYPE_ERROR:cout << "��ֵ���ǰ�����Ͳ�һ��" << endl;	break;
	case SWITCH_TYPE_ERROR:cout << "switch��case���Ͳ�һ��" << endl;	break;
	case CONDITION_TYPE_ERROR:cout << "�����еĹ�ϵ�Ƚ�ֻ��������֮�䣬�������ַ��ͣ���������ǵ������ʽ�������������" << endl;	break;
	case ASSIGN_ERROR:cout << "��ֵʱ��������" << endl;	break;
	default:cout << "δ֪����" << endl;
	}
}
void skip() {

}
void test() {

}