

/**
 * ��������:_ReadFile 
 * ���ܲ���: char* name, cnf *CNFList
 * ��������: ��ȡ�ļ�
 * ����ֵ: int
 */
int _ReadFile(const char* name, cnf *CNFList) {
    char ch;
    int temp = 0;
    FILE* fp = fopen(name,"r");
    if(!fp) {
        printf("�ļ���ʧ��\n");
        return 0;
    }
    else {
        while ((ch = getc(fp)) == 'c') {
		    while ((ch = getc(fp)) != '\n')
			    continue;           //��ȥһ����
	    }   //���е��ˣ��Ѿ���ȡ���ַ�p
	    getc(fp); getc(fp); getc(fp); getc(fp); //��ȥcnf������ĸ
        fscanf(fp, "%d", &CNFList->var_num);    //p��ĵ�1����ֵ�ǲ�����Ԫ����
	    fscanf(fp, "%d", &CNFList->cla_num);    //p��ĵ�2����ֵ���Ӿ�����
        CNFList->first_c = (cNode*)malloc(sizeof(cNode));
        cNode *cla = CNFList->first_c;
        for(int i = 0;i < CNFList->cla_num;i++,cla = cla->next) {
            fscanf(fp, "%d",&temp);
            cla->first_l = (lNode*)malloc(sizeof(lNode));
            lNode *lit = cla->first_l;
            for(;temp;lit = lit->next) {
                lit->l = temp;
                lit->next = (lNode*)malloc(sizeof(lNode));
                fscanf(fp, "%d", &temp);
			    if (temp == 0) 
                    lit->next = NULL;
            }
            cla->next = (cNode*)malloc(sizeof(cNode));
            if (i == CNFList->cla_num - 1) {
			    cla->next = NULL;
			    break;
		    }
        }
        printf("��ȡ���\n");
	    fclose(fp);
	    return 1;
    }
}