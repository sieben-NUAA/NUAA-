#include<iostream>
#include<string>
#include<vector>
#include<fstream>
#include<algorithm>
using namespace std;

#define KeyType int
#define Record string
#define M 3

typedef struct BTNode {
	int keynum;					//关键字个数，即结点的大小
	struct BTNode* parent;		//指向双亲结点
	KeyType key[M + 1];			//关键字向量，0号地址未用
	struct BTNode* ptr[M + 1];	//子树指针向量
	BTNode()
	{
		keynum = 0;
		parent = NULL;
		for (int i = 0; i < M + 1; i++) {
			ptr[i] = NULL;
		}
	}
}BTNode,* BTree;

BTree T = NULL;

//B-树的查找结果类型
typedef struct {
	BTNode* pt;			//指向找到的结点
	int i;				//1....M，在结点中的关键字序号
	int tag;			//1:查找成功 0:查找失败
}Result;

//在p->key[1...keynun]中查找i 使得 p->key[i-1]<K<=p->key[i]
int Search(BTree p, int K)
{
	int i = 1;
	while (i <= p->keynum && K > p->key[i]) i++;
	return i;
}

//查找树T中关键字K的位置
Result SearchBTree(BTree T, KeyType K)
{
	BTree p = T,q = NULL;									//p为插入的结点，q为其双亲结点
	bool found = false;
	int i = 0;
	while (p && !found)
	{
		i = Search(p, K);
		if (i <= p->keynum && p->key[i] == K) found = true;
		else { q = p; p = p->ptr[i - 1]; }					//结点下移
	}
	if (found) return{ p, i, 1 };
	else return { q, i, 0 };								//查找不成功，返回K的插入位置信息
}

//将x和ap分别插入到q->key[i+1]和q->ptr[i+1]
void Insert(BTree& q, int i, KeyType x, BTree ap)
{
	int j, n = q->keynum;
	for (j = n; j >= i; j--)			//插入位置后的数据后移
	{
		q->key[j + 1] = q->key[j];
		q->ptr[j + 1] = q->ptr[j];
	}
	q->key[i] = x;
	q->ptr[i] = ap;
	if (ap != NULL) ap->parent = q;
	q->keynum++;
}

//分裂,将前一半留在原结点，后一半移入ap
void Split(BTree& q, int s, BTree& ap)
{
	int i, j, n = q->keynum;
	ap = new BTNode;
	ap->ptr[0] = q->ptr[s];
	for (i = s + 1, j = 1; i <= n; i++, j++) {			//将后一半移入ap结点里
		ap->key[j] = q->key[i];
		ap->ptr[j] = q->ptr[i];
	}
	ap->keynum = n - s;									//修改结点关键字的数量
	ap->parent = q->parent;
	for (i = 0; i <= n - s; i++)						//后一半结点的孩子的父节点指向该结点
		if (ap->ptr[i] != NULL) ap->ptr[i]->parent = ap;
	q->keynum = s - 1;									//s前面的数
}

//创建结点
void NewRoot(BTree& T, BTree p, int x, BTree ap)
{
	T = new BTNode;
	T->keynum = 1;
	T->ptr[0] = p;
	T->ptr[1] = ap;
	T->key[1] = x;
	if (p) p->parent = T;
	if (ap) ap->parent = T;
	T->parent = NULL;
}

//插入,将关键字K插入q结点的第i个位置
void InsertBTree(BTree& T, KeyType K, BTree q, int i)
{
	int x = K, s;
	BTree ap = NULL;
	bool finished = false;
	if (q == NULL)							//q为空，需要重新建立一个根结点
	{ NewRoot(T, NULL, K, NULL); return; }
	while (q && !finished)
	{
		Insert(q, i, x, ap);
		if (q->keynum < M) finished = true; //插入完成。关键字个数没有超过m-1
		else {								//分裂结点
			s = (M + 1) / 2;				//中间位置
			Split(q, s, ap);				//分裂，将q->key[s+1...m],q->ptr[s...m]移入新节点ap
			x = q->key[s];					//x为中间位置，分裂之后插入上层双亲结点中
			
			if (q->parent != NULL)
			{
				q = q->parent;				
				i = Search(q, x);			//查找在双亲结点里插入的位置
			}
			else break;						//需要新的结点，T是空树或者根结点也分裂成了q和ap
		}
	}
	if (!finished) NewRoot(T, q, x, ap);
}

//删除关键字,将key[i]与ptr[i]删除
void Remove(BTree& p, int i)
{
	for (int j = i; i < p->keynum; i++)
	{
		p->key[j] = p->key[j + 1];
		p->ptr[j] = p->ptr[j + 1];
	}
	p->keynum--;
}

//用结点q中的ptr[i]所指子树中的最小关键字来覆盖
void PassOver(BTree& q, int i)
{
	BTree child = q->ptr[i];
	while (child->ptr[0] != NULL) child = child->ptr[0];
	q->key[i] = child->key[1];
	q = child;
}

