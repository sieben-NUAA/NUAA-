#include<iostream>
#include<algorithm>
#include<fstream>
#include<vector>
#include<string>
#include"cilent.h"
using namespace std;

int ClientInfo::Num_Used = 0;

// 中文转换编码
string UTF8ToGB(const char* str)
{
	string rst;
	WCHAR* strSrc;
	LPSTR szRes;

	// 获取临时变量大小
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

//读取文件内容
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

//计算身份证的数
int Get_ID_Num(string s)
{
	int  sum = 0;
	for (int i = 0; i < s.length() ; i++)
	{
		sum = sum + s[i] -48;
	}
	
	return sum;
}

//链地址解决冲突
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
		
		if (Hash_Table[hash] == 0)//这是第一次，之前没有数据
		{
			ConflictLink* p = new ConflictLink;
			p->Freq++;
			p->id = ClientV[i].id;
			p->name = ClientV[i].name;
			p->Sum_Mile += ClientV[i].mile;
			p->next = NULL;
			//添加一次航班及时间信息
			information* n = new information;
			p->inf.next = n;//第一个位置放弃使用，方便之后的while
			n->date = ClientV[i].date;
			n->flight = ClientV[i].flight;
			n->next = NULL;
			//同样弃用第一个，方便后面的遍历
			c[hash].next = p;
			Hash_Table[hash]++;
		}
		else //已出现，判断是否有重复的，有的话就合并，没有就新建
		{
			ConflictLink* p = c[hash].next;
			if (p->id == ClientV[i].id)//第一个就是同一个人
			{
				//将频次加1，里程累计
				p->Freq++; p->Sum_Mile += ClientV[i].mile;
				//新建一个信息记录
				information* mation = p->inf.next;
				//cout << mation->flight << endl;
				//找到最后一个信息地址
				while (mation->next != NULL) mation = mation->next;
				//新建信息
				information* new_record = new information;
				new_record->date = ClientV[i].date;
				new_record->flight = ClientV[i].flight;
				new_record->next = NULL;
				mation->next = new_record;
			}
			else//第一个不是
			{
				ConflictLink* fa = p; //fa记录p的前一个指针
				p = p->next;
				//寻找一样ID的记录
				while (p != NULL && p->id != ClientV[i].id) { fa = p; p = p->next; }
				if (p == NULL)//没找到
				{
					//在c[hash]后面新加一个结点，添加新人
					ConflictLink* new_con = new ConflictLink;
					new_con->Freq++;
					new_con->id = ClientV[i].id;
					new_con->name = ClientV[i].name;
					new_con->Sum_Mile += ClientV[i].mile;
					new_con->next = NULL;
					//新建该结点的一个信息
					information* new_record = new information;
					new_record->date = ClientV[i].date;
					new_record->flight = ClientV[i].flight;
					new_record->next = NULL;
					new_con->inf.next = new_record;
					fa->next = new_con;
					Hash_Table[hash]++;
				}
				else//找到了
				{
					p->Freq++; p->Sum_Mile += ClientV[i].mile;
					//新建一个信息记录
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
			cout << "第" << i << "个位置有" << Hash_Table[i] << "个:"<<endl;
			ConflictLink* p = c[i].next;
			while(p!=NULL)
			{
				cout <<"\t姓名:"<< p->name << " ID:" << p->id << " 频次:"<<p->Freq<<" 里程数:" << p->Sum_Mile << endl;
				p = p->next;
			}
		}
	}
	cout << "共有"<<sum << "人  哈希表的装填因子为" << (double)s/MAXH;
}

//线性探测法
void Hash_Linear(ClientInfo c_INFO[], vector<Client> ClientV)
{
	int Hash_Table[MAXN] = { 0 };
	int pos;
	for (int i = 0; i < ClientV.size(); i++)
	{
		pos = Get_ID_Num(ClientV[i].id);
		if (Hash_Table[pos] == 0)//该位置没有数据放过
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
		else if (c_INFO[pos].id == ClientV[i].id)//放过了，但是是同一个
		{
			c_INFO[pos].Freq++;
			c_INFO[pos].Sum_Mile += ClientV[i].mile;
			information* mation = c_INFO[pos].inf.next;
			//cout << mation->flight << endl;
			//找到最后一个信息地址
			while (mation->next != NULL) mation = mation->next;
			//新建信息
			information* new_record = new information;
			new_record->date = ClientV[i].date;
			new_record->flight = ClientV[i].flight;
			new_record->next = NULL;
			mation->next = new_record;
			Hash_Table[pos]++;
		}
		else //不是同一个，查下一个地址
		{
			int Empty_Pos = i;
			int j = 1;
			bool flag = false;
			for (j = 1; (j + i) % MAXN != i; j++)//查找之后的位置里有没有这个人
			{
				int p = (j + i) % MAXN;
				if (Empty_Pos == i && Hash_Table[p] == 0) Empty_Pos = p;
				if (c_INFO[p].id == ClientV[i].id)
				{
					flag = true;
					break;
				}
			}
			if(flag)//有一样的人在后面
			{ 
				pos = (j + i) % MAXN;
				Hash_Table[pos]++;
				c_INFO[pos].Freq++;
				c_INFO[pos].Sum_Mile += ClientV[i].mile;
			}
			else //没有一样的人
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
			cout << "姓名为 " << c_INFO[i].name << " id为 " << c_INFO[i].id << " 乘机频率为 " << c_INFO[i].Freq << " 里程数为 " << c_INFO[i].Sum_Mile << " 重新定位次数为 " << c_INFO[i].reLocNum << endl;
		}
	}
	cout << "共有" << sum << "组数据 ， 有" << s << "人，  冲突次数为" << c << "次";
}

//查VIP
void Find(ConflictLink c[])
{
	cout << "输入VIP用户的频次(>):";
	int f;
	cin >> f;
	cout << "输入VIP用户的里程数(>):";
	double m;
	cin >> m;
	for (int i = 0; i < MAXH; i++)
	{
		ConflictLink* p = c[i].next;
		while (p != NULL)
		{
			if (p->Freq != 0 && p->Freq > f && p->Sum_Mile > m)
			{
				cout << "姓名:" << p->name << "  id:" << p->id << "  频次:" << p->Freq << "  里程数:" << p->Sum_Mile << endl;
			}
			p = p->next;
		}
	}
}