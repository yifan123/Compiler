#pragma once
#include<iostream>
using namespace std;
//���error_code�Ķ���
//�������
#define	UNDEF_ID			    28		//δ������ʶ��
#define	MULTI_ID_DEF		    29		//�ض����ʶ��
#define FUNC_RPARA_TYPE_ERROR   30      //��������ʱ��������ʵ�����Ͳ�ƥ��
#define FUNC_RPARA_NUM_ERROR    31      //��������ʱ��������ʵ�θ�����ƥ��
#define VAR_TYPE_ERROR		    32		//�������ʹ��󣬱���ֻ������int����char����
#define OPRAND_TYPE_ERROR       33      //����������ֻ������int����char����
#define ARRAY_INDEX_TYPE_ERROR  34      //�����±����ʹ��󣬱���Ϊ����
#define	FUNC_NO_RET			    35		//�޷���ֵ���������ڱ��ʽ��ʹ��
#define VOID_RETURN_ERROR	    36		//void������return�ǿ�ֵ
#define NON_REACH_FUNC		    37
#define	MISSING_RETURN		    38	    //��ʧreturn���
#define	OUT_OF_ARRAY		    39		//�����±�Խ��
#define RETURN_TYPE_ERROR       40      //��������ֵ���ʹ���
#define ASS_TYPE_ERROR		    41      //��ֵ���ǰ�����Ͳ�һ��
#define SWITCH_TYPE_ERROR	    42      //switch��case���Ͳ�һ��
#define	CONDITION_TYPE_ERROR    43      //�����еĹ�ϵ�Ƚ�ֻ��������֮�䣬�������ַ��ͣ���������ǵ������ʽ�������������
#define	ASSIGN_ERROR		    44	    //��ֵʱ��������

//�﷨����
//��Ϊ�տ�ʼдparser.cppʱ��û�ú�����ʾ�������ͣ�ֱ��Ӳ���룬���ԾͰ�����ע�͵���

//#define     0      //����������ֵӦΪvoid��
//#define     1      //main����Ӧ�����ڳ���ĩβ
//#define     2      //��ʧmain����
//#define     3      //Ӧ��'('
//#define     4      //Ӧ��')'
//#define     5      //Ӧ��'{'
//#define     6      //Ӧ��'}'
//#define     7      //Ӧ��';'
//#define     8      //ȱ��CONST
//#define     9      //Ӧ�Ǳ�ʶ��
//#define     10     //Ӧ��=	
//#define     11     //Ӧ���ַ�	
//#define     12     //ȱ�����ͱ�ʶ��(int char)	
//#define     13     //Ӧ���޷�������	
//#define     14     //Ӧ��']'	
//#define     15     //ȱ�ٺ������ͱ�ʶ��(int char void)	
//#define     16     //ӦΪ���	
//#define     17     //ӦΪif	
//#define     18     //ӦΪwhile	
//#define     19     //ӦΪscanf	
//#define     20     //ӦΪprintf	
//#define     21     //ӦΪswitch	
//#define     22     //ӦΪcase	
//#define     23     //Ӧ��':'	
//#define     24     //ӦΪreturn	
//#define     25     //Ӧ������	
//#define     26     //Ӧ���ַ���	
//#define     27     //δ�����﷨	


//�ʷ�����ʱ�Ĵ���
//�ʷ������������󲻻ᱨ����������¶���ֱ������һ���Ϸ��Ĵʷ���ɷ�

//һЩ֮ǰд�ĵ�����û���˵Ĵ�������
//#define	ID_TOO_LONG			3		//��ʶ������
//#define	PROGRAM_ERROR		4		//����ṹ����
//#define	STR_CONTENT_ERROR	5		//�ַ������ݴ���
//#define	STR_TOO_LONG		6		//�ַ�������
//#define	ZERO_HEAD_NUM		7		//����������0��ͷ
//#define	NUM_HEAD_IDENT		8		//���ֿ�ͷ�ı�ʶ��
//#define	NUM_TOO_LARGE		9		//����̫��
////#define	UNDEF_INPUT			10		//���Ϸ�����	
//#define	CHAR_MISS_QUOTE		11		//�ַ����Ͷ�ʧ��һ��������'
//#define	CHAR_CONTENT_ERROR	12		//�ַ����ݲ��Ϸ�
//#define	CHAR_OVERFLOW		13		//�ַ����	
//#define	NEQUAL_MISS			14		//����ʹ�ò��Ⱥţ�ֻ�У�û�еȺ�
//#define	MISSING_SEMI		15		//��ʧ�ֺ�
//#define	MISSING_RPARENT		16		//��ʧ������
//#define	MISSING_RBRACK		17		//��ʧ��������
//#define	MISSING_RBRACE		18		//��ʧ�Ҵ�����
//#define	MISSING_IDENT		19		//��ʧ��ʶ��
//#define	ERROR_PARA_NUM		20		//������Ŀ����
//#define	MISSING_LPARENT		21		//��ʧ������
//#define	MISSING_LBRACK		22		//��ʧ��������
//#define	MISSING_LBRACE		23		//��ʧ�������
//#define	MISSING_PLUS		24		//��ʧ�ӺŻ����
//#define	MISSING_MULTI		25		//��ʧ�˺Ż����
//
//#define	RETURN_ERROR		27		//����ֵ����
//#define	MISSING_MAIN		28		//��ʧmain����
//#define	EXPRESSION_ERROR	30		//���ʽ����
////#define	CONST_NOT_INIT		31		//����ʹ��ǰδ��ʼ��
////#define	VAR_NOT_INIT		32		//����ʹ��ǰδ��ʼ��
//
//#define	OUT_OF_TABLE		34		//���ű�����
//#define	WRONG_TYPE			36		//�������ʹ���
//#define	MISSING_ASSIGN		37		//��ʧ�Ⱥ�
//#define MISSING_COLON		38		//��ʧð��
//#define MISSING_VOID		39		//��ʧVOID
//#define MISSING_DOUQUOTE	40		//��ʧ˫����
//#define CASE_TYPE_ERROR		41		//case����������,��ʶ�����Ͳ���ȷ
//#define CONST_TYPE_ERROR	42		//�������ʹ���,����ֻ������int����char����
//#define FACTOR_ERROR		43		//���ӷ������������Ƿ����ʹ�������
//#define SCANF_ERROR			44		//SCANFʹ�ô���
//
//#define UNDEF_FUNC			46		//ʹ����δ����ĺ���
//
//#define ARRAY_LENGTH_ERROR	48		//�������������ʱ��������
//
////���ܹ��ﵽ�Ĵ����֧
//#define NON_REACH_IF		100		
//#define NON_REACH_WHILE		101		
//#define NON_REACH_SWITCH	102
//#define NON_REACH_CASE		103
//#define NON_REACH_SCANF		104
//#define NON_REACH_PRINTF	105
//#define NON_REACH_RETURN	106
//#define NON_REACH_CON		108

void error();//��lexer.cpp��ʹ��
void error(int line, int error_code);//��parser.cpp��ʹ�ã�������������﷨�ϵĴ���
void skip();
void test();