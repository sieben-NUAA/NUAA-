#include<iostream>
#include<algorithm>
#include<fstream>
#include<vector>
#include<string>
#include"cilent.h"
using namespace std;

int ClientInfo::Num_Used = 0;

// ����ת������
string UTF8ToGB(const char* str)
{
	string rst;
	WCHAR* strSrc;
	LPSTR szRes;

	// ��ȡ��ʱ������С
	int i = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
	strSrc = new WCHAR[i + 1];
	MultiByteToWideChar(CP_UTF8, 0, str, -1, strSrc, i);

	i = WideCharToMultiByte(CP_ACP, 0, strSrc, -1, NULL, 0, NULL, NULL);
	szRes = new CHAR[i + 1];
	WideCharToMultiByte(CP_ACP, 0, strSrc, -1, szRes, i, NULL, NULL);

	rst = szRes;
	delete[] strSrc;
	delete[] szRes;
	return rst;
}

//��ȡ�ļ�����
void Read_File(vector<Client>& ClientV)
{
	fstream datafile;
	Client cli;
	datafile.open("Hash.txt", ios::in);
	if (datafile.fail())
	{
		cout << "open fail!\n";
		exit(0);
	}
	while (!datafile.eof())
	{
		datafile >> cli.id;
		datafile >> cli.name;
		datafile >> cli.flight;
		datafile >> cli.date;
		datafile >> cli.mile;
		cli.name = UTF8ToGB(cli.name.c_str());
		//cout << cli.id << " " << cli.name << " " << cli.flight << " " << cli.date << " " << cli.mile << endl;
		ClientV.push_back(cli);
	}
	datafile.close();

}

//�������֤����
int Get_ID_Num(string s)
{
	int  sum = 0;
	for (int i = 0; i < s.length() ; i++)
	{
		sum = sum + s[i] -48;
	}
	
	return sum;
}

//����ַ�����ͻ
void Hash_Link(ConflictLink* c,vector<Client> ClientV)
{
	int s = 0;
	ClientInfo Client_Info[MAXH];
	int hash;
	int Hash_Table[MAXH] = { 0 };
	ConflictLink* cl;
	for (int i = 0; i < ClientV.size(); i++)
	{
		hash = Get_ID_Num(ClientV[i].id) % MAXH;
		
		if (Hash_Table[hash] == 0)//���ǵ�һ�Σ�֮ǰû������
		{
			ConflictLink* p = new ConflictLink;
			p->Freq++;
			p->id = ClientV[i].id;
			p->name = ClientV[i].name;
			p->Sum_Mile += ClientV[i].mile;
			p->next = NULL;
			//���һ�κ��༰ʱ����Ϣ
			information* n = new information;
			p->inf.next = n;//��һ��λ�÷���ʹ�ã�����֮���while
			n->date = ClientV[i].date;
			n->flight = ClientV[i].flight;
			n->next = NULL;
			//ͬ�����õ�һ�����������ı���
			c[hash].next = p;
			Hash_Table[hash]++;
		}
		else //�ѳ��֣��ж��Ƿ����ظ��ģ��еĻ��ͺϲ���û�о��½�
		{
			ConflictLink* p = c[hash].next;
			if (p->id == ClientV[i].id)//��һ������ͬһ����
			{
				//��Ƶ�μ�1������ۼ�
				p->Freq++; p->Sum_Mile += ClientV[i].mile;
				//�½�һ����Ϣ��¼
				information* mation = p->inf.next;
				//cout << mation->flight << endl;
				//�ҵ����һ����Ϣ��ַ
				while (mation->next != NULL) mation = mation->next;
				//�½���Ϣ
				information* new_record = new information;
				new_record->date = ClientV[i].date;
				new_record->flight = ClientV[i].flight;
				new_record->next = NULL;
				mation->next = new_record;
			}
			else//��һ������
			{
				ConflictLink* fa = p; //fa��¼p��ǰһ��ָ��
				p = p->next;
				//Ѱ��һ��ID�ļ�¼
				while (p != NULL && p->id != ClientV[i].id) { fa = p; p = p->next; }
				if (p == NULL)//û�ҵ�
				{
					//��c[hash]�����¼�һ����㣬�������
					ConflictLink* new_con = new ConflictLink;
					new_con->Freq++;
					new_con->id = ClientV[i].id;
					new_con->name = ClientV[i].name;
					new_con->Sum_Mile += ClientV[i].mile;
					new_con->next = NULL;
					//�½��ý���һ����Ϣ
					information* new_record = new information;
					new_record->date = ClientV[i].date;
					new_record->flight = ClientV[i].flight;
					new_record->next = NULL;
					new_con->inf.next = new_record;
					fa->next = new_con;
					Hash_Table[hash]++;
				}
				else//�ҵ���
				{
					p->Freq++; p->Sum_Mile += ClientV[i].mile;
					//�½�һ����Ϣ��¼
					information* mation = p->inf.next;
					while (mation->next != NULL) mation = mation->next;
					information* new_record = new information;
					new_record->date = ClientV[i].date;
					new_record->flight = ClientV[i].flight;
					new_record->next = NULL;
					mation->next = new_record;
				}
			}
		}
	}
	int sum = 0;
	
	for (int i = 0; i < MAXH; i++)
	{
		
		sum += Hash_Table[i];
		if (Hash_Table[i] != 0)
		{
			s++;
			cout << "��" << i << "��λ����" << Hash_Table[i] << "��:"<<endl;
			ConflictLink* p = c[i].next;
			while(p!=NULL)
			{
				cout <<"\t����:"<< p->name << " ID:" << p->id << " Ƶ��:"<<p->Freq<<" �����:" << p->Sum_Mile << endl;
				p = p->next;
			}
		}
	}
	cout << "����"<<sum << "��  ��ϣ���װ������Ϊ" << (double)s/MAXH;
}

