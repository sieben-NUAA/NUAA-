#define STACKINITSIZE 256
#define STACKINCREMENT 128
typedef int ElemType;
typedef struct SeqStack
{
	ElemType* pBase;
	ElemType* pTop;
	int stacksize;
}SeqStack;
void InitStack(SeqStack& S);
void DestroyStack(SeqStack &S);
void ClearStack(SeqStack&S);
bool StackEmpty(SeqStack &S);
int StackLength(SeqStack&S);
void GetTop(SeqStack S,ElemType &e);
void StackTraverse(SeqStack &S);
void Push(SeqStack &S,ElemType e);
void Pop(SeqStack &S,ElemType &e);


#pragma once