//数据数量少了，需要合并/借
void Adjust(BTree& T, BTree& p)
{
	BTree pa = p->parent;
	if (pa == NULL) 
	{				//空的根结点，需要删除，树的深度减一
		T = p;
		p = p->parent;
		return;
	}
	BTree leftchild, rightchild, pr;//记录p结点的左右兄弟
	int finished = 0, r = 0;		//r用来记录p的位置是双亲结点的哪个下标
	while (!finished) 
	{
		r = 0;
		while (pa->ptr[r] != p) r++;										//确定p在pa的位置
		if (r == 0) 
		{
			r++;
			leftchild = NULL;
			rightchild = pa->ptr[r];
		}
		else if (r == pa->keynum) 
		{
			leftchild = pa->ptr[r - 1];
			rightchild = NULL;
		}
		else 
		{
			leftchild = pa->ptr[r - 1];
			rightchild = pa->ptr[r + 1];
		}
		if (r > 0 && leftchild != NULL && (leftchild->keynum > (M - 1) / 2)) //向左兄弟借值
		{
			p->keynum++;
			for (int i = p->keynum; i > 1; i--)								//关键字右移
			{
				p->key[i] = p->key[i - 1];
				p->ptr[i] = p->ptr[i - 1];
			}
			p->key[1] = pa->key[r];											//父结点的下移
			p->ptr[1] = p->ptr[0];
			p->ptr[0] = leftchild->ptr[leftchild->keynum];
			if (p->ptr[0] != NULL)											//修改子树的父结点
				p->ptr[0]->parent = p;
			pa->key[r] = leftchild->key[leftchild->keynum];					//左兄弟的上移
			leftchild->keynum--;
			finished = 1;
			break;
		}
		else if (pa->keynum > r && rightchild != NULL && (rightchild->keynum > (M - 1) / 2))//向右兄弟借
		{
			p->keynum++;
			p->key[p->keynum] = pa->key[r];						//父结点下移
			p->ptr[p->keynum] = rightchild->ptr[0];
			if (p->ptr[p->keynum] != NULL)						//修改子树的父结点
				p->ptr[p->keynum]->parent = p;
			pa->key[r] = rightchild->key[1];					//右兄弟上移
			rightchild->ptr[0] = rightchild->ptr[1];
			for (int i = 1; i < rightchild->keynum; i++)		//右兄弟结点关键字左移
			{
				rightchild->key[i] = rightchild->key[i + 1];
				rightchild->ptr[i] = rightchild->ptr[i + 1];
			}
			rightchild->keynum--;
			finished = 1;
			break;
		}
		r = 0;
		while (pa->ptr[r] != p) r++;
		if (r > 0 && (pa->ptr[r - 1]->keynum <= (M - 1) / 2))//与左兄弟合并
		{
			leftchild = pa->ptr[r - 1];
			p->keynum++;
			for (int i = p->keynum; i > 1; i--)				//将p结点关键字右移
			{
				p->key[i] = p->key[i - 1];
				p->ptr[i] = p->ptr[i - 1];
			}
			p->key[1] = pa->key[r];							//将父结点的下移
			p->ptr[1] = p->ptr[0];							
			pa->ptr[r] = leftchild;
			for (int i = 1; i <= leftchild->keynum; i++)	//将p的关键字移到左兄弟中
			{
				leftchild->key[leftchild->keynum+i] = p->key[i];
				leftchild->ptr[leftchild->keynum + i] = p->ptr[i];
			}
			if (p->ptr[0])									//将子树双亲结点设置
				for (int i = 1; i <= leftchild->keynum; i++)
					if (p->ptr[p->keynum + i])p->ptr[p->keynum + i]->parent = leftchild;
			leftchild->keynum += p->keynum;
			for (int i = r; i < pa->keynum; i++)			//将双亲结点的关键字移位
			{
				pa->key[i] = pa->key[i + 1];
				pa->ptr[i] = pa->ptr[i + 1];
			}
			pa->keynum--;
			pr = p;
			free(pr);
			pr = NULL;
			p = leftchild;
		}
		else //与右兄弟合并
		{
			rightchild = pa->ptr[r + 1];
			if (r == 0) r++;
			p->keynum++;
			p->key[p->keynum] = pa->key[r];								//父结点的关键字与p合并
			p->ptr[p->keynum] = rightchild->ptr[0];						//从右兄弟左移一个指针
			rightchild->keynum += p->keynum;
			pa->ptr[r - 1] = rightchild;
			for (int i = 1; i <= rightchild->keynum - p->keynum; i++)	//将p右兄弟关键字右移
			{
				rightchild->key[p->keynum + i] = rightchild->key[i];
				rightchild->ptr[p->keynum + i] = rightchild->ptr[i];
			}
			for (int i = 1; i <= p->keynum; i++)						//将p的关键字插入右兄弟
			{
				rightchild->key[i] = p->key[i];
				rightchild->ptr[i] = p->ptr[i];
			}
			rightchild->ptr[0] = p->ptr[0];								//修改p中子树双亲为右兄弟
			if (p->ptr[0])
				for (int i = 1; i <= p->keynum; i++)
					if (p->ptr[p->keynum + i]) p->ptr[p->keynum + i]->parent = rightchild;
			for (int i = r; i < pa->keynum; i++)						//将双亲关键字左移
			{
				pa->key[i] = pa->key[i + 1];
				pa->ptr[i] = pa->ptr[i + 1];
			}
			pa->keynum--;
			pr = p;
			free(pr);
			pr = NULL;
			p = rightchild;
		}
		pa = pa->parent;
		if (p->parent->keynum >= (M - 1) / 2 || (pa == NULL && p->parent->keynum > 0))
			finished = 1;
		else if (pa == NULL)											//根结点空，删除
		{
			pr = T;
			T = p;
			free(pr);
			pr = NULL;
			finished = 1;
		}
		p = p->parent;
	}

}

