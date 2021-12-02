/**
 * ��������: IsUnitClause
 * ���ܲ���: cnf *CNFList,int *ans
 * ��������:  �ж��Ƿ�Ϊ���Ӿ䣬�Ƿ���1�����Ƿ���0
 * ����ֵ: int
 */
int IsUnitClause(cNode *cla) {
    if (cla->first_l != NULL && cla->first_l->next != NULL)
        return 0;
    else
        return 1;
}

/**
 * ��������: RemoveClause
 * ���ܲ���: cNode* cla1, cnf *CNFList
 * ��������:  ɾ���Ӿ䣬�ɹ�����1
 * ����ֵ: int
 */
int RemoveClause(cNode* cla1, cnf *CNFList) {
    cNode *cla = CNFList->first_c;
    if (cla1 == CNFList->first_c)//���Ӿ�Ϊͷ�Ӿ�
        CNFList->first_c = CNFList->first_c->next;
    else {//���Ӿ䲻��ͷ�Ӿ�
        while (cla != NULL && cla->next != cla1)
            cla = cla->next;
        cla->next = cla->next->next;
    }
    free(cla1);
    cla1 = NULL;
    return 1;
}

/**
 * ��������: RemoveLiteral
 * ���ܲ���: lNode *lit, cNode *cla1
 * ��������:  ��ָ�����Ӿ���ɾ��ָ�������֣�ɾ���ɹ�����1
 * ����ֵ: int
 */
int RemoveLiteral(lNode *lit, cNode *cla1) {
    lNode *lit1 = cla1->first_l;
    if(lit1 == lit)//��Ϊͷ����
        cla1->first_l = cla1->first_l->next;
    else {//������ͷ����
        while (lit1 != NULL && lit1->next != lit)
            lit1 = lit1->next;
        lit1->next = lit1->next->next;
    }
    free(lit);
    lit = NULL;
    return 1;
}

/**
 * ��������: IsEmptyClause
 * ���ܲ���: cnf *CNFList
 * ��������: �ж��Ƿ��п��Ӿ䣬�Ƿ���1�����Ƿ���0
 * ����ֵ: int
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
 * ��������: Destroy
 * ���ܲ���: cnf *CNFList
 * ��������: ��������,�ɹ�����1
 * ����ֵ: int
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
 * ��������: Copycnf
 * ���ܲ���: cnf *CNFList,cnf *cnf1
 * ��������:  ����cnf�ṹ��ĸ���
 * ����ֵ: void
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
 * ��������: Addclause
 * ���ܲ���: cnf *CNFList,cNode *c
 * ��������:  �����Ӿ�
 * ����ֵ: void
 */
void Addclause(cnf *CNFList,cNode *c) {
    if (CNFList) {
        cNode *temp = CNFList->first_c;
        CNFList->first_c = c;
        CNFList->first_c->next = temp;
    }
}

/**
 * ��������: Dpll
 * ���ܲ���: cnf *CNFList,int *ans
 * ��������:  ���SAT���⣬������������ʱ��һ��ʽ��,���н��򷵻�1���޽ⷵ��0
 * ����ֵ: int
 */

int Dpll(cnf *CNFList, int *ans) {
    cNode *cla = CNFList->first_c, *cla1 = CNFList->first_c, *cla2 = NULL;
    int temp;
    lNode *lit = NULL, *lit1 = NULL;
    while (cla != NULL ) {//�ҳ����Ӿ�
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
                        RemoveClause(cla1, CNFList);//ɾ���Ӿ�
                        cla1 = NULL;
                        break;
                    }
                    if(lit->l == -temp) {
                        RemoveLiteral(lit, cla1);//ɾ������
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
 * ��������: WriteFile
 * ���ܲ���: int result, double time_, int* ans
 * ��������:  ����д���ļ��У��ɹ�����1��ʧ�ܷ���0
 * ����ֵ: int
 */
int WriteFile(int result, int time_, int* ans) {
    FILE* fp;
	for (int i = 0; fileName[i] != '\0'; i++)
	{
		//�޸���չ��
		if (fileName[i] == '.' && fileName[i + 4] == '\0')
		{
			fileName[i + 1] = 'r';
			fileName[i + 2] = 'e';
			fileName[i + 3] = 's';
			break;
		}
	}
	if (fopen_s(&fp, fileName, "w")) {
		printf("�ļ���ʧ��!\n");
		return 0;
	}
	fprintf(fp, "s %d\nv ", result);  //�����
	if (result == 1)
	{
		//�����ֵ
		for (int i = 1; i <= CNFList->var_num; i++)
		{
			if (ans[i] == 1) fprintf(fp, "%d ", i);
			else fprintf(fp, "%d ", -i);
		}
	}
	fprintf(fp, "\nt %d", time_ * 1000);  //����ʱ��/����
	fclose(fp);
	return 1;
}
