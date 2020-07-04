#ifndef _SYMBAL_H
#include "global.h"

#define MaxBukSize 211

typedef enum{
TConst , TVariable , TArray , TFunc , TArg
}entrytype;//���ű���Ŀ������

typedef struct entry//���ű����Ŀ
{
    entrytype type;//��Ŀ������
    char* name;//identifier�����֣�a()��a | int a ��a | const int a ��a | arg
    int addr;//��Ŀ�����ƫ�Ƶ�ַ
    Expkind kind;//identifier�����ͣ�int a ��int | const int a ��int | char a()��char | arg������
    union
    {
        char ch_val;
        int int_val;
    }val;//���ڳ�����˵�����泣����ֵ
    int paranum ;//����function��˵��������ĸ������Ա����ͼ��
    int array_size;//����������˵����������Ĵ�С����ֹԽ��
    int used;//������ʾ��������Ƿ�ʹ����
    struct entry* next;//���ڹ�ϣ����˵��ָ����һ���ڵ�
}entry;

typedef struct symboltable
{
    entry* bucket[MaxBukSize];//hash table
    char* name;//����ȫ�ֵķ��ű�Ϊglobal �������ľֲ����ű�Ϊfunction�����֣�
    int totaladdr;//�ܵĵ�ַ

    struct symboltable* funchild;//����global��˵ָ���һ��function
    struct symboltable* mainchild;//����global��˵ָ��main
    struct symboltable* sibling;//����funtion��˵ָ��ڶ���function
    struct symboltable* parent;//ָ�򸸽ڵ�
}symboltable;

int hash(char*);
entry* find_table(char* in_name);
void build_table(ASTNODE* tree);
void printtable();

extern symboltable* top;
extern symboltable* cur;

#endif // _SYMBAL_H
