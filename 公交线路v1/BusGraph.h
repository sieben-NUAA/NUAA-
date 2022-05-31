#include<iostream>
#include<string.h>
#include<fstream>
#define STACKINITSIZE 256
#define STACKINCREMENT 128

struct Node_Arc //�洢��
{
	int adjvex;//�ߵ����վ��
	int road;//����·��
	Node_Arc* Next_Arc;//��һ����
};

struct Bus_Station//�洢����վ̨
{
	char* name=new char[50];
	Node_Arc* First_Arc;//�洢��һ����
};

typedef struct line_sta//��¼��i·����������վ��
{
	char* name = new char[50];
	line_sta* next;
	line_sta* pre;
}line_sta;

typedef struct BusGraph
{
	Bus_Station* Station=new Bus_Station[5200];
	line_sta* lu[1000];
	int vexnum, arcnum;
	BusGraph()
	{
		vexnum = 0; arcnum = 0;
		for (int i = 0; i < 1000; i++) lu[i] = NULL;
	}
}BusGraph;

typedef struct Node_Line//����վ·�ߵ��ڽ�
{
	Node_Arc* firstarc;
}Node_Line;

typedef struct Line//����̻��������
{
	Node_Line l[1000];
	int vexnum, arcnum;
	int* visit_lint;
}Line;

void Add_Arc(BusGraph& Graph, int station_a, int station_b, int road);
BusGraph Init_BusGraph();
int Find_Station(BusGraph G, char* des);
void Destroy_BusGraph(BusGraph& Graph);
bool Is_Adj(BusGraph G,int i, int j);
void Less_Station(BusGraph Graph, char* begin, char* end);

void Create_arc(int i, int j, Line& r);
void InitLine(BusGraph G, Line& r);
void Destory_Line(Line& r);
bool Is_Adj(Line r, int i, int j);
void Print_Min_Change(BusGraph G, Line r, int i, int j, char* start, char* end);
void Min_Change(Line r, BusGraph G, char* beg, char* en);
#pragma once
