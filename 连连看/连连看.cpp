#include<iostream>
#include<vector>
#include<Windows.h>
#include<algorithm>
#include<iomanip>
#include<ctime>
using namespace std;
int map[12][22];

//���������ͼ
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
		pos = rand() % 200;//�������λ�ã�ǰ��λ�ǵڼ��У���λ�ǵڼ���
		
		while (a[pos] == 0)//������λ��û�зŹ������ݣ���ô��������
			pos = rand() % 200;
		pattern = rand() % 10;//�����������
		a[pos] = 0;
		
		int x = pos % 10+1, y = pos / 10+1;//����ڼ��еڼ���
		map[x][y] = pattern;//��������͸�ֵ��ͼ

		//������Ҫ��ż�����㣬һ������Ҫ��������λ��
		pos = rand() % 200;
		
		while (a[pos] == 0)
			pos = rand() % 200;
		
		x = pos % 10 + 1; y = pos / 10 + 1;
		map[x][y] = pattern;
		a[pos] = 0;
	}
}

//��ӡͼ
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

//�ж������ʱ�ܷ���ȥ
bool Row_Equal(int y1, int y2, int x1,int x2)
{
	if (x1 != x2) return false;
	//cout << "�����" << endl;
	if (y1 == y2 - 1) return true;
	//cout << "������" << endl;
	int sign = 0;
	//�ж��ܷ������һ��ֱ����ͨ
	for (int i = y1 + 1; i < y2; i++)
	{
		if (map[x1][i] != -1)
		{

			return false;//sign = 1;
		}
	}
	return true;
}

//�ж������ʱ�ܷ���ȥ
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

//��ת��
bool Is_Zero_Turn(int x1, int x2, int y1, int y2)
{
	if (Row_Equal(min(y1, y2),max(y1,y2), x1, x2))	return true;
	if (Col_Equal(min(x1,x2),max(x1, x2), y1, y2))return true;
	return false;
}

//ת��һ��
bool Is_One_Turn(int x1, int x2, int y1, int y2)
{
	if (Is_Zero_Turn(x1, x1, y2, y1) && Is_Zero_Turn(x1, x2, y2, y2)) return true;
	if (Is_Zero_Turn(x2, x1, y1, y1) && Is_Zero_Turn(x2, x2, y1, y2)) return true;
	return false;
}

//ת������
bool Is_Two_Turn(int x1, int y1, int x2, int y2)
{
	//������
	int sign = 0;
	for (int i = x1 - 1; i >= 0; i--)
	{
		if (map[i][y1] != -1) break;
		if (Is_One_Turn(i, x2, y1, y2)) return true;
	}

	//������
	for (int i = x1 + 1; i <= 11; i++)
	{
		if (map[i][y1] != -1)break;
		if (Is_One_Turn(i, x2, y1, y2)) return true;
	}

	//������
	for (int i = y1 - 1; i >= 0; i--)
	{
		if (map[x1][i] != -1) break;
		if (Is_One_Turn(x1, x2, i, y2))return true;
	}

	//������
	for (int i = y1 + 1; i <= 21; i++)
	{
		if (map[x1][i] != -1) break;
		if (Is_One_Turn(x1, x2, i, y2)) return true;
	}
	return false;
}

//�ж��ܷ���ȥ
void Erase(int x1,int y1,int x2,int y2)
{
	//������ת��
	if (Is_Zero_Turn(x1,x2,y1,y2))
	{
		cout << "�ܹ���ȥ" << endl;
		map[x1][y1] = -1; map[x2][y2] = -1;
		return;
	}
	//����һ��ת��
	if (Is_One_Turn(x1, x2, y1, y2))
	{
		cout << "�ܹ���ȥ" << endl;
		map[x1][y1] = -1; map[x2][y2] = -1;
		return;
	}
	//��������ת��
	if (Is_Two_Turn(x1, y1, x2, y2))
	{
		cout << "������ȥ" << endl;
		map[x1][y1] = -1; map[x2][y2] = -1;
		return;
	}
	cout << "�޷���ȥ��" << endl;
}

//�ж��Ƿ����
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
		cout << "�������һ��ͼ��������:";
		cin >> x1 >> y1;
		cout << "������ڶ���ͼ��������:";
		cin >> x2 >> y2;

		if (x1 == x2 && y1 == y2) { cout << "�벻Ҫ����һ��������!\n" << endl; system("pause"); continue; }
		if(map[x1][y1]!=map[x2][y2]) { cout << "ͼ����һ�����޷�����!\n" << endl; system("pause"); continue; }
		if (x1 < 1 || x1>10 || y1 < 1 || y1>20 || x2 < 1 || x2>10 || y1 < 1 || y2>20) { cout << "��������Ч����!\n" << endl; system("pause"); continue; }
		Erase(x1, y1, x2, y2);
		system("pause");
	}

	cout << "You Win!!!!!";

	return 0;
}