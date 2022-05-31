#include<iostream>
#include<string>
#include<Windows.h>
#include<algorithm>
#include<fstream>
#include<vector>
using namespace std;
#define MAXH 35
#define MAXN 150

//客户
typedef struct Client {
	string id;   //身份证
	string name; //姓名
	string flight;  //航班号
	string date; //航班日期
	double mile; //里程
}Client;
 
//其他信息
typedef struct information {
	string flight;  //航班号
	string date; //航班日期
	information* next;
}information;


//存放里程数、乘机频率的客户信息
class ClientInfo {
public:
	string id;
	string name;
	information inf;
	double Sum_Mile; //里程总数
	int Freq;        //乘机频率
	static int Num_Used;
	int  reLocNum;   //解决冲突重新定位的次数
	ClientInfo() {
		Freq = 0;
		Sum_Mile = 0;
		inf.flight = '\0';
		reLocNum = 0;
	}
};

//链地址解决冲突的结点
typedef struct ConflictLink {
	string id;
	string name;
	information inf;
	int Freq;
	double Sum_Mile;
	struct ConflictLink* next;
	ConflictLink() {
		next = NULL;
		Freq = 0;
		Sum_Mile = 0;
		id = "\0";
		inf.date = '\0';
		inf.flight = '\0';
		inf.next = NULL;
	}
}ConflictLink;

void Read_File(vector<Client>& ClientV);
int Get_ID_Num(string s);
void Hash_Link(ConflictLink* c, vector<Client> ClientV);
void Hash_Linear(ClientInfo c_INFO[], vector<Client> ClientV);
void Find(ConflictLink c[]);
#pragma once