//删除p节点的第i个关键字
void DeleteBTree(BTree& T, BTree& p, int i)
{
	if (p->ptr[i] != NULL) {		//不是最下层的点
		PassOver(p, i);				//用结点p中的ptr[i]子树的最小关键字覆盖key[i]
		DeleteBTree(T, p, 1);		//转换为删除最底层的分支节点中删除一个关键字
	}
	else {							//是最下层的结点
		Remove(p, i);				
		if (p->keynum < (M - 1) / 2)//关键字个数小于
			Adjust(T, p);			
	}
}

// 打印树中结点信息
void ShowBtree(BTree& p)
{
	if (p == NULL) {
		puts("B-树未创建!");
		return;
	}
	bool have_child = false;
	printf("[");
	for (int i = 1; i <= p->keynum; i++) {
		if (i == 1);
		else cout<<" ";
		cout<<p->key[i];
	}
	printf("]");
	for (int i = 0; i <= p->keynum; i++) {
		if (p->ptr[i] != NULL) {
			if (i == 0) cout<<"<";
			else cout<<",";
			ShowBtree(p->ptr[i]);
			have_child = true;
		}
	}
	if (have_child) cout << ">";
}

// 创建树
void CreateBtree()
{
	T = new BTNode;
	T->keynum = 0;
}

// 销毁
void Destory(BTree& t)
{
	int i = 0;
	if (t != NULL) {
		while (i < t->keynum) {
			Destory(t->ptr[i]);
			free(t->ptr[i]);
			i++;
		}
	}
	free(t);
	t = NULL;
}

/*操作界面函数*/
//插入关键字
void InsertKeytype()
{
	puts("请输入插入的数据:");
	KeyType temp;
	scanf_s("%d", &temp);
	Result p;
	p=SearchBTree(T, temp);
	if (p.tag == 0) {
		InsertBTree(T, temp, p.pt, p.i);
		puts("插入成功");
		ShowBtree(T);
		puts("");
	}
	else puts("数据已存在.");
}

// 查找数据
void FindKeytype()
{
	puts("请输入查找的数据:");
	KeyType temp;
	scanf_s("%d", &temp);
	Result p;
	p=SearchBTree(T, temp);
	if (p.tag) {
		puts("查找成功");
	}
	else puts("数据不存在!");
}

//删除
void DeleteKeytype()
{
	cout << "请输入需要删除的数据：";
	KeyType p;
	cin >> p;
	Result r;
	r = SearchBTree(T, p);
	if (r.tag)
	{
		DeleteBTree(T, r.pt, r.i);
		cout << "删除成功!";
		ShowBtree(T);
	}
	else cout << "数据不存在!";
}

int main()
{
	vector<int> testData;
	const char* filename = "btree.txt";

	// 读取文件
	ifstream infile(filename, ios::in);
	if (!infile.is_open()) { cout << "文件打开失败！\n"; return 0; }

	while (!infile.eof())
	{
		int temp;
		infile >> temp;
		Result p;
		p = SearchBTree(T, temp);
		if (p.tag == 0) InsertBTree(T, temp, p.pt, p.i);
	}

	int cmd = 0;
	while (cmd != 7) {
		cout << "1. 插入数据\n";
		cout << "2. 查找数据\n";
		cout << "3. 删除数据\n";
		cout << "4. 显示全部数据\n";
		cout << "0. 退出\n";
		cout << "请选择(0-4): ";
		cin >> cmd;
		switch (cmd) {
		case 1: 
			InsertKeytype();
			break;
		case 2: 
			FindKeytype();
			break;
		case 3: 
			DeleteKeytype();
			break;
		case 4: 
			ShowBtree(T);
			//puts("");
			break;
		case 0: 
			return 0;
		}
		cout << endl;
		system("pause");
		system("cls");
	}

	Destory(T);
	return 0;
}
