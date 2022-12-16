#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TEST_COUNT 100
#define ARRY_LEN 20

struct StackNode // 定义链栈节点的数据结构
{
    char bracket;
    struct StackNode *next;
};

struct MatchResult // 定义字符比对的结果返回值。包括栈顶节点，挂链时产生的指针（作为当前最新的链表头指针）、单个字符匹配的情况（括号匹配、不匹配）
{
    struct StackNode *NewHead;
    int Match;
};


/* 自定义函数的调用层次：
   main-> InputString
       -> SearchMatchBrackets-> MatchAction -> Push
                                            -> Pop
 */
void InputStrings(int n, char *pstr);                             // 输入一个算术表达式到指针指向的数组。是一个字符串，包含括号和其它字符

int SearchMatchBrackets(char *pstr);                              // 对输入的字符串进行搜索同时进行括号匹配，返回结果为1匹配、0不匹配
struct MatchResult MatchAction(struct StackNode *head, char c);   // 对字符串中的单个字符进行匹配检查，包括相应的链栈节点的增加、删除操作
struct StackNode *Push(struct StackNode *head, char c);           // 增加1个栈顶节点，当字符为左括号时调用
struct StackNode *Pop(struct StackNode *head);                    // 删除1个栈顶节点，当字符为右括号且与栈顶配对时调用

int left_cnt;
int right_cnt;

int main()
{
	char str[ARRY_LEN];           // 用来存放键盘输入的字符串
    int match;
    /* 外层for循环，每一轮循环提示输入一个表达式 */
    for(int i=1; i<TEST_COUNT; i++){
        InputStrings(i, str);         //输入一个字符串，存入到数组str[ARRY_LEN]。数组名是数组的首地址，是一个指针常量
        left_cnt =0;
        right_cnt =0;

		    if (strcmp(str,"exit") != 0){
            match = SearchMatchBrackets(str);
            if(match ==1){
                printf("第%d次输入的表达式的括号全部匹配!\n", i);
            }else if(match == 0){
                printf("第%d次输入的表达式的括号不匹配!\n", i);
            }
		    }else{
            printf("*****************\n");
            printf("**exit 结束实验**\n");   // 输入exit，退出循环
            printf("*****************\n");
			  break;
        }
    }
    return 0;
}

// 输入字符串时调用这个函数
void InputStrings(int n, char *pstr)
{
	// 键盘输入的字符存入数组str[]，由指针pstr索引
	printf("******************************************************\n");
	printf("第%d次输入，请输入表达式字符串：",n);
    scanf("%s",pstr);
	return;
}


/* 1.对于输入给定的字符串表达式，从左到右逐个取出字符，对链栈表进行匹配
   2.在这个过程中对链栈进行更新：包括节点的插入、删除；头指针及头节点next指针的修改
   3.打印匹配的结果 */
int SearchMatchBrackets(char str[])
{
    struct StackNode *head = NULL;
    struct MatchResult Result;

    /* 内层循环，针对每个表达式字符串，循环遍历字符串的每一个字符 */
    for(int i=0;i<ARRY_LEN;i++){
        char k = str[i];
        if(k=='\0'){
            printf("检测到字符串结束标志，i=%d \n",i);
            break;
        }

        Result = MatchAction(head, k);
        head = Result.NewHead;
        
        if(Result.Match == 0){
            printf("检测到不匹配的括号！\n");
            break;
        }else{
            continue;
        }
    }
    return Result.Match; 
}

/* 对遍历到的单个字符的处理：检查是括号还是其它字符；左括号还是右括号，相应地进行入栈或判断是否配对。并输出配对结果和栈顶指针（这两个维度的结果，存入结构体，作为函数结果返回） */
struct MatchResult MatchAction(struct StackNode *head, char c)
{
    char k =c;
    int match =1;
    int is_bracket = 0;

    struct StackNode *NewHead;
    struct MatchResult Result;

    switch(k)
    {
        case '(':
        case '[':
        case '{':
        {
          left_cnt++;
          printf("识别第%d个左括号：%c 入栈\n", left_cnt,k); 

          NewHead = Push(head, k);
          match = 2;
          break;
        }

        case ')':
        {
          right_cnt++;
          
          if(head == NULL ){       // 在栈为空的情况下检测到右括号，不符合预期，肯定不匹配
            match = 0;
            printf("识别第%d个右括号：%c 不匹配!结束当前表达式的匹配！！！\n", right_cnt,k);
          }else if( head->bracket == '(' ){
            printf("识别第%d个右括号：%c 匹配\n", right_cnt,k);
            NewHead = Pop(head);
          }else if((head->bracket == '[') || (head->bracket == '{') ){
            match = 0;
            printf("识别第%d个右括号：%c 不匹配!\n", right_cnt,k);
          }
          break;
        }
        
        case ']':
        {
          right_cnt++;
          
          if(head == NULL ){       // 在栈为空的情况下检测到右括号，不符合预期，肯定不匹配
            match = 0;
            printf("识别第%d个右括号：%c 不匹配!结束当前表达式的匹配！！！\n", right_cnt,k);
          }else if( head->bracket == '[' ){
            printf("识别第%d个右括号：%c 匹配\n", right_cnt,k);
            NewHead = Pop(head);
          }else if((head->bracket == '(') || (head->bracket == '{') ){
            match = 0;
            printf("识别第%d个右括号：%c 不匹配!\n", right_cnt,k);
          }
          break;
        }
        
        case '}':
        {
          right_cnt++;

          if(head == NULL ){       // 在栈为空的情况下检测到右括号，不符合预期，肯定不匹配
            match = 0;
            printf("识别第%d个右括号：%c 不匹配!结束当前表达式的匹配！！！\n", right_cnt,k);
          }else if( head->bracket == '{' ){
            printf("识别第%d个右括号：%c 匹配\n", right_cnt,k);
            NewHead = Pop(head);
          }else if((head->bracket == '[') || (head->bracket == '(') ){
            match = 0;
            printf("识别第%d个右括号：%c 不匹配!结束当前表达式的匹配！！！\n", right_cnt,k);
          }
          break;
        }
        default:
        {
          printf("识别到普通字符：%c\n", k);
            match = 3;
        }
     }
     Result.NewHead = NewHead;
     Result.Match = match;
     return Result;
}

// 入栈，实际上是创建一个节点
struct StackNode *Push(struct StackNode *head, char c)
{
    struct StackNode *p = (struct StackNode *)malloc(sizeof(struct StackNode));
    p->bracket =c;
    if(head == NULL){
        p->next = NULL; //空栈，push进来的这个节点，不指向任何节点
    }else{
        p->next = head; //非空栈，push进来的节点作为头节点，原先的头节点被头节点的next指针指向
    }
    return p;           //更新头指针指向新生成的头节点。（用栈的术语来说，是指向了栈顶）
}

// 出栈，实际上是删除一个节点
struct StackNode *Pop(struct StackNode *head)
{
    struct StackNode *p;
    if(head == NULL){
        return NULL;          //空栈，返回错误
    }else{
        p = head->next;      //暂存下一个节点的指针，作为头指针（被函数返回）。即指向原链表的第2个节点
        free(head);
        return p;
    }
}
