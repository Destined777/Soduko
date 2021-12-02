/**
 * 函数名称: IsUnitClause
 * 接受参数: cnf *CNFList,int *ans
 * 函数功能:  判断是否为单子句，是返回1，不是返回0
 * 返回值: int
 */
int IsUnitClause(cNode *cla) {
    if (cla->first_l != NULL && cla->first_l->next != NULL)
        return 0;
    else
        return 1;
}

/**
 * 函数名称: RemoveClause
 * 接受参数: cNode* cla1, cnf *CNFList
 * 函数功能:  删除子句，成功返回1
 * 返回值: int
 */
int RemoveClause(cNode* cla1, cnf *CNFList) {
    cNode *cla = CNFList->first_c;
    if (cla1 == CNFList->first_c)//该子句为头子句
        CNFList->first_c = CNFList->first_c->next;
    else {//该子句不是头子句
        while (cla != NULL && cla->next != cla1)
            cla = cla->next;
        cla->next = cla->next->next;
    }
    free(cla1);
    cla1 = NULL;
    return 1;
}

/**
 * 函数名称: RemoveLiteral
 * 接受参数: lNode *lit, cNode *cla1
 * 函数功能:  在指定的子句中删除指定的文字，删除成功返回1
 * 返回值: int
 */
int RemoveLiteral(lNode *lit, cNode *cla1) {
    lNode *lit1 = cla1->first_l;
    if(lit1 == lit)//若为头文字
        cla1->first_l = cla1->first_l->next;
    else {//若不是头文字
        while (lit1 != NULL && lit1->next != lit)
            lit1 = lit1->next;
        lit1->next = lit1->next->next;
    }
    free(lit);
    lit = NULL;
    return 1;
}

/**
 * 函数名称: IsEmptyClause
 * 接受参数: cnf *CNFList
 * 函数功能: 判断是否含有空子句，是返回1，不是返回0
 * 返回值: int
 */
int IsEmptyClause(cnf *CNFList) {
    cNode *cla = CNFList->first_c;
    while (cla != NULL) {
        if (cla->first_l == NULL)
            return 1;
        cla = cla->next;
    }
    return 0;
}

/**
 * 函数名称: Destroy
 * 接受参数: cnf *CNFList
 * 函数功能: 销毁链表,成功返回1
 * 返回值: int
 */
int Destroy(cnf *CNFList) {
    cNode *cla1, *cla2;
    lNode *lit1, *lit2;
    for (cla1 = CNFList->first_c;cla1;cla1 = cla2) {
        cla2 = cla1->next;
        for (lit1 = cla1->first_l;lit1;lit1 = lit2) {
            lit2 = lit1->next;
            free(lit1);
        }
        free(cla1);
    }
    CNFList->first_c = NULL;
    return 1;
}

/**
 * 函数名称: Copycnf
 * 接受参数: cnf *CNFList,cnf *cnf1
 * 函数功能:  保存cnf结构体的副本
 * 返回值: void
 */
void Copycnf(cnf* CNFList, cnf* cnf1) {
    cnf1->cla_num = CNFList->cla_num;
    cnf1->var_num = CNFList->var_num;
    cnf1->first_c = (cNode*)malloc(sizeof(cNode)); 
    cNode *cla1 = CNFList->first_c, *cla2 = cnf1->first_c;
    lNode *lit1, *lit2;
    while (cla1) {
        lit1 = cla1->first_l;
        cla2->first_l = (lNode*)malloc(sizeof(lNode));
        lit2 = cla2->first_l;
        while (lit1) {
            lit2->l = lit1->l;
            if (lit1->next == NULL)
                break;
            lit2->next = (lNode*)malloc(sizeof(lNode));
            lit1 = lit1->next;
            lit2 = lit2->next;
        }
        lit2->next = NULL;
        if (cla1->next == NULL)
            break;
        cla2->next = (cNode*)malloc(sizeof(cNode));
        cla1 = cla1->next;
        cla2 = cla2->next;
    }
    cla2->next = NULL;
}

/**
 * 函数名称: Addclause
 * 接受参数: cnf *CNFList,cNode *c
 * 函数功能:  增加子句
 * 返回值: void
 */
void Addclause(cnf *CNFList,cNode *c) {
    if (CNFList) {
        cNode *temp = CNFList->first_c;
        CNFList->first_c = c;
        CNFList->first_c->next = temp;
    }
}

