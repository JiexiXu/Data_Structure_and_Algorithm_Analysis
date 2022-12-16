#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define DebugLog 0 
#define MaxInt 32767    //INT类型可表示的最大值
#define MaxVNum 100     //设置实验中顶点的最大个数 Max Vertex Number

//宏定义Bool类型的值
#define true 1
#define false 0
typedef int Boolean; 

Boolean visited[MaxVNum];//全局变量，数组。作为搜索过程中的“已访问”标志

typedef int PathMatrix[MaxVNum][MaxVNum]; // P矩阵，记录源点到终点的最短路径过的所有顶点
typedef int PathDistance[MaxVNum];       // D数组，记录源点到终点的最短路径长度
Boolean final[MaxVNum];    //即S数组，true表示已经求得从v0到该顶点v的最短距离

// 图的邻接矩阵表示法
struct Graph{
    char vexs[MaxVNum];        //一维数组，存储顶点值。假设顶点的数据类型为字符型
    int  arcs[MaxVNum][MaxVNum]; //二维数组，存储邻接矩阵
    int  vexnum, arcnum;     //图的顶点数和边数
};

struct StartVex{
	char value;
	int  index;
};

struct SQ{
	int *base;
	int front;
	int rear;
};

typedef struct{
    char adjvex;      //最小边在U集中的顶点 
    int lowcost;      //最小边上的权值
  }ClosEdge[MaxVNum]; //数组长度与连通网顶点的个数相同

// 创建无向网
struct Graph *CreateUDN();
int LocateVex(struct Graph *G, char v);

// 判断连通性
void BFS (struct Graph *G, int v);
struct StartVex SetStartVex(struct Graph *G);
void InitQueue(struct SQ *Q);
void EnQueue(struct SQ *Q, int e);
int DeQueue(struct SQ *Q);
Boolean QueueEmpty(struct SQ *Q);
Boolean ResultCheck(struct Graph *G);

// 求最小生成树
void MiniSpanTree_PRIM(struct Graph *G, char u);
int FirstAdjVex(struct Graph *G , int v);
int NextAdjVex(struct Graph *G , int u , int w);
int Min(ClosEdge SZ, struct Graph *G);

// 求源点到其它顶点的最短路径
void ShortPath_DIJ(struct Graph *G,int v0 , PathMatrix P , PathDistance D);
void ResultReport(struct Graph *G, int v0, PathMatrix P, PathDistance D);



void InitQueue(struct SQ *Q)
{
	//创建一个新队列
    int Queue[MaxVNum]; 
	Q->base = Queue;
	Q->front = Q->rear = 0;
}

void EnQueue(struct SQ *Q, int e)
{
	//插入元素e为Q的新的队尾元素
	if((Q->rear + 1) % MaxVNum == Q->front)
		return;
	Q->base[Q->rear] = e;
	Q->rear = (Q->rear + 1) % MaxVNum;
}

Boolean QueueEmpty(struct SQ *Q)
{
	//判断是否为空队
	if(Q->rear == Q->front)
		return true;
	return false;
}

int DeQueue(struct SQ *Q)
{
	//队头元素出队并置为u
    int u;
	u = Q->base[Q->front];
	Q->front = (Q->front + 1) % MaxVNum;
    return u;
}

//确定顶点在图G中的编号
int LocateVex(struct Graph *G, char v)
{
	int i;
	for (i = 0; i < G->vexnum; i++){
		if (G->vexs[i] == v)break;
	}
	return i;
}

struct Graph *CreateUDN()
{
	struct Graph *G =NULL;
	G = (struct Graph*)malloc(sizeof(struct Graph));
	//struct Graph graph;
	//struct Graph *G =&graph;

    int i, j, k, w;
	char va, vb;

    // 分别输入无向网的顶点数和边的个数
	printf("\n请输入无向网的顶点数:");
	scanf("%d", &G->vexnum);
	printf("\n请输入无向网的边数:");
	scanf("%d", &G->arcnum);

    // 输入所有顶点值，产生一个顶点向量
	printf("\n请输入顶点值的集合(连续输入%d个顶点，最后以Enter结束)：", G->vexnum);
	fflush(stdin);
	for (i = 0; i < G->vexnum; i++)
    {
        scanf("%c", &G->vexs[i]);
    }

    // 初始化邻接矩阵
	for (i = 0; i < G->vexnum; i++){
		for (j = 0; j < G->vexnum; j++){
			G->arcs[i][j] = MaxInt;
		}
    }

