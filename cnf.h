

/**
 * 函数名称:_ReadFile 
 * 接受参数: char* name, cnf *CNFList
 * 函数功能: 读取文件
 * 返回值: int
 */
int _ReadFile(const char* name, cnf *CNFList) {
    char ch;
    int temp = 0;
    FILE* fp = fopen(name,"r");
    if(!fp) {
        printf("文件打开失败\n");
        return 0;
    }
    else {
        while ((ch = getc(fp)) == 'c') {
		    while ((ch = getc(fp)) != '\n')
			    continue;           //弃去一整行
	    }   //运行到此，已经读取了字符p
	    getc(fp); getc(fp); getc(fp); getc(fp); //弃去cnf三个字母
        fscanf(fp, "%d", &CNFList->var_num);    //p后的第1个数值是布尔变元数量
	    fscanf(fp, "%d", &CNFList->cla_num);    //p后的第2个数值是子句数量
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
        printf("读取完毕\n");
	    fclose(fp);
	    return 1;
    }
}