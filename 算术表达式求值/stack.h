#include<iostream>
#include<string>
#include<stdlib.h>
#define STC_IN_LEN 100
#define ST_Cre 10
using namespace std;

// 栈 
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
	void ClearStack() {  // 清空 
		top = base;
	}
	bool StackEmpty() { // 判空 
		if (top == base) return true;
		else return false;
	}
	int StackLength() {  // 获取栈中元素个数 
		return top - base;
	}
	T GetTop() {  // 获取栈顶 
		return *(top - 1);
	}
	T* GetBaseLoca() { // 获取栈底 
		return base;
	}
	void Push(T num) { // 进栈 
		if (top - base != stacksize) {
			*(top) = num;
			top++;
		}
		else {  // 扩展空间 
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
	T Pop() { //出栈 
		if (top != base) {
			top--;
			return *(top);
		}
		else return '\0';
	}
	void ShowStack() {  // 打印栈中元素 
		int i;
		if (top - base == 0) cout << "空栈" << endl;
		else {
			cout << "栈底到栈顶：";
			for (i = 0; i < top - base; i++) {
				cout << data[i] << " ";
			}
			cout << endl;
		}
	}
};
#pragma once
