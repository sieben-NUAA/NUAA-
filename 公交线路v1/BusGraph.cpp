#pragma warning(disable : 4996)
#include<iostream>
#include<string.h>
#include<fstream>
#include"BusGraph.h"
#include"Queue.h"
#include"Stack.h"
using namespace std;
int* visit;

//��ӱ�
void Add_Arc(BusGraph& Graph, int station_a, int station_b, int road)
{
	Node_Arc* Arc;
	Arc = Graph.Station[station_a].First_Arc;
	if (Arc == NULL)//���û���ڽӵ�
	{
		Graph.Station[station_a].First_Arc = new Node_Arc;
		Graph.Station[station_a].First_Arc->adjvex = station_b;
		Graph.Station[station_a].First_Arc->road = road;
		Graph.Station[station_a].First_Arc->Next_Arc = NULL;
	}
	else
	{
		while (Arc->Next_Arc != NULL) Arc = Arc->Next_Arc;
		Arc->Next_Arc = new Node_Arc;
		Arc = Arc->Next_Arc;
		Arc->adjvex = station_b;
		Arc->Next_Arc = NULL;
		Arc->road = road;
	}

	//����ͼ������
	Arc = Graph.Station[station_b].First_Arc;
	if (Arc == NULL)
	{
		Graph.Station[station_b].First_Arc = new Node_Arc;
		Graph.Station[station_b].First_Arc->adjvex = station_a;
		Graph.Station[station_b].First_Arc->road = road;
		Graph.Station[station_b].First_Arc->Next_Arc = NULL;
	}
	else
	{
		while (Arc->Next_Arc != NULL) Arc = Arc->Next_Arc;
		Arc->Next_Arc = new Node_Arc;
		Arc = Arc->Next_Arc;
		Arc->adjvex = station_a;
		Arc->Next_Arc = NULL;
		Arc->road = road;
	}
	Graph.arcnum++;
}

//��ʼ��ͼ�����ļ�
BusGraph Init_BusGraph()
{
	BusGraph p;
	p.arcnum = 0;
	p.vexnum = 0;

	fstream datafile;
	datafile.open("�Ͼ�������·.txt", ios::in);
	if (datafile.fail())
	{
		cout << "�ļ���ʧ��!" << endl;
		exit(0);
	}

	char str[3000];
	int road = 0;
	int count = 0;
	int pre_p;
	int n = 0;
	
	while (!datafile.eof())
	{
		datafile >> road;
		datafile.getline(str, 3000, '\n');
		int i = 5;
		//��ȡһ·������������
		while (i < strlen(str))
		{
			int j;
			line_sta* tp = new line_sta;
			//��¼��վ�������
			for (j = 0; str[i] != ',' && i < strlen(str); i++, j++)
			{
				p.Station[p.vexnum].name[j] = str[i];
				tp->name[j] = str[i];
			}
			p.Station[p.vexnum].name[j] = '\0';
			tp->name[j] = '\0';
			tp->next = NULL;
			line_sta* pz = p.lu[road];
			if (pz == NULL)
			{
				p.lu[road] = tp;
				tp->pre = NULL;
			}
			else
			{
				while (pz->next != NULL) pz = pz->next;
				tp->pre = pz;
				pz->next = tp;
			}

			//��վ���Ѿ���ͼ��
			for (j = 0; j < p.vexnum; j++)
			{
				if (strcmp(p.Station[j].name, p.Station[p.vexnum].name) == 0)
				{
					if (count > 0)
					{
						Add_Arc(p, j, pre_p, road);
					}
					pre_p = j;
					count++;
					break;
				}
			}//end for (j = 0; j < p.vexnum; j++)

			//վ�㲻��ͼ��
			if (j == p.vexnum)
			{
				p.Station[p.vexnum].First_Arc = NULL;
				if (count > 0) Add_Arc(p, pre_p, p.vexnum, road);

				pre_p = p.vexnum;
				p.vexnum++;
				count++;
			}// end if(j==p.vexnum)
			if (i < strlen(str)) i++;
			else break;
		}//end while (i < strlen(str))
		pre_p = 0;
		count = 0;
	}//end while (!datafile.eof())
	datafile.close();
	visit = new int[p.vexnum];
	return p;
}

