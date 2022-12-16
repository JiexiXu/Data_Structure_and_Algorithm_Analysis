#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct HTNode{
	char data;
	float weight;
	int parent, lchild, rchild;
};

struct Alphabet{
	int number;
	float ratio;
};

struct queue{
	int data;
	struct queue* next;
};

/* 字符序列输入、字符权重计算 */
void InputCharSeq(char seq[]);
int WeightCount(char seq[], struct Alphabet abc[]);

/* 构造哈夫曼树 */ 
void BuildHuffmanTree(struct HTNode* htree,char** htable,struct Alphabet abc[], int n,int m);
void InitHuffmanTree(struct HTNode* htree,struct Alphabet abc[],int m);
void BuildTree(struct HTNode* htree,int n,int m);
void PrintTree(struct HTNode* htree,int m); 
void BuildTable(struct HTNode* htree,char** htable,int n); 
void PrintTable(struct HTNode* htree,char** htable,int n);

/* 哈夫曼编码、解码 */
void HuffmanEncode(struct HTNode* htree,char** htable,char *seq,char *seq_enc);
void HuffmanDecode(struct HTNode* htree,char *seq_enc,int m);

/* 哈夫曼树遍历,四种方法 - 前序、中序、后序、层次 */
void TreeTranversal(struct HTNode* htree,int m);
void TreeTranvPreOrder(struct HTNode* htree,int root); 
void TreeTranvInOrder(struct HTNode* htree,int root); 
void TreeTranvPostOrder(struct HTNode* htree,int root);
void TreeTranvLevelOrder(struct HTNode* htree,struct queue *head,struct queue *tail,int m);
int SearchRoot(struct HTNode* htree,int m);
struct queue *enqueue(struct queue *tail,int i);
struct queue *dequeue(struct queue *head);


int main(){
	char seq[100];           // 数组保存输入的字符串(正文)
	struct Alphabet abc[26];  // 数组保存正文中各字母出现的次数和比例
    int n,m;
	
    /* 统计输入序列中不重复的字符的个数，以及权重 */
    InputCharSeq(seq);
    n = WeightCount(seq, abc);
    m = 2*n-1;

    /* 构造哈弗曼树 */
    struct HTNode*  htree;
    htree=(struct HTNode*)malloc((m+1)*sizeof(struct HTNode));//*******有m+1项，从1开始 ****// 
	char** htable;
	htable=(char**)malloc((n+1)*sizeof(char*));//分配n个字符编码的头指针向量 
	BuildHuffmanTree(htree,htable,abc,n,m);

    /* 使用哈弗曼树对输入序列进行编码 */
	char seq_enc[100];
	HuffmanEncode(htree,htable,seq,seq_enc);//编码，结果保存到数组seq_enc，并打印显示
	HuffmanDecode(htree,seq_enc, m);        //解码，并打印显示

    /* 遍历哈弗曼树，用四种方法遍历哈弗曼树，并分别打印输出 */
    TreeTranversal(htree,m);
    return 0;
}


void InputCharSeq(char seq[])
{
	// 键盘输入的字符存入数组str[]，由指针pstr索引
	printf("******************************************************\n");
    printf("请输入字符序列正文，以回车键结束（26个英文字母，不区分大小写）：\n");
	scanf("%s",seq);
	return;    
}
int WeightCount(char seq[], struct Alphabet abc[])
{
    int i,j,n=0;
    float a,b;
    /* 初始化一个字母表数组，保存正文中各字母出现的次数和比例（权重） */
//	struct Alphabet abc[26];
    for (i=0;i<26;i++){
	   abc[i].number=0;      // 次数初始为0
	}    

    /* 遍历输入序列，统计字符出现的次数 */
	int len=strlen(seq);//字符串长度
    // 对输入字符串进行遍历 
    // 数组abc的每个Entry，下标为0时表示字母a，下标为25时表示字母z
    // 数组seq的每个Entry，内容为输入的字符（英文字母）
	for (j=0;j<len;j++){
        // 预处理，把输入的大写字母转换为小写
        if(seq[j]>='A' && seq[j]<='Z'){
            seq[j] += 32;
        }
        //统计每个字母出现的次数，并记录到data数组中 
	    abc[seq[j]-'a'].number++; //seq[j]-'a' 为字母与'a'之间的偏移，用来表示字母从1~26之间的编号
	}
    // 统计每个字母出现的次数
	for (i=0;i<26;i++){
	    if(abc[i].number>0){
	      //printf("%c=",'a'+i);
	      //printf("%d\n",abc[i].number);
	      n++;
	    }
	  }
	printf("\n序列中出现不同字符个数：n=%d",n);

    /* 统计字符出现的权重 */
    // 统计每个字母出现的比例
	for(i=0;i<26;i++){
		a=abc[i].number;
	    float b=a/len;
	    abc[i].ratio=b;
	}
    // 打印输出每个字条的权重
	printf("\n各字符的权重：");
	for(i=0;i<26;i++){
		if(abc[i].number>0){
			printf("\n%c：%.4f",i+'a',abc[i].ratio);
		}
	}
    return n;
}

