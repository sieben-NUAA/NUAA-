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
	int keynum;					//�ؼ��ָ����������Ĵ�С
	struct BTNode* parent;		//ָ��˫�׽��
	KeyType key[M + 1];			//�ؼ���������0�ŵ�ַδ��
	struct BTNode* ptr[M + 1];	//����ָ������
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

//B-���Ĳ��ҽ������
typedef struct {
	BTNode* pt;			//ָ���ҵ��Ľ��
	int i;				//1....M���ڽ���еĹؼ������
	int tag;			//1:���ҳɹ� 0:����ʧ��
}Result;

//��p->key[1...keynun]�в���i ʹ�� p->key[i-1]<K<=p->key[i]
int Search(BTree p, int K)
{
	int i = 1;
	while (i <= p->keynum && K > p->key[i]) i++;
	return i;
}

//������T�йؼ���K��λ��
Result SearchBTree(BTree T, KeyType K)
{
	BTree p = T,q = NULL;									//pΪ����Ľ�㣬qΪ��˫�׽��
	bool found = false;
	int i = 0;
	while (p && !found)
	{
		i = Search(p, K);
		if (i <= p->keynum && p->key[i] == K) found = true;
		else { q = p; p = p->ptr[i - 1]; }					//�������
	}
	if (found) return{ p, i, 1 };
	else return { q, i, 0 };								//���Ҳ��ɹ�������K�Ĳ���λ����Ϣ
}

//��x��ap�ֱ���뵽q->key[i+1]��q->ptr[i+1]
void Insert(BTree& q, int i, KeyType x, BTree ap)
{
	int j, n = q->keynum;
	for (j = n; j >= i; j--)			//����λ�ú�����ݺ���
	{
		q->key[j + 1] = q->key[j];
		q->ptr[j + 1] = q->ptr[j];
	}
	q->key[i] = x;
	q->ptr[i] = ap;
	if (ap != NULL) ap->parent = q;
	q->keynum++;
}

//����,��ǰһ������ԭ��㣬��һ������ap
void Split(BTree& q, int s, BTree& ap)
{
	int i, j, n = q->keynum;
	ap = new BTNode;
	ap->ptr[0] = q->ptr[s];
	for (i = s + 1, j = 1; i <= n; i++, j++) {			//����һ������ap�����
		ap->key[j] = q->key[i];
		ap->ptr[j] = q->ptr[i];
	}
	ap->keynum = n - s;									//�޸Ľ��ؼ��ֵ�����
	ap->parent = q->parent;
	for (i = 0; i <= n - s; i++)						//��һ����ĺ��ӵĸ��ڵ�ָ��ý��
		if (ap->ptr[i] != NULL) ap->ptr[i]->parent = ap;
	q->keynum = s - 1;									//sǰ�����
}

//�������
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

//����,���ؼ���K����q���ĵ�i��λ��
void InsertBTree(BTree& T, KeyType K, BTree q, int i)
{
	int x = K, s;
	BTree ap = NULL;
	bool finished = false;
	if (q == NULL)							//qΪ�գ���Ҫ���½���һ�������
	{ NewRoot(T, NULL, K, NULL); return; }
	while (q && !finished)
	{
		Insert(q, i, x, ap);
		if (q->keynum < M) finished = true; //������ɡ��ؼ��ָ���û�г���m-1
		else {								//���ѽ��
			s = (M + 1) / 2;				//�м�λ��
			Split(q, s, ap);				//���ѣ���q->key[s+1...m],q->ptr[s...m]�����½ڵ�ap
			x = q->key[s];					//xΪ�м�λ�ã�����֮������ϲ�˫�׽����
			
			if (q->parent != NULL)
			{
				q = q->parent;				
				i = Search(q, x);			//������˫�׽��������λ��
			}
			else break;						//��Ҫ�µĽ�㣬T�ǿ������߸����Ҳ���ѳ���q��ap
		}
	}
	if (!finished) NewRoot(T, q, x, ap);
}