//����ͼ
void Destroy_BusGraph(BusGraph& Graph)
{
	Node_Arc* Arc, * Pre_Arc;
	for (int i = 0; i < Graph.vexnum; i++)
	{
		delete Graph.Station[i].name;
		Arc = Graph.Station[i].First_Arc;
		if (Arc != NULL)
		{
			Pre_Arc = Arc;
			while (1)
			{
				Arc = Arc->Next_Arc;
				if (Arc == NULL) break;
				delete Pre_Arc;
				Pre_Arc = Arc;
			}
		}
	}
	delete visit;
}

//Ѱ��վ�ı��
int Find_Station(BusGraph G, char* des)
{
	for (int i = 0; i < G.vexnum; i++)
	{
		if (strcmp(G.Station[i].name, des) == 0)
			return i;
	}
	return -1;
}

//�ж��Ƿ�����
bool Is_Adj(BusGraph G,int i, int j)
{
	Node_Arc* p = G.Station[i].First_Arc;
	if (p == NULL) return false;
	else
	{
		while (p != NULL)
		{
			if (p->adjvex == j)
				return true;
			p = p->Next_Arc;
		}
	}
	return false;
}

//����վ�����ٵģ��������
void Less_Station(BusGraph Graph, char* begin, char* end)
{
	int sta = 0, en = 0;//sta��¼���ı�ţ�en��¼�յ�ı��
	for (int i = 0; i < Graph.vexnum; i++) visit[i] = 0;
	sta = Find_Station(Graph, begin);
	en = Find_Station(Graph, end);
	
	if (sta == -1 || en == -1)
	{
		cout << "�������վ��" << endl;
		exit(0);
	}

	LinkQueue q;
	InitQueue(q);
	int j = sta;//��ʼ��
	visit[j] = 1;
	EnQueue(q, j);//��ʼ�����
	int flag = 0;
	Node_Arc* Arc = NULL;
	while (!QueueEmpty(q))
	{
		if (flag) break;
		DeQueue(q, j);
		for (Arc = Graph.Station[j].First_Arc; Arc != NULL; Arc = Arc->Next_Arc)
		{
			if (visit[Arc->adjvex] == 0)
			{
				visit[Arc->adjvex] = visit[j] + 1;//��¼���ʲ�����˶���վ
				if (Arc->adjvex == en)//�����յ����˳�
				{
					flag = 1;
					break;
				}
				EnQueue(q, Arc->adjvex);
			}
		}
	}
	
	Node_Arc* a;
	SeqStack s_station;
	
	InitStack(s_station);
	
	Push(s_station, Arc->adjvex);
	int i = Arc->adjvex;
	cout << "����վ�����ٵķ�����" << endl;
	cout << "һ��:" << visit[Arc->adjvex] << "վ" << endl;
	for (int deep = visit[Arc->adjvex] - 1; deep > 1; deep--)//deep�ǹ�ȵ�Ȧ��
	{
		for (j = 0; j < Graph.vexnum; j++)
		{
			if (visit[j] == deep && Is_Adj(Graph, i, j))//�������һȦ���뱾վ�����ڵĿ�����ջ
			{
				Push(s_station, j);
				i = j;
				break;
			}
		}
	}

	i = sta;
	a = Graph.Station[j].First_Arc;
	while (a->adjvex != i)
		a = a->Next_Arc;
	cout << Graph.Station[sta].name << "(" << a->road << "·��)";
	
	while (!StackEmpty(s_station))
	{
		cout << "->";
		Pop(s_station, j);
		a = Graph.Station[j].First_Arc;
		while (a->adjvex != i)
			a = a->Next_Arc;
		
		cout << Graph.Station[j].name<<"("<<a->road<<"·��)";
		i = j;
	}
	cout << endl;

}

//���
void Create_arc(int i, int j, Line& r)
{
	int flag;
	Node_Arc* p;
	p = r.l[i].firstarc;
	if (p == NULL)							//��һ���գ��½�һ��
	{
		r.l[i].firstarc = new Node_Arc;
		r.l[i].firstarc->adjvex = j;
		r.l[i].firstarc->Next_Arc = NULL;
	}
	else
	{
		flag = 0;
		while (p->Next_Arc != NULL)			//Ѱ����û���ظ���
		{
			if (p->adjvex == j) flag = 1;
			p = p->Next_Arc;
			if (p->adjvex == j) flag = 1;
		}
		if (!flag)							//û���ظ��ľ��¼�һ��
		{
			p->Next_Arc = new Node_Arc;
			p = p->Next_Arc;
			p->adjvex = j;
			p->Next_Arc = NULL;
		}
	}
	p = r.l[j].firstarc;
	if (p == NULL)							//��һ���գ��½�һ��
	{
		r.l[j].firstarc = new Node_Arc;
		r.l[j].firstarc->adjvex = i;
		r.l[j].firstarc->Next_Arc = NULL;
	}
	else
	{
		flag = 0;
		while (p->Next_Arc != NULL)			//Ѱ����û���ظ���
		{
			if (p->adjvex == i) flag = 1;
			p = p->Next_Arc;
			if (p->adjvex == i) flag = 1;
		}
		if (!flag)							//û���ظ��ľ��¼�һ��
		{
			p->Next_Arc = new Node_Arc;
			p = p->Next_Arc;
			p->adjvex = i;
			p->Next_Arc = NULL;
		}
	}
	r.arcnum++;
}

