#include<iostream>
#include"Stack.h"
using namespace std;
//≥ı ºªØ’ª
void InitStack(SeqStack &S)
{
	S.pBase = (ElemType*)malloc(STACKINITSIZE * sizeof(ElemType));
	if (S.pBase == NULL) { cout << "…Í«Îƒ⁄¥Ê ß∞‹£°|0"; return; }
	S.pTop = S.pBase;
	S.stacksize = STACKINITSIZE;
}

//À≥–Ú’ªœ˙ªŸ
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

//À≥–Ú’ª«Âø’
void ClearStack(SeqStack& S)
{
	S.pTop = S.pBase;
}

//≈–∂œ «∑ÒŒ™ø’
bool StackEmpty(SeqStack& S)
{
	return S.pBase == S.pTop;
}

//∑µªÿ’ª÷–‘™Àÿ∏ˆ ˝
int StackLength(SeqStack& S)
{
	return S.pTop - S.pBase;
}

//∑µªÿ’ª∂•‘™Àÿ
void GetTop(SeqStack S, ElemType& e)
{
	if (S.pBase == S.pTop) { cout << "ø’’ª£¨Œﬁ’ª∂•‘™Àÿ!\0"; return; }
	e = *(S.pTop - 1);
}


//—π’ª
void Push(SeqStack& S, ElemType e)
{
	if (S.pTop - S.pBase >= S.stacksize)
	{
		S.pBase = (ElemType*)realloc(S.pBase, (S.stacksize + STACKINCREMENT) * sizeof(ElemType));
		if (S.pBase == NULL) { cout << "…Í«Îƒ⁄¥Ê ß∞‹£°\0"; return; }
		S.pTop = S.pBase + S.stacksize;
		S.stacksize += STACKINCREMENT;
	}
	*S.pTop = e;
	S.pTop++;
}

//µØ’ª
void Pop(SeqStack& S, ElemType& e)
{
	if (S.pBase == S.pTop) { cout << "ø’’ª£¨Œﬁ∑®µØ≥ˆ£°\0"; return; }
	e = *(S.pTop - 1);
	S.pTop--;
}
