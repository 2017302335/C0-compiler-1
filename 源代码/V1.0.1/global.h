#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include<stdio.h>
#include<stdlib.h>
#define TRUE 1
#define FALSE 0
#define MAXCHILDREN 7

extern FILE * fp_source;
extern FILE * fp_log;

extern int print_log;

extern int linenumber;

typedef enum
{
    PROGRAM , CONSTDECLARE , CONSTSUBDECLARE , IDTYPE , CONSTNUM , CONSTCHAR , CONSTSTRING,
    VARIDECLARE , SUBVARIDECLARE , ARRAYTYPE , FUNCDECLARE , ARGS , ARG , MAINTYPE , COM_STMT ,
    STMT_SEQ , STMT , OPTYPE ,  CALL , ASSIGN_STMT , IF_STME ,
    FOR_STMT , RELATION , VALUE_ARG, WHILE_STMT , SCANF_STMT , PRINTF_STMT , RETURN_STMT
}NodeType;

typedef enum
{
    Void , Integer , Char
}Expkind;


typedef enum
{
    ID,PLUS,MINUS,MULTIP,DIVD,LT,LTAE,BT,BTAE,EQ,NEQ,CHR,STRING,LBB,RBB,LB,RB,DOT,SEMI,ASSIGN,INTGE,
    VOID,INT,CHAR,IF,ELSE,CONST,WHILE,FOR,DO,RETURN,BREAK,CONTINUE,PRINTF,SCANF,MAIN,ERR,ENDFILE,LSB,RSB
}tokentype;

typedef struct ASTNODE
{
    NodeType type;//AST�ڵ�����ͱ��

    union{
        char* name ;//��IDTYPE���ͽڵ㣬��ʶ����ֵ���д洢����������ű�||�Ժ�������ڵ㣬���溯�������֣���������ű�
        int val;//����CONSTNUM���ͽڵ㣬�洢���ֵ�ֵ || ��value_arg�ڵ��¼��������ʱ�ж��ٸ�����
        char ch;//����CONSTCHAR���ͽڵ㣬�洢������
        char* str;//����CONSTSTRING���ͽڵ㣬�洢�ַ�����ֵ
    } attr;
    tokentype ttype;//��¼��+,-,<,>,<=,>=,==,!= || ��for�м�¼�����Ǽӻ��Ǽ�
    struct ASTNODE* childen[MAXCHILDREN];//�ӽڵ�
    struct ASTNODE* sibling;//�ֵܽڵ�
    int lineno;//�����id��Ҫ��¼������������������ű�
    Expkind kind;//������¼id��void,int,char����||����CONSTDECLARE VARIDECLARE���ֽڵ�Ҳʹ�������������������������
    int useful;//���������������¼����exp�����0+��0-�����Ϊfalse���ʾ�ǹ�������ĸ����ڵ㣬����Ҫ���ɴ�������ͼ��
}ASTNODE;


#endif