//����̽�ⷨ
void Hash_Linear(ClientInfo c_INFO[], vector<Client> ClientV)
{
	int Hash_Table[MAXN] = { 0 };
	int pos;
	for (int i = 0; i < ClientV.size(); i++)
	{
		pos = Get_ID_Num(ClientV[i].id);
		if (Hash_Table[pos] == 0)//��λ��û�����ݷŹ�
		{
			c_INFO[pos].Freq++;
			c_INFO[pos].id = ClientV[i].id;
			c_INFO[pos].name = ClientV[i].name;
			c_INFO[pos].reLocNum = 0;
			c_INFO[pos].Sum_Mile += ClientV[i].mile;
			information* new_info = new information;
			new_info->date = ClientV[i].date;
			new_info->flight = ClientV[i].flight;
			new_info->next = NULL;
			c_INFO[pos].inf.next = new_info;
			Hash_Table[pos]++;
			c_INFO[pos].Num_Used++;
		}
		else if (c_INFO[pos].id == ClientV[i].id)//�Ź��ˣ�������ͬһ��
		{
			c_INFO[pos].Freq++;
			c_INFO[pos].Sum_Mile += ClientV[i].mile;
			information* mation = c_INFO[pos].inf.next;
			//cout << mation->flight << endl;
			//�ҵ����һ����Ϣ��ַ
			while (mation->next != NULL) mation = mation->next;
			//�½���Ϣ
			information* new_record = new information;
			new_record->date = ClientV[i].date;
			new_record->flight = ClientV[i].flight;
			new_record->next = NULL;
			mation->next = new_record;
			Hash_Table[pos]++;
		}
		else //����ͬһ��������һ����ַ
		{
			int Empty_Pos = i;
			int j = 1;
			bool flag = false;
			for (j = 1; (j + i) % MAXN != i; j++)//����֮���λ������û�������
			{
				int p = (j + i) % MAXN;
				if (Empty_Pos == i && Hash_Table[p] == 0) Empty_Pos = p;
				if (c_INFO[p].id == ClientV[i].id)
				{
					flag = true;
					break;
				}
			}
			if(flag)//��һ�������ں���
			{ 
				pos = (j + i) % MAXN;
				Hash_Table[pos]++;
				c_INFO[pos].Freq++;
				c_INFO[pos].Sum_Mile += ClientV[i].mile;
			}
			else //û��һ������
			{
				c_INFO[Empty_Pos].Freq++;
				c_INFO[Empty_Pos].id = ClientV[i].id;
				c_INFO[Empty_Pos].name = ClientV[i].name;
				c_INFO[Empty_Pos].reLocNum = 0;
				c_INFO[Empty_Pos].Sum_Mile += ClientV[i].mile;
				information* new_info = new information;
				new_info->date = ClientV[i].date;
				new_info->flight = ClientV[i].flight;
				new_info->next = NULL;
				c_INFO[Empty_Pos].inf.next = new_info;
				Hash_Table[Empty_Pos]++;
				c_INFO[Empty_Pos].Num_Used++;
				if (Empty_Pos > i) c_INFO[Empty_Pos].reLocNum = Empty_Pos - i;
				else c_INFO[Empty_Pos].reLocNum = Empty_Pos + MAXN - i;
			}
		}
	}

	int sum = 0, s = 0, c = 0;;
	for (int i = 0; i < MAXN; i++)
	{

		sum += Hash_Table[i];
		if (Hash_Table[i] != 0)
		{
			if (c_INFO[i].reLocNum > 0) c++;
			s++;
			cout << "����Ϊ " << c_INFO[i].name << " idΪ " << c_INFO[i].id << " �˻�Ƶ��Ϊ " << c_INFO[i].Freq << " �����Ϊ " << c_INFO[i].Sum_Mile << " ���¶�λ����Ϊ " << c_INFO[i].reLocNum << endl;
		}
	}
	cout << "����" << sum << "������ �� ��" << s << "�ˣ�  ��ͻ����Ϊ" << c << "��";
}

//��VIP
void Find(ConflictLink c[])
{
	cout << "����VIP�û���Ƶ��(>):";
	int f;
	cin >> f;
	cout << "����VIP�û��������(>):";
	double m;
	cin >> m;
	for (int i = 0; i < MAXH; i++)
	{
		ConflictLink* p = c[i].next;
		while (p != NULL)
		{
			if (p->Freq != 0 && p->Freq > f && p->Sum_Mile > m)
			{
				cout << "����:" << p->name << "  id:" << p->id << "  Ƶ��:" << p->Freq << "  �����:" << p->Sum_Mile << endl;
			}
			p = p->next;
		}
	}
}