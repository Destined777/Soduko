

/*
 * 函数名称: CreateSuduko 
 * 接受参数: int dif, int** mat, int *num
 * 函数功能: 生成数独
 * 返回值: void
 */
void CreateSuduko(int dif, int mat[][10], int *num) {
    int temp[11];
    for(int i = 1; i <= 9; i++) temp[i] = i;
    srand(rand());
    std::random_shuffle(temp + 1, temp + 10);
    int k = 1;
    for(int i = 4; i <= 6; i++) {
        for(int j = 4; j <= 6; j++, k++) {
            mat[i][j] = temp[k];
        }
    }
    for(int i = 4; i <= 6; i++) {
        mat[i - 3][6] = mat[i + 3][5] = mat[i][4];
        mat[i - 3][4] = mat[i + 3][6] = mat[i][5];
        mat[i - 3][5] = mat[i + 3][4] = mat[i][6];
    }
    for(int i = 4; i <= 6; i++) {
        mat[6][i - 3] = mat[5][i + 3] = mat[4][i];
        mat[4][i - 3] = mat[6][i + 3] = mat[5][i];
        mat[5][i - 3] = mat[4][i + 3] = mat[6][i];
    }
    for(int i = 4; i <= 6; i++) {
        mat[i - 3][3] = mat[i + 3][2] = mat[i][1];
        mat[i - 3][1] = mat[i + 3][3] = mat[i][2];
        mat[i - 3][2] = mat[i + 3][1] = mat[i][3];
    }
    for(int i = 4; i <= 6; i++) {
        mat[i - 3][9] = mat[i + 3][8] = mat[i][7];
        mat[i - 3][7] = mat[i + 3][9] = mat[i][8];
        mat[i - 3][8] = mat[i + 3][7] = mat[i][9];
    }
    dif *= 20;
    int i , j;
    while(dif--) {
        i = rand() % 9 + 1; j = rand() % 9 + 1;
        if(mat[i][j]) num--;
        mat[i][j] = 0;
    }
    for (int i = 1; i <= 9; i++) {//打印棋盘
        for (int j = 1; j <= 9; j++) {
            if (mat[i][j])
                printf("%d ", mat[i][j]);
            else 
                printf("_ ");
        }
        putchar('\n');
    }
}

/*
 * 函数名称: Getans 
 * 接受参数: int** mat, int num
 * 函数功能: 计算数独答案
 * 返回值: void
 */
void Getans(int mat[][10], int num) {
    FILE* fp = fopen("sudoku.cnf", "w");
    fprintf(fp, "p cnf 1001 %d\n", 11988 - 36 * num);
    int x[10] = {0, 0, 0, 0, 1, 1, 1, 2, 2, 2}, y[10] = {0, 0, 1, 2, 0, 1, 2, 0, 1, 2};
    for(int i = 1; i <= 9; i++) {
        for(int j = 1; j <= 9; j++) {
            if(mat[i][j]) {fprintf(fp, "%d%d%d 0\n", i, j, mat[i][j]); continue;}
            for(int k = 1; k <= 9; k++) fprintf(fp, "%d%d%d ", i, j, k);
            fprintf(fp,"0\n");
            for(int i1 = 1; i1 <= 8; i1++) {
                for(int j1 = i1 + 1; j1 <= 9; j1++) {
                    fprintf(fp, "-%d%d%d -%d%d%d 0\n", i, j, i1, i, j, j1);
                }
            }
        }
    }
    for(int i = 1; i <= 9; i++) {
        for(int j = 1; j <= 9; j++) {
            for(int k = 1; k <= 9; k++) fprintf(fp, "%d%d%d ", i, k, j);
            fprintf(fp,"0\n");
            for(int i1 = 1; i1 <= 8; i1++) {
                for(int j1 = i1 + 1; j1 <= 9; j1++) {
                    fprintf(fp, "-%d%d%d -%d%d%d 0\n", i, i1, j, i, j1, j);
                }
            }
        }
    }
    for(int i = 1; i <= 9; i++) {
        for(int j = 1; j <= 9; j++) {
            for(int k = 1; k <= 9; k++) fprintf(fp, "%d%d%d ", k, i, j);
            fprintf(fp,"0\n");
            for(int i1 = 1; i1 <= 8; i1++) {
                for(int j1 = i1 + 1; j1 <= 9; j1++) {
                    fprintf(fp, "-%d%d%d -%d%d%d 0\n", i1, i, j, j1, i, j);
                }
            }
        }
    }
    for(int i = 1; i <= 9; i++) {
        for(int j = 1; j <= 9; j++) {
            for(int k = 1; k <= 9; k++) fprintf(fp, "%d%d%d ", 1 + 3 * x[i] + x[k], 1 + 3 * y[i] + y[k], j);
            fprintf(fp,"0\n");
            for(int i1 = 1; i1 <= 8; i1++) {
                for(int j1 = i1 + 1; j1 <= 9; j1++) {
                    fprintf(fp, "-%d%d%d -%d%d%d 0\n", 1 + 3 * x[i] + x[i1], 1 + 3 * y[i] + y[i1], j, 1 + 3 * x[i] + x[j1], 1 + 3 * y[i] + y[j1], j);
                }
            }
        }
    }
    fclose(fp);
    _ReadFile("sudoku.cnf", CNFList);
    ans = (int*)malloc(sizeof(int) * (CNFList->var_num + 1));
    for (int i = 1; i <= CNFList->var_num; i++) 
		ans[i] = 1;  //初始化，均赋为1
    Dpll(CNFList, ans);
    for (int i = 111; i < 1000; i++) {
        if (ans[i]) {
            mat[i/100][(i/10)%10] = i % 10;
        }
    }
    for(int i = 1; i <= 9; i++) {
       for(int j = 1; j <= 9; j++) {
            printf("%d ", mat[i][j]);
        }
        putchar('\n');
    }
}