#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* 定义链表的数据结构 */
struct LinkList
{
	char word[20];
	int num;
	struct LinkList *next;
};

//函数声明
struct LinkList *TypeStrBuildList(int n);           // 调用下面3个函数，返回值为头指针
char *InputStrings(int n,char str[]);               /* 函数输入参数n表示第n次调用、函数返回值是键盘输入的字符 */
struct LinkList *AddNode(char *str);                /* 函数输入参数str是从键盘输入的字符串、函数返回值是生成的节点的结构体指针 */
void SearchList(struct LinkList *head, char *str);  /* 函数输入参数head是链表的头指针，函数输入参数str是从键盘输入的字符串指针 */
                                                    /* 在键盘输入字符后搜索链表，插入一个新的节点，或更新已有节点的计数域 */

void PrintNode(struct LinkList *p);                  /* 打印单个节点信息 */
void PrintAllNodes(struct LinkList *top);            /* 打印链表的全部节点信息 */
void PrintTopKnodes(struct LinkList *head, int k);   /* 打印词频最高的前k个单词 */

void SortDesc(struct LinkList *head);                  /* 冒泡排序 */
void NodeSwap(struct LinkList *a, struct LinkList *b); /* 排序中交换节点操作 */

void FreeAllNodes(struct LinkList *top);               /* 释放链表所有节点内存 */



int main()
{
    /* 定义链表的头指针变量，初值为空。用来指向链表的头节点。后面的所有操作都需要从头指针开始遍历链表 */
	struct LinkList* head = NULL;
    
    /* 设置实验条件：输入的单词个数n，出现频度最高的前k个单词 */
	int n, k;
	printf("本次实验需要输入的单词个数：");
	scanf("%d", &n);
	printf("您的输入数字是：%d \n", n);
	printf("显示出现次数最多的前k个单词：");
	scanf("%d", &k);
	printf("您的输入数字是：%d \n", k);

    head = TypeStrBuildList(n); 
    //printf("***字符串输入，产生的原始链表：***\n"); 
    //PrintAllNodes(head);
    
    /* 根据num计数对链表节点排序，并打印。冒泡排序、降序 */
    SortDesc(head); // 冒泡排序
    //printf("***完成排序操作，排序后的链表：***\n");    
    //PrintAllNodes(head);       
    PrintTopKnodes(head, k);   

    /* 释放所有节点内存，退出程序 */
    FreeAllNodes(head);
	return 0;
}


struct LinkList *TypeStrBuildList(int n)
{
	struct LinkList *head = NULL;   
	char str[20] = "\a";           
	char *pstr =str;

	/* 产生头节点 */
    pstr = InputStrings(1,str);       
	head = AddNode(pstr);            
//	printf("输入第1个字符，产生头节点！新节点的数据结构是：\n");
//	PrintNode(head);                  // 打印头节点内容

	/* 产生后续节点 */
	for (int i = 1; i < n; i++)             
	{
		pstr = InputStrings(i+1, str);  
		
		if (strcmp(pstr,"exit") != 0){
		    SearchList(head, pstr);    
		}else{
            printf("*****************\n");
            printf("**exit 结束实验**\n");   
            printf("*****************\n");
			break;
        }
	}
	return head;
}


char *InputStrings(int n, char str[])
{
	
	// printf("******************************************************\n");
	printf("第%d次输入，请输入单词：",n);
	//fgets(str,10,stdin);
    scanf("%s",str);
	// printf("第%d次输入的单词是：%s \n",n ,str);	
	return str;
}

// 增加一个节点
struct LinkList *AddNode(char *str)
{
	struct LinkList node;
	struct LinkList *p = NULL;
	p = (struct LinkList*)malloc(sizeof(node));
	strcpy(p->word,str);
	p->num = 1;
	p->next = NULL;

	return p;
}

// 输入后续字符时调用（键盘输入的第2个单词开始进入这个函数）
void SearchList(struct LinkList *head, char *str)
{
	struct LinkList *top;            
	top = head;                      
	if(strcmp(str,top->word) == 0){  
		top->num = top->num + 1;     
		return;                      
	}else{                           
		if(top->next != NULL) {      
			top =top->next;          
		}else{
			top = top;               
		}
	}
	/*判断中间节点，直到链尾*/
	while (top != NULL)               
	{
        if(strcmp(str,top->word) == 0){
			top->num = top->num + 1;
			break;                     
		}
		else{ //没有找到匹配的字符
			if(top->next != NULL) {    
				top =top->next;        
			}
			else{                      
                top->next = AddNode(str);
				break;                 
			}
		}
	}
    return;
}

/* 打印链表中的单个节点，被PrintAllNodes调用 */
void PrintNode(struct LinkList *p)
{
	printf("top  0x%p \n",p);
	printf("top->next:0x%p \n",p->next);
	printf("top->word:%s \n",p->word);
	printf("top->num:%d \n",p->num);
	printf("\n");
}

/* 打印链表中的所有节点 */
void PrintAllNodes(struct LinkList *top)
{
    struct LinkList *temp = top;
	while(temp != NULL){
		temp = top->next;
		PrintNode(top);
		top = temp;
	}
}

/* 释放链表中的所有节点 */
void FreeAllNodes(struct LinkList *top){
	struct LinkList *temp = top;
	while(temp != NULL){
		temp = top->next;
		free(top);
		top = temp;
	}
    printf("All Nodes are freed!");
}

/* 冒泡排序，降序排序 */
void SortDesc(struct LinkList *head)
{
    struct LinkList *p, *q;                            
	for(p = head; p->next != NULL; p = p->next){       
		for(q = head; q->next != NULL; q = q->next){   // q在一条链上从头到尾比较相邻节点的num字段，重复扫多轮链表
			if((q->num) < (q->next->num)){
				NodeSwap(q, q->next);                  // 冒泡过程中的大小比较、交换位置
			}
		}
	}
    //printf("SortDesc()!\n");
}


void NodeSwap(struct LinkList *a, struct LinkList *b){
    // 申请临时节点
	struct LinkList *tmp_node = (struct LinkList*)malloc(sizeof(struct LinkList));
    // 交换
	tmp_node->num = a->num;
	strcpy(tmp_node->word,a->word);
	
	a->num = b->num;
	strcpy(a->word,b->word);
	
	b->num = tmp_node->num;
	strcpy(b->word,tmp_node->word);
	// 释放临时节点
	free(tmp_node);
}

void PrintTopKnodes(struct LinkList *head, int k){
    printf("PrintTopKnodes()!\n");
    printf("*****************************\n");
    printf("**输入出现频度前%d的单词排序**\n",k);
    printf("*****************************\n");
    int i = 1;
	while((head != NULL) && (i-1 != k)){
		printf("排序%d的单词:%s\n",i,head->word);
		printf("出现次数是 :%d\n\n",head->num);
		head = head->next;
		i++;
	}	
}