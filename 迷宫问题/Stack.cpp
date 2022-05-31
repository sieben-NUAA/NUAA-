#include<iostream>
#include"Stack.h"
using namespace std;
//��ʼ��ջ
void InitStack(SeqStack &S)
{
	S.pBase = (ElemType*)malloc(STACKINITSIZE * sizeof(ElemType));
	if (S.pBase == NULL) { cout << "�����ڴ�ʧ�ܣ�|0"; return; }
	S.pTop = S.pBase;
	S.stacksize = STACKINITSIZE;
}

//˳��ջ����
void DestroyStack(SeqStack& S)
{
	if (S.pBase != NULL)
	{
		free(S.pBase);
		S.pBase = NULL;
	}
	S.pTop = NULL;
	S.stacksize = 0;
}

//˳��ջ���
void ClearStack(SeqStack& S)
{
	S.pTop = S.pBase;
}

//�ж��Ƿ�Ϊ��
bool StackEmpty(SeqStack& S)
{
	return S.pBase == S.pTop;
}

//����ջ��Ԫ�ظ���
int StackLength(SeqStack& S)
{
	return S.pTop - S.pBase;
}

//����ջ��Ԫ��
void GetTop(SeqStack S, ElemType& e)
{
	if (S.pBase == S.pTop) { cout << "��ջ����ջ��Ԫ��!\0"; return; }
	e = *(S.pTop - 1);
}


//ѹջ
void Push(SeqStack& S, ElemType e)
{
	if (S.pTop - S.pBase >= S.stacksize)
	{
		S.pBase = (ElemType*)realloc(S.pBase, (S.stacksize + STACKINCREMENT) * sizeof(ElemType));
		if (S.pBase == NULL) { cout << "�����ڴ�ʧ�ܣ�\0"; return; }
		S.pTop = S.pBase + S.stacksize;
		S.stacksize += STACKINCREMENT;
	}
	*S.pTop = e;
	S.pTop++;
}

//��ջ
void Pop(SeqStack& S, ElemType& e)
{
	if (S.pBase == S.pTop) { cout << "��ջ���޷�������\0"; return; }
	e = *(S.pTop - 1);
	S.pTop--;
}