	// 填充邻接矩阵。按设置的边的数目循环，依次输入arcnum条边，构造邻接矩阵
	printf("\n请输入%d条边(顶点a顶点b权值，连续重复输入，回车结束输入) ... \n", G->arcnum);
	fflush(stdin);
	for (k = 0; k < G->arcnum; k++)
    {
        //printf("第%d条边:", k+1);
	    //fflush(stdin);                  // 清空buffer中的CR等字符，避免输入干扰
		//scanf("%c-%c %d", &va, &vb, &w);// 输入2个顶点的字符、对应的权值
		scanf("%c%c%d", &va, &vb, &w);
		//scanf("%d%c%c", &w, &va, &vb);
        
        // 确定两个顶点在vexs向量中的位置，作为这条边邻接矩阵中的横纵坐标
		i = LocateVex(G,va); 
        j = LocateVex(G,vb);
        // 对矩阵元素赋值，值为这条边对应的权值
		G->arcs[i][j] = w;
        G->arcs[j][i] = w; // 由于是无向网，所以矩阵是对称的
	}
    printf("完成无向网邻接矩阵输入！\n");

    // 打印输出邻接矩阵
	for(i=0;i<G->vexnum;i++){
		for(j=0;j<G->vexnum;j++){
			printf("%10d ",G->arcs[i][j]);
		}
		printf("\n");
	}

    return G;
}

int FirstAdjVex(struct Graph *G , int v)
{
	//返回v的第一个邻接点
	int i;
	for(i = 0 ; i < G->vexnum ; ++i){
        if(DebugLog==1) printf("\nFirstAdjVex: i=%d",i); //for debug
		if(G->arcs[v][i] <= MaxInt && visited[i] == false){
            if(DebugLog==1) printf("\nv=%d,有邻接点",v); //for debug
			return i;
        }
	}
    if(DebugLog==1) printf("\nv=%d,无邻接点",v); //for debug
	return -1;
}

int NextAdjVex(struct Graph *G , int u , int w)
{
	//返回v相对于w的下一个邻接点
	int i;
	for(i = w ; i < G->vexnum ; ++i){
        if(DebugLog==1) printf("\nNextAdjVex: i=%d",i); //for debug
		if(G->arcs[u][i] <= MaxInt && visited[i] == false){
            if(DebugLog==1) printf("\nu=%d,有邻接点",u); //for debug
			return i;
        }
	}
    if(DebugLog==1) printf("\n无邻接点"); //for debug
	return -1;
}

void BFS (struct Graph *G, int v)
{ 
    // 按广度优先非递归遍历连通图G
    // 参考《数据结构（第2版）》 p158伪代码实现

	int u,w;
    struct SQ sq;
	struct SQ *Q =&sq;

    // 初始化访问标志为false
	for(int i=0;i<G->vexnum;i++){
		visited[i]=false;
	}

	//printf("开始遍历：\n %c",G->vexs[v]);
	printf("开始遍历：");
    if(DebugLog==1)
        printf("\n");
	printf("%c",G->vexs[v]);

	visited[v] = true;  //访问第v个顶点，并置访问标志数组相应分量值为true 
	InitQueue(Q);      	//辅助队列Q初始化为空队列         
    EnQueue(Q, v);  	//首个顶点v入队 
    if(DebugLog==1) printf("\nu=%d, vex=%c,入队",u,G->vexs[u]); //for debug

    while(!QueueEmpty(Q))
    {   													//队列非空 
		u= DeQueue(Q);       													//队头元素出队并置为u
    	if(DebugLog==1) printf("\nu=%d, vex=%c,出队",u,G->vexs[u]); //for debug
		for(w = FirstAdjVex(G, u); w >= 0; w = NextAdjVex(G, u, w))
        {
			if(DebugLog==1) printf("\nw=%d, u=%d",w,u);//for debug
			
            //依次检查u的所有邻接点w ，FirstAdjVex(G, u)表示u的第一个邻接点 
			//NextAdjVex(G, u, w)表示u相对于w的下一个邻接点，w≥0表示存在邻接点 
			if(!visited[w])               //w为u的尚未访问的邻接顶点 
            {	           										
				//printf("\n %c",G->vexs[w]);
                if(DebugLog==1) printf("\n");
				printf("%c",G->vexs[w]);
                visited[w] = true;		  //置访问标志数组相应分量值为true 
				EnQueue(Q, w);			  //w入队
    			if(DebugLog==1) printf("\nw=%d, vex=%c,入队",w,G->vexs[w]); //for debug
			}//if 
		}//for
    }//while 
}//BFS

