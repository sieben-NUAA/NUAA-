#include<iostream>
#include"Queue.h"
using namespace std;

//��ʼ����
void InitQueue(LinkQueue& Q)
{
	Q.fornt = Q.rear = (LNode*)malloc(sizeof(LNode));
	if (Q.fornt == NULL) { cout << "�ڴ�����ʧ�ܣ�\n"; return; }
	Q.fornt->next = NULL;
}

//���ٶ�
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

//��ն�
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

//�жϿ�
bool QueueEmpty(LinkQueue Q)
{
	return Q.fornt == Q.rear;
}

//�󳤶�
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

//��ȡ��ͷԪ��
void GetHead(LinkQueue Q, ElemType& e)
{
	if (QueueEmpty(Q)) { cout << "�նӣ��޶�ͷԪ�أ�\n"; return; }
	e=Q.fornt->next->data;
}

//�����
void QueueTraverse(LinkQueue Q)
{
	if (QueueEmpty(Q)) { cout << "�նӣ��������\n"; return; }
	LNode* p = Q.fornt->next;
	while (p)
	{
		cout << p->data<<"  ";
		p = p->next;
	}
	cout << endl;
}

//���
void EnQueue(LinkQueue& Q, ElemType e)
{
	LNode* s = (LNode*)malloc(sizeof(LNode));
	if (s == NULL) { cout << "�ڴ�����ʧ�ܣ�\n"; return; }
	s->data = e;
	s->next = NULL;
	Q.rear->next = s;
	Q.rear = s;
}

//����
void DeQueue(LinkQueue& Q, ElemType& e)
{
	if (QueueEmpty(Q)) { cout << "�նӣ��޷��ٳ��ӣ�\n"; return; }
	LNode* p = Q.fornt->next;
	e = p->data;
	Q.fornt->next = p->next;
	if (Q.rear == p) Q.rear = Q.fornt;
	free(p);
}