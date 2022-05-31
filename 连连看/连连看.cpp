#include<iostream>
#include<vector>
#include<Windows.h>
#include<algorithm>
#include<iomanip>
#include<ctime>
using namespace std;
int map[12][22];

//随机创建地图
void Get_New_Map()
{
	int a[200];
	int num[10] = { 0 };
	int sum[10];
	int pos;
	int pattern;
	srand(time(NULL));
	
	for (int i = 0; i < 200; i++) a[i] = 1;
	for (int i = 0; i < 12; i++)
		for (int j = 0; j < 22; j++) map[i][j] = -1;
	for (int i = 0; i < 100; i++)
	{
		pos = rand() % 200;//随机生成位置，前两位是第几列，个位是第几行
		
		while (a[pos] == 0)//如果这个位置没有放过了数据，那么重新生成
			pos = rand() % 200;
		pattern = rand() % 10;//随机生成类型
		a[pos] = 0;
		
		int x = pos % 10+1, y = pos / 10+1;//计算第几行第几列
		map[x][y] = pattern;//将点的类型赋值给图

		//由于需要是偶数个点，一个类型要生成两个位置
		pos = rand() % 200;
		
		while (a[pos] == 0)
			pos = rand() % 200;
		
		x = pos % 10 + 1; y = pos / 10 + 1;
		map[x][y] = pattern;
		a[pos] = 0;
	}
}

//打印图
void Print()
{
	cout << "    ";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
	for (int i = 0; i < 20; i++)
		cout  << setw(4) << std::left << i+1;
	cout << endl;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	for (int i = 1; i <= 10; i++)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
		cout << setw(4) << std::left << i;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		for (int j = 1; j <= 20; j++)
		{
			if (map[i][j] != -1)
				cout << setw(4) << std::left << map[i][j];
			else cout << "    ";
		}
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
		cout << setw(4) << std::left << i;
		cout << endl;
	}
	cout << "    ";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
	for (int i = 0; i < 20; i++)
		cout << setw(4) << std::left << i + 1;
	cout << endl;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}

//判断行相等时能否消去
bool Row_Equal(int y1, int y2, int x1,int x2)
{
	if (x1 != x2) return false;
	//cout << "行相等" << endl;
	if (y1 == y2 - 1) return true;
	//cout << "非相邻" << endl;
	int sign = 0;
	//判断能否从左到右一条直线连通
	for (int i = y1 + 1; i < y2; i++)
	{
		if (map[x1][i] != -1)
		{

			return false;//sign = 1;
		}
	}
	return true;
}

//判断列相等时能否消去
bool Col_Equal(int x1, int x2, int y1,int y2)
{
	if (y1 != y2) return false;
	if (x2 - x1 == 1) return true;
	int sign = 0;
	for (int i = x1 + 1; i < x2; i++)
	{
		//cout << "map[" << i << "][" << y1 << "]=" << map[i][y1] << endl;
		if (map[i][y1] != -1)
		{
			return false;//sign = 1;
		}
	}
	return true;
}

//不转折
bool Is_Zero_Turn(int x1, int x2, int y1, int y2)
{
	if (Row_Equal(min(y1, y2),max(y1,y2), x1, x2))	return true;
	if (Col_Equal(min(x1,x2),max(x1, x2), y1, y2))return true;
	return false;
}

//转折一次
bool Is_One_Turn(int x1, int x2, int y1, int y2)
{
	if (Is_Zero_Turn(x1, x1, y2, y1) && Is_Zero_Turn(x1, x2, y2, y2)) return true;
	if (Is_Zero_Turn(x2, x1, y1, y1) && Is_Zero_Turn(x2, x2, y1, y2)) return true;
	return false;
}

//转折两次
bool Is_Two_Turn(int x1, int y1, int x2, int y2)
{
	//向上找
	int sign = 0;
	for (int i = x1 - 1; i >= 0; i--)
	{
		if (map[i][y1] != -1) break;
		if (Is_One_Turn(i, x2, y1, y2)) return true;
	}

	//向下找
	for (int i = x1 + 1; i <= 11; i++)
	{
		if (map[i][y1] != -1)break;
		if (Is_One_Turn(i, x2, y1, y2)) return true;
	}

	//向左找
	for (int i = y1 - 1; i >= 0; i--)
	{
		if (map[x1][i] != -1) break;
		if (Is_One_Turn(x1, x2, i, y2))return true;
	}

	//向右找
	for (int i = y1 + 1; i <= 21; i++)
	{
		if (map[x1][i] != -1) break;
		if (Is_One_Turn(x1, x2, i, y2)) return true;
	}
	return false;
}

//判断能否消去
void Erase(int x1,int y1,int x2,int y2)
{
	//不进行转折
	if (Is_Zero_Turn(x1,x2,y1,y2))
	{
		cout << "能够消去" << endl;
		map[x1][y1] = -1; map[x2][y2] = -1;
		return;
	}
	//进行一次转折
	if (Is_One_Turn(x1, x2, y1, y2))
	{
		cout << "能够消去" << endl;
		map[x1][y1] = -1; map[x2][y2] = -1;
		return;
	}
	//进行两次转折
	if (Is_Two_Turn(x1, y1, x2, y2))
	{
		cout << "可以消去" << endl;
		map[x1][y1] = -1; map[x2][y2] = -1;
		return;
	}
	cout << "无法消去！" << endl;
}

//判断是否完成
bool Is_Finished()
{
	for (int i = 0; i < 12; i++)
		for (int j = 0; j < 22; j++)
			if (map[i][j] != -1)return false;
	return true;
}

int main()
{
	Get_New_Map();
	Print();
	int x1, x2, y1, y2;

	while (!Is_Finished())
	{
		system("cls");
		Print();
		//cout << map[0][0] << endl;
		cout << "请输入第一个图案的坐标:";
		cin >> x1 >> y1;
		cout << "请输入第二个图案的坐标:";
		cin >> x2 >> y2;

		if (x1 == x2 && y1 == y2) { cout << "请不要输入一样的坐标!\n" << endl; system("pause"); continue; }
		if(map[x1][y1]!=map[x2][y2]) { cout << "图案不一样，无法消除!\n" << endl; system("pause"); continue; }
		if (x1 < 1 || x1>10 || y1 < 1 || y1>20 || x2 < 1 || x2>10 || y1 < 1 || y2>20) { cout << "请输入有效坐标!\n" << endl; system("pause"); continue; }
		Erase(x1, y1, x2, y2);
		system("pause");
	}

	cout << "You Win!!!!!";

	return 0;
}