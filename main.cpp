#include <cstddef>//malloc�ȶ�̬���亯��
#include <cstdlib>//ϵͳ��������
#include <cstdio>//��׼�����������
#include <cmath>
#include <climits>
#include <cstring> //�����ַ����ļ���д����
#include <ctime>
#include <algorithm>


typedef struct lNode {//����
    int l;//����ı�Ԫ����
    struct lNode *next;//�������һ�����
} lNode, *plNode;

typedef struct cNode {//�Ӿ�
    lNode *first_l; //��һ�����ֵĽ��
    struct cNode *next; //��һ�����ӽ��
} cNode, *pcNode;
 
typedef struct cnf {
    int var_num;//��������
    int cla_num;//�Ӿ�����
    cNode *first_c;//��һ�����ӽ��
} cnf;

cnf *CNFList = NULL;
clock_t start, finish; //����ʱ�����
double time_;
char fileName[200];
int op = 1, result, dimension;
int *ans;  //������


int _ReadFile(const char* name, cnf *CNFList);
int IsUnitClause(cNode *cla);
int RemoveClause(cNode* cla1, cnf *CNFList);
int RemoveLiteral(lNode *lit, cNode *cla1);
int IsEmptyClause(cnf *CNFList);
int Destroy(cnf *CNFList);
int Dpll(cnf *CNFList,int *ans);
int WriteFile(int result, int time, int* ans);
void CreateSuduko(int dif, int** mat, int *num);
void Getans(int** mat, int num);

#include "cnf.h"
#include "dpll.h"
#include "Sudoku.h"

/**
 * ��������: main
 * ���ܲ���: void
 * ��������: ������
 * ����ֵ: int
 */
int main(void)
{
	while (op)
	{
		system("cls");//����
        system("color 0A");//������ɫ	
        printf("\n\n");
		printf("                   ���ܲ˵�                       \n");
		printf("-------------------------------------------------\n");
		printf("1.��ȡcnf�ļ�           2.cnf��ʽ����֤���������ÿ���Ӿ䣩\n");
		printf("3.dpll�������������    4.������Ϸ\n");
		printf("0.�˳�\n");
		printf("-------------------------------------------------\n");
		printf("     ��ѡ����Ĳ���:0~4");
		scanf("%d", &op);
		switch (op)
		{
		case 1:{
			CNFList = (cnf*)malloc(sizeof(cnf));
			printf("����Ҫ�����cnf�ļ�");
			scanf("%s",fileName);
			_ReadFile(fileName, CNFList);
			getchar(); getchar();
			break;
		}
		case 2:
			if (CNFList == NULL) printf("δ�����ļ�\n");
			else
			{
				printf("cnf�Ӿ�����\n");
				for (cNode *cp = CNFList->first_c; cp != NULL; cp = cp->next)
				{
					for (lNode *lp = cp->first_l; lp != NULL; lp = lp->next)
					{
						printf("%d ", lp->l);
					}
					printf("\n");
				}
			}
			getchar(); getchar();
			break;
		case 3:
			if (CNFList == NULL) printf("δ�����ļ�\n");
			else
			{
				ans = (int*)malloc(sizeof(int) * (CNFList->var_num + 1));
				for (int i = 1; i <= CNFList->var_num; i++) 
					ans[i] = 1;  //��ʼ��������Ϊ1
				start = clock();  //��ʱ��ʼ;
				result = Dpll(CNFList, ans);
				finish = clock();    //����
				printf("�������%d\n0��ʾ�޽�,1��ʾ�н�\n", result);
				if (result == 1)
				{
					for (int i = 1; i <= CNFList->var_num; i++)
					{
						if (ans[i] == 1) printf("%d ", i);
						else printf("%d ", -i);
					}
					printf("\n");
				}
				time_ = (int)(finish - start) / CLOCKS_PER_SEC;//��������ʱ��
				printf("����ʱ��=%lfms\n", time_ * 1000);//�������ʱ��
				if (WriteFile(result, time_, ans) == 1)
					printf("����Ѿ����浽ͬ��.res�ļ�\n");
				else printf("�������ʧ��\n");
			}
			getchar(); getchar();
			break;
		case 4: {
			int mat[10][10];//������
			int num = 81;//�Ѿ��������������
			int dif;//�������Ѷ�
			printf("�������������Ѷȣ�1~3");
			scanf("%d",&dif);
			CreateSuduko(dif, mat, &num);
			printf("�����������Բ鿴��");
			int t;
			scanf("%d",&t);
			CNFList = (cnf*)malloc(sizeof(cnf)); 
			Getans(mat, num);
			getchar();getchar();
			break;
		}
		case 0:
			return 0;
		}
	}
	return 0;
}