int Min(ClosEdge SZ, struct Graph *G)
{//找lowcost最小的顶点下标 
	int i=0,j,k,min;
	while(!SZ[i].lowcost) i++;//lowcost为0的顶点不再计算，因已在U集合中 
	min=SZ[i].lowcost;//第一个不为0的值
	k=i;
	for(j=i+1;j<G->vexnum;j++)
		if(SZ[j].lowcost>0 && min>SZ[j].lowcost){//找到新的大于0的最小值
		    min=SZ[j].lowcost;
		    k=j;
	        } 
    return k;
}

void MiniSpanTree_PRIM(struct Graph *G, char u)
{
	int i,j,k;
	char u0,v0;
	ClosEdge closedge;
	k=LocateVex(G,u);  // 根据顶点值找到顶点u对应的下标k。这是初始顶点
	// 初始化closedge数组，对应u到V-U的每一个顶点的权值
	for(j=0;j<G->vexnum;j++){
		if(j!=k){
			closedge[j].adjvex=u;
			closedge[j].lowcost=G->arcs[k][j]; //把邻接矩阵中记录的权值赋到closedge数组（当前顶点k到其它顶点的权值）
		}
	}
    closedge[k].lowcost=0; // 把首个顶点并入U集（k是顶点u的下标）。closedge数组中的lowCost值0，表示该顶点已并入U集

	// 循环
	printf("最小生成树的各条边是：\n");
	for(i=1;i<G->vexnum;i++){
		k=Min(closedge,G);    // 求当前顶点到U集中路径最短的顶点。作为下一个新的顶点：第k个顶点

		// 打印当前顶点与新顶点的距离（权值）
		u0= closedge[k].adjvex;
		v0= G->vexs[k];
		printf("%c-%c 权值：%d 下标k：%d\n",u0,v0,closedge[k].lowcost,k);

		closedge[k].lowcost=0;                     //新的顶点并入U集（k是顶点u的下标）
		for(j=0;j<G->vexnum;j++){
			if(G->arcs[k][j]<closedge[j].lowcost){ //新顶点并入U集后，再次选择与V-U集中的顶点的最小边
				closedge[j].adjvex=G->vexs[k];
				closedge[j].lowcost=G->arcs[k][j]; // 把邻接矩阵中记录的权值，刷新到closedge数组（更新后的顶点到其它顶点的权值）
			}
		}
	}
}

void ShortPath_DIJ(struct Graph *G,int v0 , PathMatrix P , PathDistance D)
{
    // 用Dijkstra算法求有图G中v0顶点到其余顶点v的最短路径P[v]及其带权长度D[v]
    // 若P[v][w]为true，则w是从v0到v当前求得最短路径上的顶点
    // final[v]为true时v属于S集，即已经求得从v0到v的最短路径
    // 参考教材p189算法代码
	int v,w,i,j,min;
	//Boolean final[MaxVNum];    //S数组，true表示已经求得从v0到该顶点v的最短距离

	// 初始化final，D，P
	// 基于源顶点v0
    for(v=0;v<G->vexnum;v++){
		final[v]=false;      //所有顶点初值全部设为false，即尚未求得最短距离 
		D[v]=G->arcs[v0][v]; //辅助向量D的每个分量表示该顶点v到v0的最短路径。初值为权值

		for(w=0;w<G->vexnum;w++){ //初始化P表，0表示没有路径
			P[v][w]=0;
        }
        if(D[v]<MaxInt){          //基于源顶点v0刷新P表，只有D表中的值不为MaxInt时才表示有直达路径
		    P[v][v0]=P[v][v]=1;
        }
	}//for

    // 更新final，P，D
	D[v0]=0;       //v0到v0的最短路径定义为0
	final[v0]=true;//v0顶点并入S集

 /*----------开始主循环，每次求得v0到某个v顶点的最短路径，并到加到S集---------*/
	for(i=1;i<G->vexnum;i++){//对其余 G->vexnum-1个顶点，依次进行计算 
		min=MaxInt;
		for(w=0;w<G->vexnum;w++){//对所有顶点进行检查 
		    if(!final[w] && D[w]<min){  // w顶点在V-S中 && w顶点离v0顶点更近
		    	v=w;                    // 循环中会多次赋值，取最后一次赋值
		    	min=D[w];
                if(DebugLog) printf("DIJ主循环-1，w=%d,min=%d\n", w,min);
		    }//if
        }//for
    	final[v]=true;                  //将离v0顶点最近的v加入S集
        if(DebugLog) printf("DIJ主循环-1结束，v=%d\n\n", v);

        // 更新当前最短路径及距离（S集中有新顶点，需更新最短路径）
		// 循环遍历V-S集中的顶点w，更新它们到源点v0的最短路径值
    	for(w=0;w<G->vexnum;w++){//根据主循环当前识别的v顶点，V-S集中顶点w到V0的距离和路径数据 
    	    //if(!final[w] && min<MaxInt && G->arcs[v][w]<MaxInt && (min+G->arcs[v][w]<D[w])){
    	    //if(!final[w] && (min+G->arcs[v][w]<D[w])){
    	    if(!final[w] && ((min+G->arcs[v][w])<D[w])){
    	    	D[w]=min+G->arcs[v][w]; // 更新顶点w到源点v0的最短路径
    	    	for(j=0;j<G->vexnum;j++)
    	    	   P[w][j]=P[v][j];  //即P[w]=P[v]
    	    	P[w][w]=1;
    	    }//if
        }//for
    }//for
}//ShortPath_DIJ