void BuildHuffmanTree(struct HTNode* htree,char** htable,struct Alphabet abc[], int n,int m)
{
	InitHuffmanTree(htree,abc,m); // 初始化哈夫曼树

	BuildTree(htree, n, m);     // 创建哈夫曼树 
	PrintTree(htree,m);         //打印哈夫曼树

	BuildTable(htree,htable, n);//创建哈夫曼编码表 
	PrintTable(htree,htable, n);//打印哈夫曼编码表
}

void InitHuffmanTree(struct HTNode* htree,struct Alphabet abc[],int m)
{
	int i,k;
    /* 初始化 */
    // 循环2n-1次，从1号单元开始，依次将所有单元中的双亲、左孩子、右孩子初始化为0；
    // 同时初始化data为0和weitht为值（留待一下个循环进行初始化）
    // step1
	for(i=1;i<=m;i++){
		htree[i].data=0;
		htree[i].weight=2;//大于1
		htree[i].lchild=0;htree[i].parent=0;htree[i].rchild=0;
	}
    // 将abc数组中已有的字母，用来初始化前n个单元中叶子节点的权值
    // n为abc数组中已有字母的个数
    // step2
    i=1;
	for(k=0;k<26;k++){
		if(abc[k].number>0){
			htree[i].data='a'+k;
			htree[i].weight=abc[k].ratio;
			i++;
		}
	}
}

void BuildTree(struct HTNode* htree,int n,int m){
	int  k1,k2;
	float* s;
	s=(float*)malloc((m+1)*sizeof(float));
	int i,a;
	for(a=1;a+n<=m;a++){
	k1=1;k2=1;
	 for(i=1;i<=m;i++){
		if(htree[k1].weight>htree[i].weight && htree[i].weight<1){
			k1=i; 
		}
	  }
	  s[k1]=htree[k1].weight;
	 htree[k1].weight=2;//任意一个大于1的数都可以
	 for(i=1;i<=m;i++){
		if(htree[k2].weight>htree[i].weight && htree[i].weight<1){
			k2=i;
		}
	 }
	 s[k2]=htree[k2].weight;
	 htree[k2].weight=2;//任意一个大于1的数都可以
	 
     htree[k1].parent=n+a;htree[k2].parent=n+a;
	 htree[n+a].lchild=k1;htree[n+a].rchild=k2;
	 htree[n+a].weight=s[k1]+s[k2];
	}
	for(i=1;i<=m;i++){
		htree[i].weight=s[i];
	}
	htree[m].weight=1;
}

void PrintTree(struct HTNode* htree,int m){
    printf("\n输出哈弗曼树：\n");
	int i;
	printf("字符\t  weight\tlchild\tparent\trchild\n");
	for(i=1;i<=m;i++){
		printf("%c\t%f\t%d\t%d\t%d\t\n",htree[i].data,htree[i].weight,htree[i].lchild,htree[i].parent,htree[i].rchild);
	}
	printf("\n");
}

void BuildTable(struct HTNode* htree,char** htable,int n){
	int i,c,f,start;
	char *cd;
	cd=(char*)malloc( n*sizeof(char));
	cd[n-1]='\0';
	for(i=1;i<=n;i++){
		start=n-1;
		c=i;
		f=htree[i].parent;
		while(f!=0){
			if(htree[f].lchild==c) cd[--start]='0';
			else cd[--start]='1';
			c=f;
			f=htree[f].parent;
		}
		htable[i]=(char*)malloc((n-start)*sizeof(char));
		strcpy(htable[i],&cd[start]);
	}
}

