#ifndef _MIPS_H_
#include<vector>

#define maxregnum 8
#define maxrefernum 100000


typedef struct Reg
{
    int id;
    bool busy;//��ʾ����Ĵ������Ƿ���ֵ Ϊtrue��ʾ�Ѿ���ռ�� false��ʾû�б�ռ��
    std::vector<char*> vari_info;//��ʾ������Ĵ�������ı�������a | �������� 'c' | 1
}Reg;

#endif // _MIPS_H_