struct StartVex SetStartVex(struct Graph *G)
{
	int i;
	char c;
	struct StartVex StartVex;
	//printf("请输入遍历连通图的起始点：\n"); 
	printf("源点是："); 
	fflush(stdin);
	scanf("%c",&c);
	//printf("源点是%c \n",c);
	for(i=0;i<=G->vexnum;i++){
		if(c == G->vexs[i])
			break;
	} 
	while(i >= G->vexnum){
		printf("该点不存在，请重新输入！\n");
		printf("请输入遍历连通图的起始点：\n");
		scanf("%c",&c);
	    printf("您输入的是%c \n",c);
		for(i = 0 ; i < G->vexnum ; ++i){
			if(c == G->vexs[i])
				break;
		}
	}
	StartVex.index =i;
	StartVex.value =c;
	return StartVex;
}

Boolean ResultCheck(struct Graph *G)
{
	for(int i=0; i<G->vexnum; i++){
		if(!visited[i]){
			printf("\n遍历完成，这是一个非连通网！\n"); 
			return false; 
		}
	}
	printf("\n遍历完成，这是一个连通网！\n");
	return true;
}
void ResultReport(struct Graph *G, int v0, PathMatrix P, PathDistance D)
{
    int i,j;
    printf("****** Dijkstra算法结果输出******\n");
    printf("输出S数组：\n");
	for(i=0;i<G->vexnum;i++){
		printf("%d ",final[i]);
		//printf("\n"); 
	}
    
	printf("\n输出D数组：\n");
	for(i=0;i<G->vexnum;i++){
		printf("%d ",D[i]);
		//printf("\n"); 
	}

	printf("\n源点v0(%c)到其它顶点的最短路径：\n",G->vexs[v0]);
	for(i=0;i<G->vexnum;i++)
	  if(i!=v0)
	     printf(" %c-%c  路径长度:%d\n",G->vexs[v0],G->vexs[i],D[i]);

	//printf("\n源点v0到对应的Path矩阵：\n");
	printf("源点v0(%c)到其它顶点的Path矩阵：\n",G->vexs[v0]);
	for(i=0;i<G->vexnum;i++){
		for(j=0;j<G->vexnum;j++)
		   printf("%2d",P[i][j]);
		printf("\n"); 
	}
}
int main()
{
	// 1.创建无向网
	struct Graph *G =NULL;
	G =CreateUDN();

	// 2.判断连通性
	printf("请输入遍历连通图的源点...\n"); 
	struct StartVex Start=SetStartVex(G); // 设置搜索起点
	BFS(G,Start.index);    // 广域搜索
    Boolean conn =ResultCheck(G);     // 对搜索结果进行检查，判断是否连通
    if(!conn)
    {
        printf("这是一个非连通网，程序退出！");
        return 0;
    }

    // 3.最小生成树
	MiniSpanTree_PRIM(G, Start.value);
	
    // 4.求最短路径（给定源点，求其到其他顶点的最短路径）
	PathMatrix p;
	PathDistance d;

	printf("请输入计算最短路径的源点...\n");
	Start =SetStartVex(G); // 设置搜索起点
    int v0= Start.index;
	ShortPath_DIJ(G,v0,p,d);//DIJ算法计算最短路径
    ResultReport(G,v0,p,d); //DIJ算法输出的源点到其它顶点的最短路径，和PathMatrix、ShortPathTable

	return 0;
}