#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TEST_COUNT 100
#define MAXSIZE 100
#define MAXR    10

/* 三元组结构体 */
struct Triple { //定义稀疏矩阵非零元素的三元组
    int i,j;                    //记录稀疏矩阵非零元素所在的行号i，列号j
    int e;                      //记录稀疏矩阵非零元素值
};
/* 稀疏矩阵的数据结构 */
struct Matrix{
    struct Triple data[MAXSIZE];  //定义一个数组，每个元素是矩阵元素的一个三元组，数组的所有元素描述了矩阵的所有的非零元素a
    int rpos[MAXR];             //每行第一个非0元素在data数组中的位置
    int m,n,num;                   //n、m、num 分别表示矩阵的行数、列数、非零元素的个数
};

/* 自定义函数的调用层次：
   main-> OpSelect
       -> InputMatrix -> MatrixDisp
       -> MatrixTrans -> MatrixDisp
       -> MatrixMulti -> GenResult
                      -> MatrixDisp
 */
int OpSelect();                         // 
struct Matrix InputMatrix();                                          // 输入一个矩阵并保存，在函数内部按提示输入每行数据，按三元组压缩保存。
void MatrixTrans(struct Matrix *pMatrix);                             // 对指定矩阵做转置操作。
void MatrixMulti(struct Matrix M1, struct Matrix M2);                 // 对指定矩阵做乘法操作。
void MatrixDisp(struct Matrix *pMatrix);                              // 打印显示指定矩阵。
void GenResult( struct Matrix *pMatrix,int i,int j,int e);            // 在遍历两个相乘的三元组表示的矩阵的同时，存储乘积结果。生成新的由三元组表示的矩阵。

int main(){
    struct Matrix M, *ParseMatrix;
    struct Matrix A, B, C;
    int sel;

    for(int i=1; i<TEST_COUNT; i++)
    {
        sel = OpSelect();
        switch(sel)
        {
            case 1:{
                M = InputMatrix();
                ParseMatrix = &M;
                MatrixTrans(ParseMatrix);
                break;
            }
            case 2:{
                printf("输入矩阵A -->");
                A = InputMatrix();
                printf("输入矩阵B -->");
                B = InputMatrix();
                MatrixMulti(A, B);
               break;
            }
            default:{
                printf("选择您操作类型（1~3）! \n");
                break;
            }
        }
    }
    return 0;
}

int OpSelect()
{
    int sel;
    printf("\n");
    printf("请输入操作类型：\n");
    printf("1.矩阵转置操作 \n");
    printf("2.矩阵乘法操作 \n");
    scanf("%d",&sel);

    return sel;
}

struct Matrix InputMatrix()
{
    struct Triple tri;
    struct Matrix M, *pMatrix;
    int m,n;
    int element;
    int row[10];

    printf("请输入矩阵的行数，列数（用空格分隔）：\n");
    scanf("%d%d",&m,&n);



    tri.i = -1;
    tri.j = 0;
    tri.e = 0;

    M.m = m;
    M.n = n;
    M.num =0;
    
    for(int i=0;i<m; i++){
        printf("请输入矩阵第%d行数据后回车（连续输入本行所有数据，取值范围0~9）：\n",i);
         for (int j = 0; j < n; j++){
            scanf ("%1d", &row[j]); // 为输入处理方便，限制矩阵元素取值范围是0~9
         }
        
//        scanf("%d%d%d%d%d%d%d%d%d%d", &row[0], &row[1], &row[2], &row[3], &row[4], &row[5], &row[6], &row[7], &row[8], &row[9]);
        for(int j=0;j<n; j++){
            element = row[j];
            if(element !=0){
                if(i !=tri.i){   // 只在此行号下出现的第1个非零元素才记录其位置
                    M.rpos[i] =j;
                }
                tri.i = i;       // 扫描到1个矩阵非零元素，生成1个三元组
                tri.j = j;  
                tri.e = element;
                
                M.num =M.num +1;                   // 新增1个非零元素，数组长度加1 
                M.data[M.num-1] = tri;             // 把这个非零的元素对应的三元组保存到数组
            }
        }
    }
   pMatrix = &M;
   printf("输入的矩阵是：\n");
   MatrixDisp(pMatrix);

    //return pMatrix;
    return M;
}