/**
 * 函数名称: Dpll
 * 接受参数: cnf *CNFList,int *ans
 * 函数功能:  求解SAT问题，给出满足条件时的一个式子,若有解则返回1，无解返回0
 * 返回值: int
 */

int Dpll(cnf *CNFList, int *ans) {
    cNode *cla = CNFList->first_c, *cla1 = CNFList->first_c, *cla2 = NULL;
    int temp;
    lNode *lit = NULL, *lit1 = NULL;
    while (cla != NULL ) {//找出单子句
        if (IsUnitClause(cla) == 0)
            cla = cla->next;
        else {
            if (cla->first_l->l > 0)
                ans[cla->first_l->l] = 1;
            else 
                ans[-cla->first_l->l] = 0;
            temp = cla->first_l->l;
            for (cla1 = CNFList->first_c; cla1 != NULL; cla1 = cla2) {
                cla2 = cla1->next;
                for (lit = cla1->first_l; lit != NULL; lit = lit1) {
                    lit1 = lit->next;
                    if(lit->l == temp){
                        RemoveClause(cla1, CNFList);//删除子句
                        cla1 = NULL;
                        break;
                    }
                    if(lit->l == -temp) {
                        RemoveLiteral(lit, cla1);//删除文字
                        lit = NULL;
                        break;
                    }
                } 
            }
            cla = CNFList->first_c;
            if (CNFList->first_c == NULL) {
                return 1;
            }
            else if (IsEmptyClause(CNFList)) {
                Destroy(CNFList);
                return 0;
            }
        }
    }
    int max = 0, maxlit = 0;
    int *count = (int*)malloc(sizeof(int) * (CNFList->var_num * 2 + 1));
    for (int i = 0; i <= CNFList->var_num * 2; i++) 
        count[i] = 0;
    for (cla = CNFList->first_c; cla; cla = cla->next) {
        for (lit = cla->first_l; lit; lit = lit->next) {
            if (lit->l > 0)
                count[lit->l]++;
            else
                count[CNFList->var_num - lit->l]++;
        }
    }
    for (int i = 1; i <= CNFList->var_num; i++) {
        if (max < count[i]) {
            max = count[i];
            maxlit = i;
        }
    }
    if (!max) {
        for (int i = CNFList->var_num + 1; i <= CNFList->var_num * 2; i++) {
            if (max < count[i]) {
                max = count[i];
                maxlit = i;
            }
        }
    }
    free(count);
    cnf *cnf1 = (cnf*)malloc(sizeof(cnf));
    Copycnf(CNFList,cnf1);
    cNode *c = (cNode*)malloc(sizeof(cNode));
    c->first_l = (lNode*)malloc(sizeof(lNode));
    c->first_l->l = maxlit;
    c->first_l->next = NULL;
    Addclause(cnf1, c);
    if (Dpll(cnf1,ans)){
        return 1;
    }    
    Destroy(cnf1);
    c = (cNode*)malloc(sizeof(cNode));
    c->first_l = (lNode*)malloc(sizeof(lNode));
    c->first_l->l = -maxlit;
    c->first_l->next = NULL;
    Copycnf(CNFList,cnf1);
    Addclause(cnf1, c);
    int state = Dpll(cnf1,ans);
    return state;
}

/**
 * 函数名称: WriteFile
 * 接受参数: int result, double time_, int* ans
 * 函数功能:  将答案写入文件中，成功返回1，失败返回0
 * 返回值: int
 */
int WriteFile(int result, int time_, int* ans) {
    FILE* fp;
	for (int i = 0; fileName[i] != '\0'; i++)
	{
		//修改拓展名
		if (fileName[i] == '.' && fileName[i + 4] == '\0')
		{
			fileName[i + 1] = 'r';
			fileName[i + 2] = 'e';
			fileName[i + 3] = 's';
			break;
		}
	}
	if (fopen_s(&fp, fileName, "w")) {
		printf("文件打开失败!\n");
		return 0;
	}
	fprintf(fp, "s %d\nv ", result);  //求解结果
	if (result == 1)
	{
		//保存解值
		for (int i = 1; i <= CNFList->var_num; i++)
		{
			if (ans[i] == 1) fprintf(fp, "%d ", i);
			else fprintf(fp, "%d ", -i);
		}
	}
	fprintf(fp, "\nt %d", time_ * 1000);  //运行时间/毫秒
	fclose(fp);
	return 1;
}
