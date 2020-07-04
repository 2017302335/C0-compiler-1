#include "symbol.h"
#include "global.h"
#include "lex.h"
#include "grammer.h"
#include<string.h>
#include<stdlib.h>
#include<stdio.h>

FILE * fp_source;
FILE * fp_log;

void typecheck(ASTNODE * tree)
{
    int i;
    symboltable* temp;//�������÷��ű��λ��
    entry* p;//�ұ���
    ASTNODE* t;//��������������ĸ���
    int acc = 0;//��¼�����ĸ���

    if(tree != NULL && tree->type != CONSTDECLARE && tree->type != VARIDECLARE)
    {

        if(tree->type == OPTYPE)
        {
            //��ʱ����Ϊ+ - * / | < <= > >= == !=
            if(tree->useful == FALSE)
            {
                //��ʾ��exp���ɵ����ýڵ�
                typecheck(tree->childen[1]);

                tree->kind = tree->childen[1]->kind;
                return;
            }else
            {
                //��ʱ��ʾ�������õĽڵ㣬����Ҫ������������Ż��ǱȽϷ��ţ���Ϊ���Ƕ�Ԫ����
                    typecheck(tree->childen[0]);
                    typecheck(tree->childen[1]);

                    if(tree->childen[0]->kind == tree->childen[1]->kind)
                    {
                        //��ʾ�ǿ��Ե�
                        tree->kind = tree->childen[0]->kind;
                        return;
                    }else
                    {
                        //��ʾƥ�䷢������
                        printf("Sementic Error at line %d : type conflict\n",tree->lineno);
                        exit(1);
                    }

            }

        }else if(tree->type == CALL)
        {//�����������,��Ҫ�������ĸ����Ƿ������ʱ��ƥ��
            p = find_table(tree->childen[0]->attr.name);

            if(p)
            {
                //�ҵ����������
                tree->kind = p->kind;
                typecheck(tree->childen[1]);
                if(p->type != TFunc)
                {
                    printf("Semantic Error at line %d : invalid refering\n",tree->lineno);
                    exit(1);
                }

                if((p->paranum == 0 && tree->childen[1] == NULL) || (p->paranum == tree->childen[1]->attr.val))//��������ƥ��
                {
                    p->used = TRUE;//���Ϊ�������ʹ����
                    return;
                }else
                {
                    printf("Semantic Error at line %d : function call matched wrong arg numbers\n",tree->lineno);
                    exit(1);
                }
            }else
            {
                printf("Semantic Error at line %d : function used but not declared\n",tree->lineno);
                exit(1);
            }

        }else if(tree->type == VALUE_ARG)
        {
            //��ʱֻ�����������Ƿ�ƥ��
            t = tree;
            acc = 0;
            while(t)
            {
                acc++;
                t = t->sibling;
            }
            tree->attr.val = acc;
            return;

        }else if(tree->type == FUNCDECLARE)//Ϊ�˲���function��id������
        {
            //ͬʱ���ű�ҲҪ���óɶ�Ӧ�ķ��ű�
            temp = top->funchild;
            while(temp)
            {
                if(strcmp(temp->name,tree->childen[0]->attr.name) == 0)
                {
                    cur = temp;
                    break;
                }
                temp = temp->sibling;
            }
            typecheck(tree->childen[2]);
            return;
        }else if(tree->type == MAINTYPE)
        {
            cur = top->mainchild;
            typecheck(tree->childen[0]);
            return;
        }else if(tree->type == IDTYPE)
        {
            p = find_table(tree->attr.name);

            if(p)
            {
                tree->kind = p->kind;

                if(p->type != TVariable && p->type != TConst && p->type != TArg)
                {
                    printf("Semantic Error at line %d : invalid refering \n",tree->lineno);
                    exit(1);
                }
                p->used = TRUE;
                return;
            }
            else
            {
                printf("Semantic Error at line %d : variable used but not declared\n",tree->lineno);
                exit(1);
            }
        }else if(tree->type == CONSTCHAR)
        {
            tree->kind = Char;
            return;
        }else if(tree->type == CONSTNUM)
        {
            tree->kind = Integer;
            return;
        }else if(tree->type == ASSIGN_STMT)
        {
            typecheck(tree->childen[0]);
            typecheck(tree->childen[1]);

            if(tree->childen[0]->kind == tree->childen[1]->kind)
            {
                //��ʾ�ǿ��Ե�
                tree->kind = tree->childen[0]->kind;
                return;
            }else
            {
                //��ʾƥ�䷢������
                printf("Sementic Error at line %d : type conflict\n",tree->lineno);
                exit(1);
            }
        }else if(tree->type == ARRAYTYPE)
        {
            p = find_table(tree->childen[0]->attr.name);

            if(p)
            {
                tree->kind = p->kind;
                typecheck(tree->childen[1]);

                if(p->type != TArray)
                {
                    printf("Semantic Error at line %d : invalid refering \n",tree->lineno);
                    exit(1);
                }

                if(tree->childen[1]->kind == Integer)
                {
                    if(tree->childen[1]->type == OPTYPE && tree->childen[1]->childen[1]->type == CONSTNUM)
                    {
                        if(tree->childen[1]->childen[1]->attr.val >= p->array_size)
                        {
                            printf("Semantic Error at line %d : array index out of bound\n",tree->lineno);
                            exit(1);
                        }
                    }

                }else
                {
                    printf("Semantic Error at line %d : index of array is not a integer\n",tree->lineno);
                    exit(1);
                }
                p->used = TRUE;
                return;
            }else
            {
                printf("Semantic Error at line %d : array used but not declared\n",tree->lineno);
                exit(1);
            }
        }


        for(i=0;i<MAXCHILDREN;i++)
        {
            typecheck(tree->childen[i]);
        }
        typecheck(tree->sibling);
    }
}

int main()
{
    fp_source = fopen("./���Գ���/2.txt","r");//4 | 7 | 9 �����⣬����û�п�������ת�� ��8 | 11 | test8 | test13 | testerror�д������

    ASTNODE* result = program();

    build_table(result);

    typecheck(result);

    printtable();

    printf("\nSemantic Analyze Complete...\n");

    return 0;

}
