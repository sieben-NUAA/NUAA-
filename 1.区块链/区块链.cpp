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
		cout << "��������Ҫ���еĲ���:" << endl;
		cout << "1���ж��Ƿ���Ч" << endl;
		cout << "2�������Ϣ" << endl;
		cout << "3���۸���Ϣ" << endl;
		cout << "4����ʾ��������" << endl;
		cout << "5���˳�������" << endl;
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