void PrintTable(struct HTNode* htree,char** htable,int n){
    printf("输出哈弗曼编码表：\n");
	int i,j,k;
	for(i=1;i<=n;i++){
		printf("%c:",htree[i].data);
		k=strlen(htable[i]);
		for(j=0;j<k;j++){
			printf(" %c ",htable[i][j]);
		}
		printf("\n");
	}
}

void HuffmanEncode(struct HTNode* htree,char** htable,char *seq,char *seq_enc){
	int i,k;
	int len=strlen(seq);
	for(i=0;i<len;i++){
		int k=1;
		while(seq[i]!=htree[k].data ){
			k++;
		}
		strcat(seq_enc,htable[k]);
	}
    printf("\n编码后的正文:");
	for(i=1;i<=strlen(seq_enc);i++){
		printf("%c",seq_enc[i]);
	}
}

void HuffmanDecode(struct HTNode* htree,char *seq_enc,int m){
	int i,j=1;
    printf("\n解码后的正文:");
    while(seq_enc[j]!='\0')
    {
        i=m;
        while(0 != htree[i].lchild && 0 != htree[i].rchild)//从顶部找到最下面
        {
            if('0' == seq_enc[j])//0 往左
            {
                i=htree[i].lchild;
            }
            else//1 往右
            {
                i=htree[i].rchild;
            }
            ++j;//下一个路径
        }
        printf("%c",htree[i].data);
    }
    printf("\n");
}

int SearchRoot(struct HTNode* htree,int m){
	int root;
	for(root=1;root<=m;root++){
	    if(htree[root].parent==0)break;
	}
	return  root;
}

void TreeTranversal(struct HTNode* htree,int m)
{
    // 先找到根结点
    int root;
	root=SearchRoot(htree,m);
    
	printf("\n分别用四种方法遍历树："); 
	printf("\n方法1：先序 \t"); 
	TreeTranvPreOrder(htree,root);

	printf("\n方法2：中序 \t"); 
	TreeTranvInOrder(htree,root);

	printf("\n方法3：后序 \t"); 
	TreeTranvPostOrder(htree,root);

	printf("\n方法4：层次 \t"); 
	struct queue *head;
	struct queue *tail;
	head=(struct queue *)malloc(sizeof(struct queue));
	head->next=NULL;
	head->data=root;
	tail =head;
	TreeTranvLevelOrder(htree,head,tail,m);
}

void TreeTranvPreOrder(struct HTNode* htree,int root) {
	if (root != 0) {
		printf("%f ",htree[root].weight);
		TreeTranvPreOrder(htree,htree[root].lchild);
		TreeTranvPreOrder(htree,htree[root].rchild);
	}
}

void TreeTranvInOrder(struct HTNode* htree,int root){
	if (root != 0) {
		TreeTranvPreOrder(htree,htree[root].lchild);
		printf("%f ",htree[root].weight);
		TreeTranvPreOrder(htree,htree[root].rchild);
	}	
} 

void TreeTranvPostOrder(struct HTNode* htree,int root){
	if (root != 0) {
		TreeTranvPreOrder(htree,htree[root].lchild);
		TreeTranvPreOrder(htree,htree[root].rchild);
		printf("%f ",htree[root].weight);
	}	
}

struct queue *enqueue(struct queue *tail,int i){
	struct queue *p;
	p=(struct queue *)malloc(sizeof(struct queue));
	p->data=i;
	p->next=NULL;
	tail->next=p;
	tail=p;
	return tail;
}

struct queue *dequeue(struct queue *head){
	head=head->next;
	return head;
}

void TreeTranvLevelOrder(struct HTNode* htree,struct queue *head,struct queue *tail,int m){
	int i;
	struct queue *p;
	p=head;
	// 按层次遍历，并把遍历的结果排队存入队列
	for(i=1;i<=m;i++){
		if(htree[p->data].lchild!=0){
			tail=enqueue(tail,htree[p->data].lchild);//入队操作 
		}
		if(htree[p->data].rchild!=0){
			tail=enqueue(tail,htree[p->data].rchild);
		}
	    p=p->next; 
	}
	// 在出队的过程中打印
	for(i=1;i<=m;i++){
     	printf("%f ",htree[head->data].weight);
	    head=dequeue(head);//出队操作
	}
}