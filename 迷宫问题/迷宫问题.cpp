#include<iostream>
#include<fstream>
#include<windows.h>
#include"Stack.h"
using namespace std;
#define N 50
int DATA[N][N];
int col, row;
int start_x, start_y, end_x, end_y;

//读取迷宫
void ReadFile(fstream& file, char* filename)
{
	int i = 1, j = 1;
	file.open(filename, ios::in);
	if (file.fail())
	{
		cout << "读取文件失败!";
		exit(0);
	}

	file >> row >> col;
	while (!file.eof())
	{
		file >> DATA[i][j] ;
		if (j == col)
		{
			i++;
			j = 1;
		}
		else j++;
	}
	file.close();
}

//寻找路径
void GetPath()
{
	SeqStack s;
	InitStack(s);
	Point start = { start_x,start_y ,0};
	Point end = { end_x,end_y ,0};

	DATA[start.x][start.y] = 2;				//起点记为已访问

	Point t;
	Push(s, start);
	while (!StackEmpty(s))
	{
		Pop(s, t);
		
		int adjacent = t.ap;
		if (t.x == end.x && t.y == end.y)	//到终点
		{
			DestroyStack(s);
			return;
		}

		int find_flag = 0;
		int next_i, next_j;
		while (adjacent < 4 && find_flag == 0)				//寻找可以访问的位置
		{
			switch (adjacent)
			{
			case 0: {next_i = t.x - 1; next_j = t.y; break; }//上
			case 1: {next_i = t.x; next_j = t.y + 1; break; }//右
			case 2: {next_i = t.x + 1; next_j = t.y; break; }//下
			case 3: {next_i = t.x; next_j = t.y - 1; break; }//左
			}
			if (DATA[next_i][next_j] == 0 && next_i >= 1 && next_i <= row && next_j >= 1 && next_j <= col)//如果没出界并且可以走
				find_flag = 1;
			adjacent++;//记录下一个可以走的方位
		}

		if (find_flag)//找到可以走的方向
		{
			t.ap = adjacent;
			Push(s, t);
			DATA[next_i][next_j] = 2;//将可以走的方位标记为2，并入栈
			Point temp = { next_i,next_j,0 };
			Push(s, temp);
		}
		else
		{
			DATA[t.x][t.y] = 0;//不可以走，重置为0
		}
	}
	DestroyStack(s);
}

int main()
{
	fstream file;
	char filename[20] = "Labyrinth.txt";
	ReadFile(file, filename);

	cout << "请输入起点坐标:";
	cin >> start_x >> start_y;
	cout << "请输入终点坐标:";
	cin >> end_x >> end_y;

	cout << row << " " << col << endl;
	GetPath();
	for (int i = 1; i <= row; i++) {
		for (int j = 1; j <= col; j++) {
			if (DATA[i][j] == 2)
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
				cout << "0 ";
			}
			else
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);//设置三色相加
				cout << DATA[i][j] << " ";
			}
		}
		printf("\n");
	}
	return 0;
}