typedef int ElemType;
typedef struct LNode
{
	ElemType data;
	struct LNode* next;
}LNode, *QueuePtr;
typedef struct LinkQueue
{
	QueuePtr fornt;
	QueuePtr rear;
}LinkQueue;
void InitQueue(LinkQueue& Q);
void DestroyQueue(LinkQueue& Q);
void ClearQueue(LinkQueue& Q);
bool QueueEmpty(LinkQueue Q);
int QueueLength(LinkQueue Q);
void GetHead(LinkQueue Q, ElemType& e);
void QueueTraverse(LinkQueue Q);
void EnQueue(LinkQueue& Q, ElemType e);
void DeQueue(LinkQueue& Q, ElemType& e);

#pragma once
