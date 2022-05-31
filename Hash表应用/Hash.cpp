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
	cout << "1.链地址\n2.开放定址\n3.VIP用户(如需查找，请先选择1)\n4.退出\n请输入选项:";
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
		cout << "1.链地址\n2.开放定址\n3.VIP用户(如需查找，请先选择1)\n4.退出\n请输入选项:";
		cin >> cho;
	}
	return 0;
}
