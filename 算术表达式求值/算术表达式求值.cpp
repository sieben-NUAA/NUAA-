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
//��ȡ���ȼ�
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

//��������
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

//������ʽ
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
		if (isdigit(str[i]))//�����֣������ζ�ȡ���ݴ��������ջ
		{
			double num = 0;
			double x = 0.1;//�����С�����־��������
			int sign = 0;//��¼��û��С����
			while (isdigit(str[i]) || str[i] == '.')
			{
				if (isdigit(str[i]) && sign == 0)
				{
					num = num * 10 + str[i] - '0';
				}
				if (str[i] == '.') sign = 1;
				if (isdigit(str[i]) && sign)//�������Ҿ���С������
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
			case '<':OPTR.Push(str[i]); thetal=OPTR.GetTop(); i++; break;//ջ����������ȼ��ͣ���ǰ�����ѹջ
			case '>'://ջ����������ȼ��ߣ��������������������м��㣬ѹջ��������ջ
				theta = OPTR.Pop();
				b = OPND.Pop(); a = OPND.Pop();
				OPND.Push(Cal_Number(a, theta, b));
				break;
			case '='://�����ţ�������
				theta = OPTR.Pop(); i++; break;
			}
		}
		
		cout << "��" << step << "��:" << endl;
		cout << "    "; OPND.ShowStack(); cout << "    "; OPTR.ShowStack();
		cout << endl;
		if (OPTR.StackEmpty()) break;
	}
	double result;
	result=OPND.GetTop();
	cout << "��Ϊ" << result;
}

//ָ�����
void To_Error(string s,int pos)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
	for (int i = 0; i < s.length(); i++)
		if (i != pos) cout << " ";
		else cout << "��";
	cout << endl;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}

//�жϱ��ʽ�Ƿ���� 0:��β����#  1�����ڷǷ��ַ�  2��������ظ�  3�����Ŵ���  4��С������� 5������δ�ɶ� -1:��ȷ
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
		if ((s[i] == '+' || s[i] == '-' || s[i] == '*' || s[i] == '/') && (s[i + 1] == '+' || s[i + 1] == '-' || s[i + 1] == '*' || s[i + 1] == '/' || s[i + 1] == '.'))//��������滹�������
		{
			To_Error(s, i + 1);
			return 2;
		}
		if ((s[i] == '(' && !isdigit(s[i + 1])) || (s[i] == ')' && !isdigit(s[i - 1])))// �������ұ߲������֣���������߲�������
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
		case 0:cout << "��β����#" << endl; break;
		case 1:cout << "���ڷǷ��ַ�" << endl; break;
		case 2:cout << "������ظ�" << endl; break;
		case 3:cout << "���Ŵ���" << endl; break;
		case 4:cout << "С�������" << endl; break;
		case 5:cout << "����δ�ɶ�" << endl; break;
		case 6:cout << "���������" << endl; break;
		}
		
		cout << "\n���ʽ��������������:"<<endl;
		cin >> expre;
		pos = Is_Vaild(expre);
	}
	Cal_Expression(expre);
	return 0;
}