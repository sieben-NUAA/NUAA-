#include<iostream>
#include<fstream>
#include<string>
using namespace std;
struct informa
{
	int number;
	string info;
	int CheckCode;
	informa()
	{
		number = 0;
		info = "aaa";
		CheckCode = 0;
	}
};
typedef struct Block
{
	informa inf;
	Block* next;
}Block,*BlockChain;
int CalAscll(string s);
void ReadData(fstream& file, BlockChain& c);
void UpdataData(BlockChain c);
BlockChain Find(BlockChain c, int des);
void Is_Valid(BlockChain c);
void Insert(BlockChain& c);
void Revise(BlockChain& c);
void Print_All(BlockChain c);
#pragma once
