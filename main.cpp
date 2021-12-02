#include <cstddef>//malloc等动态分配函数
#include <cstdlib>//系统清屏函数
#include <cstdio>//标准输入输出函数
#include <cmath>
#include <climits>
#include <cstring> //用于字符串文件读写操作
#include <ctime>
#include <algorithm>


typedef struct lNode {//文字
    int l;//具体的变元内容
    struct lNode *next;//链表的下一个结点
} lNode, *plNode;

typedef struct cNode {//子句
    lNode *first_l; //第一个文字的结点
    struct cNode *next; //下一个句子结点
} cNode, *pcNode;
 
typedef struct cnf {
    int var_num;//变量总数
    int cla_num;//子句总数
    cNode *first_c;//第一个句子结点
} cnf;

cnf *CNFList = NULL;
clock_t start, finish; //设置时间变量
double time_;
char fileName[200];
int op = 1, result, dimension;
int *ans;  //保存结果


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
 * 函数名称: main
 * 接受参数: void
 * 函数功能: 主函数
 * 返回值: int
 */
int main(void)
{
	while (op)
	{
		system("cls");//清屏
        system("color 0A");//背景颜色	
        printf("\n\n");
		printf("                   功能菜单                       \n");
		printf("-------------------------------------------------\n");
		printf("1.读取cnf文件           2.cnf公式的验证（遍历输出每个子句）\n");
		printf("3.dpll求解算例并保存    4.数独游戏\n");
		printf("0.退出\n");
		printf("-------------------------------------------------\n");
		printf("     请选择你的操作:0~4");
		scanf("%d", &op);
		switch (op)
		{
		case 1:{
			CNFList = (cnf*)malloc(sizeof(cnf));
			printf("输入要读入的cnf文件");
			scanf("%s",fileName);
			_ReadFile(fileName, CNFList);
			getchar(); getchar();
			break;
		}
		case 2:
			if (CNFList == NULL) printf("未导入文件\n");
			else
			{
				printf("cnf子句如下\n");
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
			if (CNFList == NULL) printf("未导入文件\n");
			else
			{
				ans = (int*)malloc(sizeof(int) * (CNFList->var_num + 1));
				for (int i = 1; i <= CNFList->var_num; i++) 
					ans[i] = 1;  //初始化，均赋为1
				start = clock();  //计时开始;
				result = Dpll(CNFList, ans);
				finish = clock();    //结束
				printf("求解结果：%d\n0表示无解,1表示有解\n", result);
				if (result == 1)
				{
					for (int i = 1; i <= CNFList->var_num; i++)
					{
						if (ans[i] == 1) printf("%d ", i);
						else printf("%d ", -i);
					}
					printf("\n");
				}
				time_ = (int)(finish - start) / CLOCKS_PER_SEC;//计算运行时间
				printf("运行时间=%lfms\n", time_ * 1000);//输出运行时间
				if (WriteFile(result, time_, ans) == 1)
					printf("结果已经保存到同名.res文件\n");
				else printf("结果保存失败\n");
			}
			getchar(); getchar();
			break;
		case 4: {
			int mat[10][10];//存数独
			int num = 81;//已经填入的数的数量
			int dif;//数独的难度
			printf("请输入数独的难度：1~3");
			scanf("%d",&dif);
			CreateSuduko(dif, mat, &num);
			printf("输任意整数以查看答案");
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

