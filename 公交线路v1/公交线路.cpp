#include<iostream>
#include<string.h>
#include<fstream>
#include<stack>
#include<queue>
#include<math.h>
#include"BusGraph.h"
using namespace std;

const int inf = 0x3f3f3f3f;

int main()
{
	BusGraph Graph=Init_BusGraph();
	cout << "输入起点:";
	char Start[50];
	char End[50];
	cin >> Start;
	cout << "输入终点:";
	cin >> End;
	Less_Station(Graph, Start, End);

	Line r;
	InitLine(Graph, r);

	Min_Change(r, Graph, Start, End);
	Destory_Line(r);
	Destroy_BusGraph(Graph);
	return 0;
}