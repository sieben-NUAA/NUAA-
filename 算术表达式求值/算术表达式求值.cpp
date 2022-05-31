#include<iostream>
#include<string>
#include<algorithm>
#include<vector>
#include"stack.h"
#include<Windows.h>
using namespace std;
//#6+15*(21-8/4)#
//#3+4*2-(1+1)#
//#9.1+(7+0.1)*2+78/50#
//获取优先级
char Get_Pririty(char a, char b)
{
	int i=-1, j=-1;
	char f[7][7] = {
		{'>','>','<','<','<','>','>'},
		{'>','>','<','<','<','>','>'},
		{'>','>','>','>','<','>','>'},
		{'>','>','>','>','<','>','>'},
		{'<','<','<','<','<','=','0'},
		{'>','>','>','>','0','>','>'},
		{'<','<','<','<','<','0','='}
	};
	switch (a)
	{
	case '+':i = 0; break;
	case '-':i = 1; break;
	case '*':i = 2; break;
	case '/':i = 3; break;
	case '(':i = 4; break;
	case ')':i = 5; break;
	case '#':i = 6; break;
	}
	switch (b)
	{
	case '+':j = 0; break;
	case '-':j = 1; break;
	case '*':j = 2; break;
	case '/':j = 3; break;
	case '(':j = 4; break;
	case ')':j = 5; break;
	case '#':j = 6; break;
	}
	if (i == -1 && j == -1) return 'a';
	return f[i][j];
}

//计算数字
double Cal_Number(double x, char op, double y)
{
	double q = 0;
	if (op == '+') q = x + y;
	if (op == '-') q = x - y;
	if (op == '*') q = x * y;
	if (op == '/') q = x / y;
	//cout << q << endl;
	return q;
}

//计算表达式
void Cal_Expression(string str)
{
	Stack<double> OPND;
	Stack<char> OPTR;
	int i = 0;
	OPTR.Push(str[i]);
	i++;
	char theta, thetal;
	thetal = OPTR.GetTop();
	int step = 0;
	int sign = 1;
	while (str[i] != '#' || thetal != '#')
	{
		step++;
		if (isdigit(str[i]))//是数字，就依次读取数据存入操作数栈
		{
			double num = 0;
			double x = 0.1;//如果有小数部分就用这个乘
			int sign = 0;//记录有没有小数点
			while (isdigit(str[i]) || str[i] == '.')
			{
				if (isdigit(str[i]) && sign == 0)
				{
					num = num * 10 + str[i] - '0';
				}
				if (str[i] == '.') sign = 1;
				if (isdigit(str[i]) && sign)//是数字且经过小数点了
				{
					num += (str[i] - '0') * x;
					x *= 0.1;
				}
				i++;
			}
			OPND.Push(num);
		}
		else
		{
			double a, b;
			thetal = OPTR.GetTop();
			cout << endl;
			cout <<thetal<< Get_Pririty(thetal, str[i]) <<str[i]<< " ";
			switch (Get_Pririty(thetal,str[i]))
			{
			case '<':OPTR.Push(str[i]); thetal=OPTR.GetTop(); i++; break;//栈顶运算符优先级低，则当前运算符压栈
			case '>'://栈顶运算符优先级高，则将运算数的两个数进行计算，压栈到运算数栈
				theta = OPTR.Pop();
				b = OPND.Pop(); a = OPND.Pop();
				OPND.Push(Cal_Number(a, theta, b));
				break;
			case '='://是括号，脱括号
				theta = OPTR.Pop(); i++; break;
			}
		}
		
		cout << "第" << step << "步:" << endl;
		cout << "    "; OPND.ShowStack(); cout << "    "; OPTR.ShowStack();
		cout << endl;
		if (OPTR.StackEmpty()) break;
	}
	double result;
	result=OPND.GetTop();
	cout << "答案为" << result;
}

//指向错误
void To_Error(string s,int pos)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
	for (int i = 0; i < s.length(); i++)
		if (i != pos) cout << " ";
		else cout << "↑";
	cout << endl;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}

//判断表达式是否合理 0:首尾不是#  1：存在非法字符  2：运算符重复  3：括号错误  4：小数点错误 5：括号未成对 -1:正确
int Is_Vaild(string s)
{
	int len = s.length();
	if (s[0] != '#' || s[len-1] != '#')
	{
		To_Error(s,0);
		return 0;
	}
	if (!isdigit(s[1])) { To_Error(s, 1); return 6; }
	int sign = 0;
	for (int i = 1; i < len - 1; i++)
	{
		if (!isdigit(s[i]) && s[i] != '+' && s[i] != '-' && s[i] != '*' && s[i] != '/' && s[i] != '(' && s[i] != ')' && s[i]!='.') { To_Error(s, i); return 1; }
		if ((s[i] == '+' || s[i] == '-' || s[i] == '*' || s[i] == '/') && (s[i + 1] == '+' || s[i + 1] == '-' || s[i + 1] == '*' || s[i + 1] == '/' || s[i + 1] == '.'))//运算符后面还是运算符
		{
			To_Error(s, i + 1);
			return 2;
		}
		if ((s[i] == '(' && !isdigit(s[i + 1])) || (s[i] == ')' && !isdigit(s[i - 1])))// 左括号右边不是数字，右括号左边不是数字
		{
			To_Error(s, i);
			return 3;
		}
		if (s[i] == '.' && !isdigit(s[i + 1])) { To_Error(s, i); return 4; }
	}

	Stack<int> q;
	int temp;
	for (int i = 0; i < len; i++)
	{
		if (s[i] == '(') q.Push(i);
		if (s[i] == ')') temp = q.Pop();
	}
	if (!q.StackEmpty()) { To_Error(s, q.GetTop()); return 5; }
	return -1;
}

int main()
{
	string expre;
	cin >> expre;
	int pos = Is_Vaild(expre);
	while (pos != -1)
	{
		switch (pos)
		{
		case 0:cout << "首尾不是#" << endl; break;
		case 1:cout << "存在非法字符" << endl; break;
		case 2:cout << "运算符重复" << endl; break;
		case 3:cout << "括号错误" << endl; break;
		case 4:cout << "小数点错误" << endl; break;
		case 5:cout << "括号未成对" << endl; break;
		case 6:cout << "运算符错误" << endl; break;
		}
		
		cout << "\n表达式错误，请重新输入:"<<endl;
		cin >> expre;
		pos = Is_Vaild(expre);
	}
	Cal_Expression(expre);
	return 0;
}