//ɾ���ؼ���,��key[i]��ptr[i]ɾ��
void Remove(BTree& p, int i)
{
	for (int j = i; i < p->keynum; i++)
	{
		p->key[j] = p->key[j + 1];
		p->ptr[j] = p->ptr[j + 1];
	}
	p->keynum--;
}

//�ý��q�е�ptr[i]��ָ�����е���С�ؼ���������
void PassOver(BTree& q, int i)
{
	BTree child = q->ptr[i];
	while (child->ptr[0] != NULL) child = child->ptr[0];
	q->key[i] = child->key[1];
	q = child;
}

//�����������ˣ���Ҫ�ϲ�/��
void Adjust(BTree& T, BTree& p)
{
	BTree pa = p->parent;
	if (pa == NULL) 
	{				//�յĸ���㣬��Ҫɾ����������ȼ�һ
		T = p;
		p = p->parent;
		return;
	}
	BTree leftchild, rightchild, pr;//��¼p���������ֵ�
	int finished = 0, r = 0;		//r������¼p��λ����˫�׽����ĸ��±�
	while (!finished) 
	{
		r = 0;
		while (pa->ptr[r] != p) r++;										//ȷ��p��pa��λ��
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
		if (r > 0 && leftchild != NULL && (leftchild->keynum > (M - 1) / 2)) //�����ֵܽ�ֵ
		{
			p->keynum++;
			for (int i = p->keynum; i > 1; i--)								//�ؼ�������
			{
				p->key[i] = p->key[i - 1];
				p->ptr[i] = p->ptr[i - 1];
			}
			p->key[1] = pa->key[r];											//����������
			p->ptr[1] = p->ptr[0];
			p->ptr[0] = leftchild->ptr[leftchild->keynum];
			if (p->ptr[0] != NULL)											//�޸������ĸ����
				p->ptr[0]->parent = p;
			pa->key[r] = leftchild->key[leftchild->keynum];					//���ֵܵ�����
			leftchild->keynum--;
			finished = 1;
			break;
		}
		else if (pa->keynum > r && rightchild != NULL && (rightchild->keynum > (M - 1) / 2))//�����ֵܽ�
		{
			p->keynum++;
			p->key[p->keynum] = pa->key[r];						//���������
			p->ptr[p->keynum] = rightchild->ptr[0];
			if (p->ptr[p->keynum] != NULL)						//�޸������ĸ����
				p->ptr[p->keynum]->parent = p;
			pa->key[r] = rightchild->key[1];					//���ֵ�����
			rightchild->ptr[0] = rightchild->ptr[1];
			for (int i = 1; i < rightchild->keynum; i++)		//���ֵܽ��ؼ�������
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
		if (r > 0 && (pa->ptr[r - 1]->keynum <= (M - 1) / 2))//�����ֵܺϲ�
		{
			leftchild = pa->ptr[r - 1];
			p->keynum++;
			for (int i = p->keynum; i > 1; i--)				//��p���ؼ�������
			{
				p->key[i] = p->key[i - 1];
				p->ptr[i] = p->ptr[i - 1];
			}
			p->key[1] = pa->key[r];							//������������
			p->ptr[1] = p->ptr[0];							
			pa->ptr[r] = leftchild;
			for (int i = 1; i <= leftchild->keynum; i++)	//��p�Ĺؼ����Ƶ����ֵ���
			{
				leftchild->key[leftchild->keynum+i] = p->key[i];
				leftchild->ptr[leftchild->keynum + i] = p->ptr[i];
			}
			if (p->ptr[0])									//������˫�׽������
				for (int i = 1; i <= leftchild->keynum; i++)
					if (p->ptr[p->keynum + i])p->ptr[p->keynum + i]->parent = leftchild;
			leftchild->keynum += p->keynum;
			for (int i = r; i < pa->keynum; i++)			//��˫�׽��Ĺؼ�����λ
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
		else //�����ֵܺϲ�
		{
			rightchild = pa->ptr[r + 1];
			if (r == 0) r++;
			p->keynum++;
			p->key[p->keynum] = pa->key[r];								//�����Ĺؼ�����p�ϲ�
			p->ptr[p->keynum] = rightchild->ptr[0];						//�����ֵ�����һ��ָ��
			rightchild->keynum += p->keynum;
			pa->ptr[r - 1] = rightchild;
			for (int i = 1; i <= rightchild->keynum - p->keynum; i++)	//��p���ֵܹؼ�������
			{
				rightchild->key[p->keynum + i] = rightchild->key[i];
				rightchild->ptr[p->keynum + i] = rightchild->ptr[i];
			}
			for (int i = 1; i <= p->keynum; i++)						//��p�Ĺؼ��ֲ������ֵ�
			{
				rightchild->key[i] = p->key[i];
				rightchild->ptr[i] = p->ptr[i];
			}
			rightchild->ptr[0] = p->ptr[0];								//�޸�p������˫��Ϊ���ֵ�
			if (p->ptr[0])
				for (int i = 1; i <= p->keynum; i++)
					if (p->ptr[p->keynum + i]) p->ptr[p->keynum + i]->parent = rightchild;
			for (int i = r; i < pa->keynum; i++)						//��˫�׹ؼ�������
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
		else if (pa == NULL)											//�����գ�ɾ��
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

//ɾ��p�ڵ�ĵ�i���ؼ���
void DeleteBTree(BTree& T, BTree& p, int i)
{
	if (p->ptr[i] != NULL) {		//�������²�ĵ�
		PassOver(p, i);				//�ý��p�е�ptr[i]��������С�ؼ��ָ���key[i]
		DeleteBTree(T, p, 1);		//ת��Ϊɾ����ײ�ķ�֧�ڵ���ɾ��һ���ؼ���
	}
	else {							//�����²�Ľ��
		Remove(p, i);				
		if (p->keynum < (M - 1) / 2)//�ؼ��ָ���С��
			Adjust(T, p);			
	}
}

// ��ӡ���н����Ϣ
void ShowBtree(BTree& p)
{
	if (p == NULL) {
		puts("B-��δ����!");
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

// ������
void CreateBtree()
{
	T = new BTNode;
	T->keynum = 0;
}

// ����
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

/*�������溯��*/
//����ؼ���
void InsertKeytype()
{
	puts("��������������:");
	KeyType temp;
	scanf_s("%d", &temp);
	Result p;
	p=SearchBTree(T, temp);
	if (p.tag == 0) {
		InsertBTree(T, temp, p.pt, p.i);
		puts("����ɹ�");
		ShowBtree(T);
		puts("");
	}
	else puts("�����Ѵ���.");
}

// ��������
void FindKeytype()
{
	puts("��������ҵ�����:");
	KeyType temp;
	scanf_s("%d", &temp);
	Result p;
	p=SearchBTree(T, temp);
	if (p.tag) {
		puts("���ҳɹ�");
	}
	else puts("���ݲ�����!");
}

//ɾ��
void DeleteKeytype()
{
	cout << "��������Ҫɾ�������ݣ�";
	KeyType p;
	cin >> p;
	Result r;
	r = SearchBTree(T, p);
	if (r.tag)
	{
		DeleteBTree(T, r.pt, r.i);
		cout << "ɾ���ɹ�!";
		ShowBtree(T);
	}
	else cout << "���ݲ�����!";
}

int main()
{
	vector<int> testData;
	const char* filename = "btree.txt";

	// ��ȡ�ļ�
	ifstream infile(filename, ios::in);
	if (!infile.is_open()) { cout << "�ļ���ʧ�ܣ�\n"; return 0; }

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
		cout << "1. ��������\n";
		cout << "2. ��������\n";
		cout << "3. ɾ������\n";
		cout << "4. ��ʾȫ������\n";
		cout << "0. �˳�\n";
		cout << "��ѡ��(0-4): ";
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
