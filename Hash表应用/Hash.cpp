#include <iostream>
#include <string>
#include <windows.h>
#include <algorithm>
#include <fstream>
#include <vector>
#include"cilent.h"
using namespace std;

int main()
{
	vector<Client> ClientV;
	Read_File(ClientV);
	ConflictLink l[MAXH];
	cout << "1.����ַ\n2.���Ŷ�ַ\n3.VIP�û�(������ң�����ѡ��1)\n4.�˳�\n������ѡ��:";
	int cho;
	cin >> cho;
	ClientInfo Info[MAXN];
	while (1)
	{
		switch (cho)
		{
		case 1:
			Hash_Link(l, ClientV);
			break;
		case 2:
			Hash_Linear(Info, ClientV);
			break;
		case 3:
			Find(l); break;
		case 4:
			delete []Info;
			free(l);
			return 0;
		}
		cout << endl;
		system("pause");
		system("cls");
		cout << "1.����ַ\n2.���Ŷ�ַ\n3.VIP�û�(������ң�����ѡ��1)\n4.�˳�\n������ѡ��:";
		cin >> cho;
	}
	return 0;
}