//��·��ʼ��
void InitLine(BusGraph G, Line& r)
{
	r.vexnum = 0;
	Node_Arc* arc;
	int i, j, k;
	int count;
	int Car_bs_all_bus[500];

	for (int kk = 0; kk < 1000; kk++) r.l[kk].firstarc = NULL;

	for (i = 0; i < G.vexnum; i++)								//�Ѿ���ͬһվ���·������
	{
		arc = G.Station[i].First_Arc;
		for (j = 0; j < 500; j++) Car_bs_all_bus[j] = 0;
		if (arc == NULL) continue;
		count = 0;
		while (arc != NULL)
		{
			for (k = 0; k < count; k++)							//��һ��·���Ƿ��Ѿ����
				if (arc->road == Car_bs_all_bus[k]) break;
			if (k == count)										//û�����·�ߣ��¼���
			{
				Car_bs_all_bus[count++] = arc->road;
				if (arc->road > r.vexnum)r.vexnum = arc->road;
			}
			arc = arc->Next_Arc;
		}
		for (k = 0; k < count - 1; k++)
			for (j = k + 1; j < count; j++)
				if (Car_bs_all_bus[k] != Car_bs_all_bus[j])		//����ͬһ·�ߣ�������
					Create_arc(Car_bs_all_bus[j], Car_bs_all_bus[k], r);
	}
	r.visit_lint = new int[r.vexnum + 1];
}

//������·
void Destory_Line(Line& r)
{
	Node_Arc* p, * prep;
	for (int i = 1; i <= r.vexnum; i++)
	{
		p = r.l[i].firstarc;
		if (p != NULL)
		{
			prep = p;
			while (1)
			{
				p = p->Next_Arc;
				if (p == NULL)break;
				delete prep;
				prep = p;
			}
		}
	}
}

//�ж�����
bool Is_Adj(Line r, int i, int j)
{
	Node_Arc* p = r.l[i].firstarc;
	if (p == NULL) return false;
	else
	{
		while (p != NULL)
		{
			if (p->adjvex == j)
				return true;
			p = p->Next_Arc;
		}
	}
	return false;
}

//��ȡ��С������,��������㷨
int Less_Change(Line r, int i, int j)
{
	if (i == j)													//ͬһվ
	{
		return 0;
	}
	for (int k = 1; k < r.vexnum + 1; k++)
		r.visit_lint[k] = 0;
	int v, u;
	Node_Arc* p= NULL;
	int flag = 0;
	LinkQueue q;
	InitQueue(q);
	v = i;
	r.visit_lint[v] = 1;
	EnQueue(q, v);
	while (!QueueEmpty(q))										//������ȱ���
	{
		if (flag) break;
		DeQueue(q, u);
		for (p = r.l[u].firstarc; p != NULL; p = p->Next_Arc)
			if (r.visit_lint[p->adjvex] == 0)
			{
				r.visit_lint[p->adjvex] = r.visit_lint[u] + 1;
				if (p->adjvex == j)
				{
					flag = 1;
					break;
				}
				EnQueue(q, p->adjvex);
			}
	}
	return r.visit_lint[p->adjvex];
}

