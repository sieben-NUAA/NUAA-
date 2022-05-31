#include<iostream>
#include<fstream>
#include<windows.h>
#include"Stack.h"
using namespace std;
#define N 50
int DATA[N][N];
int col, row;
int start_x, start_y, end_x, end_y;

//��ȡ�Թ�
void ReadFile(fstream& file, char* filename)
{
	int i = 1, j = 1;
	file.open(filename, ios::in);
	if (file.fail())
	{
		cout << "��ȡ�ļ�ʧ��!";
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

//Ѱ��·��
void GetPath()
{
	SeqStack s;
	InitStack(s);
	Point start = { start_x,start_y ,0};
	Point end = { end_x,end_y ,0};

	DATA[start.x][start.y] = 2;				//����Ϊ�ѷ���

	Point t;
	Push(s, start);
	while (!StackEmpty(s))
	{
		Pop(s, t);
		
		int adjacent = t.ap;
		if (t.x == end.x && t.y == end.y)	//���յ�
		{
			DestroyStack(s);
			return;
		}

		int find_flag = 0;
		int next_i, next_j;
		while (adjacent < 4 && find_flag == 0)				//Ѱ�ҿ��Է��ʵ�λ��
		{
			switch (adjacent)
			{
			case 0: {next_i = t.x - 1; next_j = t.y; break; }//��
			case 1: {next_i = t.x; next_j = t.y + 1; break; }//��
			case 2: {next_i = t.x + 1; next_j = t.y; break; }//��
			case 3: {next_i = t.x; next_j = t.y - 1; break; }//��
			}
			if (DATA[next_i][next_j] == 0 && next_i >= 1 && next_i <= row && next_j >= 1 && next_j <= col)//���û���粢�ҿ�����
				find_flag = 1;
			adjacent++;//��¼��һ�������ߵķ�λ
		}

		if (find_flag)//�ҵ������ߵķ���
		{
			t.ap = adjacent;
			Push(s, t);
			DATA[next_i][next_j] = 2;//�������ߵķ�λ���Ϊ2������ջ
			Point temp = { next_i,next_j,0 };
			Push(s, temp);
		}
		else
		{
			DATA[t.x][t.y] = 0;//�������ߣ�����Ϊ0
		}
	}
	DestroyStack(s);
}

int main()
{
	fstream file;
	char filename[20] = "Labyrinth.txt";
	ReadFile(file, filename);

	cout << "�������������:";
	cin >> start_x >> start_y;
	cout << "�������յ�����:";
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
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);//������ɫ���
				cout << DATA[i][j] << " ";
			}
		}
		printf("\n");
	}
	return 0;
}