#include<iostream>
#include<string>
#include<stdlib.h>
#include<stdio.h>
#include <windows.h>
#include"Blockchain.h"
using namespace std;

int main()
{
	fstream datafile;
	BlockChain c=new Block;
	

	ReadData(datafile, c);
	while (1)
	{
		cout << "请输入需要进行的操作:" << endl;
		cout << "1、判断是否有效" << endl;
		cout << "2、添加信息" << endl;
		cout << "3、篡改信息" << endl;
		cout << "4、显示所有内容" << endl;
		cout << "5、退出并保存" << endl;
		int cho;
		cin >> cho;
		switch (cho)
		{
		case 1: {Is_Valid(c); system("pause"); system("cls"); break; }
		case 2: {Insert(c); system("pause"); system("cls"); break; }
		case 3: {Revise(c);  system("pause"); system("cls"); break; }
		case 4: {Print_All(c); system("pause"); system("cls"); break; }
		case 5: {UpdataData(c); return 0; }
		}
	}
	delete c;
	return 0;
}