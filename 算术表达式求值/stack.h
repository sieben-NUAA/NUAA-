#include<iostream>
#include<string>
#include<stdlib.h>
#define STC_IN_LEN 100
#define ST_Cre 10
using namespace std;

// ջ 
template<class T>
class Stack
{
private:
	T* data;
	T* base;
	T* top;
	int stacksize;
public:
	Stack(void) {
		data = new T[STC_IN_LEN];
		top = base = data;
		stacksize = STC_IN_LEN;
	}
	void ClearStack() {  // ��� 
		top = base;
	}
	bool StackEmpty() { // �п� 
		if (top == base) return true;
		else return false;
	}
	int StackLength() {  // ��ȡջ��Ԫ�ظ��� 
		return top - base;
	}
	T GetTop() {  // ��ȡջ�� 
		return *(top - 1);
	}
	T* GetBaseLoca() { // ��ȡջ�� 
		return base;
	}
	void Push(T num) { // ��ջ 
		if (top - base != stacksize) {
			*(top) = num;
			top++;
		}
		else {  // ��չ�ռ� 
			int i;
			T* tmp;
			tmp = new T[stacksize + ST_Cre];
			for (i = 0; i < stacksize; i++) tmp[i] = data[i];
			delete[]data;
			data = tmp;
			stacksize += ST_Cre;
			*(top) = num;
			top++;
		}
	}
	T Pop() { //��ջ 
		if (top != base) {
			top--;
			return *(top);
		}
		else return '\0';
	}
	void ShowStack() {  // ��ӡջ��Ԫ�� 
		int i;
		if (top - base == 0) cout << "��ջ" << endl;
		else {
			cout << "ջ�׵�ջ����";
			for (i = 0; i < top - base; i++) {
				cout << data[i] << " ";
			}
			cout << endl;
		}
	}
};
#pragma once
