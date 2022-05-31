#include<iostream>
#include<string>
#include<Windows.h>
#include<algorithm>
#include<fstream>
#include<vector>
using namespace std;
#define MAXH 35
#define MAXN 150

//�ͻ�
typedef struct Client {
	string id;   //���֤
	string name; //����
	string flight;  //�����
	string date; //��������
	double mile; //���
}Client;
 
//������Ϣ
typedef struct information {
	string flight;  //�����
	string date; //��������
	information* next;
}information;


//�����������˻�Ƶ�ʵĿͻ���Ϣ
class ClientInfo {
public:
	string id;
	string name;
	information inf;
	double Sum_Mile; //�������
	int Freq;        //�˻�Ƶ��
	static int Num_Used;
	int  reLocNum;   //�����ͻ���¶�λ�Ĵ���
	ClientInfo() {
		Freq = 0;
		Sum_Mile = 0;
		inf.flight = '\0';
		reLocNum = 0;
	}
};

//����ַ�����ͻ�Ľ��
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