//���������С����
void Print_Min_Change(BusGraph G,Line r, int i, int j, char* start, char* end)
{
	line_sta* sta = G.lu[i];
	if (i == j)
	{
		cout << "��վλ��" << i << "·����" << endl;
		
		while (strcmp(sta->name, start) != 0)
			sta = sta->next;
		while (strcmp(sta->name, end) != 0)
		{
			cout << sta->name << "("<<i<<"·)->";
			sta = sta->next;
		}
		cout << end;
		return;
	}

	for (int k = 1; k < r.vexnum + 1; k++) r.visit_lint[k] = 0;

	//������ȣ�������·��i��·��j����̻���
	int v, u;
	Node_Arc* p = NULL;
	int flag = 0;
	LinkQueue q;
	InitQueue(q);
	v = i;
	r.visit_lint[v] = 1;
	EnQueue(q, v);
	while (!QueueEmpty(q))
	{
		if (flag) break;
		DeQueue(q, u);
		for(p=r.l[u].firstarc;p!=NULL;p=p->Next_Arc)
			if (r.visit_lint[p->adjvex] == 0)
			{
				r.visit_lint[p->adjvex] = r.visit_lint[u] + 1;
				if (p->adjvex == j)
				{
					flag = 1;
					break;
				}
				EnQueue(q, p->adjvex);
			}
	}

	Node_Arc* t;
	SeqStack s;
	InitStack(s);
	Push(s,p->adjvex);
	int m = p->adjvex;
	cout << "ת�����ٷ���:\n";
	if (r.visit_lint[p->adjvex] <= 1)
		cout << "����ת��\n";
	else
		cout << "һ��ת����:" << r.visit_lint[p->adjvex] - 1 << "��" << endl;
	for(int deep = r.visit_lint[p->adjvex]-1;deep>1;deep--)
		for(j=0;j<r.vexnum;j++)
			if (r.visit_lint[j] == deep && Is_Adj(r, m, j))
			{
				Push(s, j);
				m = j;
				break;
			}
	cout << start<<"("<<i<<"·��)->";
	
	m = i;													//mΪ��վ��·��
	char* st = new char[50];
	strcpy(st, start);
	int temp;
	while (!StackEmpty(s))
	{
		int flag = 0;
													//temp�Ǻ�һվ��·��
		Pop(s, temp);

		int pos = Find_Station(G, st);
		
		Node_Arc* tp = G.Station[pos].First_Arc;
		int sum = -1;
		int p[100];
		int sum_p = 0;
		p[0] = pos; sum_p++;
		line_sta* te = G.lu[m];
		int adj;
		while (te != NULL)
		{
			//cout << te->name << endl;
			int op = Find_Station(G, te->name);
			Node_Arc* opt = G.Station[op].First_Arc;
			while (opt != NULL && opt->road!=temp)
			{
				opt = opt->Next_Arc;
			}
			if (opt == NULL)te = te->next;
			else { adj = opt->adjvex; break; }
		}
		cout << G.Station[adj].name << "(" << temp << "·��)->";
		m = temp;
	}
	cout <<end<<"("<<temp<<"·��)" << endl;
}

//��ÿһ����������
void Min_Change(Line r, BusGraph G, char* beg, char* en)
{
	int begin = Find_Station(G, beg), end = Find_Station(G, en);
	for (int i = 1; i < r.vexnum + 1; i++)	r.visit_lint[i] = 0;

	Node_Arc* p;

	int s_road[100],e_road[100],s_count=0,e_count=0;//s_road[] �������Գ˵Ĺ���·ȫ��
	char s[50], e[50];

	p = G.Station[begin].First_Arc;
	while (p != NULL)
	{
		if(s_count != 0 && s_road[s_count-1]!=p->road )//����ͬһ��·��Ҳ����������(���)
			s_road[s_count++] = p->road;
		if (s_count == 0)
			s_road[s_count++] = p->road;
		p = p->Next_Arc;
	}

	p = G.Station[end].First_Arc;
	while (p != NULL)
	{
		if (e_count != 0 && e_road[e_count - 1] != p->road)
			e_road[e_count++] = p->road;
		if (e_count == 0)
			e_road[e_count++] = p->road;
		p = p->Next_Arc;
	}


	int min = 999999;
	int tmp = 0;
	int min_s = 0;
	int min_e = 0;
	int sign = 0;											//��¼��û�о���ͬһվ
	for (int i = 0; i < s_count; i++)
	{
		sign = 0;
		for (int j = 0; j < e_count; j++)
		{
			if (s_road[i] == e_road[j])						//��վ��λ��ͬһ����·
			{
				sign = 1;
				min = 1;
				min_s = min_e = s_road[i];
				break;
			}
			tmp = Less_Change(r, s_road[i], e_road[j]);
			if (tmp < min)
			{
				min = tmp;
				min_s = s_road[i];
				min_e = e_road[j];
			}
		}
		if (sign)break;
	}
	
	Print_Min_Change(G, r, min_s, min_e, beg, en);
}