void MatrixDisp(struct Matrix *pMatrix)
{
    int k;
    for(int i=0; i<pMatrix->m; i++){
        for(int j=0; j<pMatrix->n; j++){
            // 对当前i,j位置，在数组中寻找是否有匹配，若有则打印记录值
            for(k=0; k<pMatrix->num;k++){
                if(pMatrix->data[k].i== i && pMatrix->data[k].j ==j){
                    printf("%d ",pMatrix->data[k].e);
                    break;
                }
            }
            // 若无匹配值，则打印0
            if(k == pMatrix->num){
                printf("0 ");
            }

        }
        printf("\n");
    }
}

void MatrixTrans(struct Matrix *pMatrix)
{
    int k, tmp_i, tmp_j, tmp_m, tmp_n;
    for(k=0; k<pMatrix->num;k++){
            tmp_i = pMatrix->data[k].i;
            tmp_j = pMatrix->data[k].j;
            pMatrix->data[k].i = tmp_j;
            pMatrix->data[k].j = tmp_i;
        }

    tmp_m = pMatrix->m;
    tmp_n = pMatrix->n;
    pMatrix->m = tmp_n;
    pMatrix->n = tmp_m;

    printf("转置后的矩阵是：\n");
    MatrixDisp(pMatrix);
}

//struct Matrix MatrixMulti(struct Matrix M1, struct Matrix M2)
void MatrixMulti(struct Matrix M1, struct Matrix M2)
{
    struct Matrix M3, *pMatrix;
    pMatrix = &M3;

    int k =0;
    pMatrix->m = M1.m;
    pMatrix->n = M2.n;
    pMatrix->num = 0;
    pMatrix->data[pMatrix->num].e = 0;

    //pMatrix = &M3;
    int temp_e;
    //int element[M1.m*M2.n] = 0;
    for(int k1=0; k1<M1.num;k1++){             // 遍历M1的三元数组
        for(int k2=0; k2<M2.num; k2++){        // 固定M1时，对M2的三元数组遍历
            if(M1.data[k1].j == M2.data[k2].i) // 若M1和行号等于M2的列号，则满足相乘条件
            {

                temp_e = M1.data[k1].e * M2.data[k2].e; // 但这只是需要计算求和的多个子项之一
                //printf("M1.data[k1].j:%d, M2.data[k2].i:%d, M1.data[k1].e:%d, M2.data[k2].e:%d \n" ,M1.data[k1].j, M2.data[k2].i, M1.data[k1].e, M2.data[k2].e);
                //printf("k1:%d,M1.num:%d, k2:%d, M2.num:%d,temp_e:%d \n" ,k1,M1.num,k2,M2.num, temp_e);
                GenResult(pMatrix, M1.data[k1].i, M2.data[k2].j, temp_e); // 把这个结果放入M3的e中，但后面若还有满足条件的，则继续放入
            }
        }
    }
   printf("矩阵相乘的结果是：\n");
   MatrixDisp(pMatrix);
}

void GenResult( struct Matrix *pMatrix,int i,int j,int e){//仅在乘法运算中使用，中间运算添加三元组
    int flag = 0;

    // 新矩阵中搜索，是否已有M1[i]=M2[j]的乘积结果
    int count;
	for(count = 0; count < pMatrix->num; count++){              //在目标矩阵中遍历是否已有相同行列值的结果项，有则在现有三元组上累加e值
		if(pMatrix->data[count].i == i && pMatrix->data[count].j == j){
			pMatrix->data[count].e += e; 
            flag =1; // 找到了
            //printf("flag:%d, i:%d, j:%d, tmpe_e:%d, count:%d, M3_e:%d \n", flag,i,j,e,count,pMatrix->data[count].e);
		}
	}
	// 循环结束，新矩阵中没有找到M1[i]=M2[j]的乘积结果的三元组
    if(flag == 0){
	    pMatrix->num++;//非零元个数+1,即增加新的三元组到目标矩阵
	    pMatrix->data[pMatrix->num-1].i = i;
	    pMatrix->data[pMatrix->num-1].j = j;
	    pMatrix->data[pMatrix->num-1].e = e;
        //printf("flag:%d, i:%d, j:%d, tmpe_e:%d, count:%d, M3_e:%d, num-1:%d \n", flag,i,j,e, count,e,pMatrix->num-1);
    }
}