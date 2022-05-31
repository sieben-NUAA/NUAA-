#include<iostream>
#include<algorithm>
#include<vector>
using namespace std;

const int maxx = 2e5 + 10;
int parent[maxx];
struct arc
{
	int a;
	int b;
	int c;
}arc[maxx];

int cmp(struct arc a, struct arc b)
{
	return a.c < b.c;
}

int find_root(int x)
{
	if (parent[x] == x) return x;
	else return parent[x] = find_root(parent[x]);
}

void union_arc(int x, int y)
{
	int x_root = find_root(x);
	int y_root = find_root(y);
	parent[x_root] = y_root;
}

int main()
{
	int n, m, min,num=0;
	cin >> n >> m;
	for (int i = 0; i < m; i++)
		cin >> arc[i].a >> arc[i].b >> arc[i].c;
	sort(arc, arc + m, cmp);
	for (int i = 1; i <= n; i++)
		parent[i] = i;
	for (int i = 0; i < m; i++)
	{
		int x = arc[i].a;
		int y = arc[i].b;
		union_arc(x, y);
		//cout << find_root(1) << " " << find_root(n) << endl;
		if (find_root(1) == find_root(n))
		{
			cout << arc[i].c;
			break;
		}
	}
	return 0;
}