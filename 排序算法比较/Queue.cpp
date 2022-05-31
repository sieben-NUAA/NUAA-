#include<iostream>
#include"Queue.h"
using namespace std;

//初始化队
void InitQueue(LinkQueue& Q)
{
	Q.fornt = Q.rear = (LNode*)malloc(sizeof(LNode));
	if (Q.fornt == NULL) { cout << "内存申请失败！\n"; return; }
	Q.fornt->next = NULL;
}

//销毁队
void DestroyQueue(LinkQueue& Q)
{
	LNode* p = Q.fornt;
	while (Q.fornt->next != NULL)
	{
		p = Q.fornt->next;
		Q.fornt = p->next;
		free(p);
	}
	free(Q.fornt);
	Q.fornt = NULL;
	Q.rear = NULL;
}

//清空队
void ClearQueue(LinkQueue& Q)
{
	LNode* p = Q.fornt;
	while (Q.fornt->next != NULL)
	{
		p = Q.fornt->next;
		Q.fornt = p->next;
		free(p);
	}
	Q.rear = Q.fornt;
}

//判断空
bool QueueEmpty(LinkQueue Q)
{
	return Q.fornt == Q.rear;
}

//求长度
int QueueLength(LinkQueue Q)
{
	if (QueueEmpty(Q)) return 0;
	LNode* p = Q.fornt;
	int n = 0;
	while (p->next)
	{
		n++;
		p = p->next;
	}
	return n;
}

//获取队头元素
void GetHead(LinkQueue Q, ElemType& e)
{
	if (QueueEmpty(Q)) { cout << "空队，无队头元素！\n"; return; }
	e=Q.fornt->next->data;
}

//输出队
void QueueTraverse(LinkQueue Q)
{
	if (QueueEmpty(Q)) { cout << "空队，空输出！\n"; return; }
	LNode* p = Q.fornt->next;
	while (p)
	{
		cout << p->data<<"  ";
		p = p->next;
	}
	cout << endl;
}

//入队
void EnQueue(LinkQueue& Q, ElemType e)
{
	LNode* s = (LNode*)malloc(sizeof(LNode));
	if (s == NULL) { cout << "内存申请失败！\n"; return; }
	s->data = e;
	s->next = NULL;
	Q.rear->next = s;
	Q.rear = s;
}

//出队
void DeQueue(LinkQueue& Q, ElemType& e)
{
	if (QueueEmpty(Q)) { cout << "空队，无法再出队！\n"; return; }
	LNode* p = Q.fornt->next;
	e = p->data;
	Q.fornt->next = p->next;
	if (Q.rear == p) Q.rear = Q.fornt;
	free(p);
}