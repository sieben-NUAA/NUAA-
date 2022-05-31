#include<iostream>
#include<fstream>
#include<string>
#include"Blockchain.h"
using namespace std;


//计算ascll值
int CalAscll(string s)
{
	int sum = 0;
	for (int i = 0; i < s.length(); i++)
		sum += s[i];
	return sum;
}

//判断有效
void Is_Valid(BlockChain c)
{
	BlockChain p = c;
	if (CalAscll(p->inf.info) % 113 == p->inf.CheckCode)
	{
		while (p->next)
		{
			int temp;
			temp = p->inf.CheckCode + p->next->inf.number + CalAscll(p->next->inf.info);
			if (temp % 113 == p->next->inf.CheckCode) p = p->next;
			else 
			{ 
				cout <<"第一个无效结点的编号为:"<< p->next->inf.number<<endl; 
				cout << "校验码应该为:" << temp % 113<<endl; 
				cout << "是否进行自动修改?(y/n)";
				char q;
				cin >> q;
				if (q == 'y')
				{
					cout << "正在修改中，请稍后....." << endl;
					BlockChain f = p;
					p = p->next;
					temp = (f->inf.CheckCode + p->inf.number + CalAscll(p->inf.info));//篡改结点使合理
					while (temp % 113 != p->inf.CheckCode)
					{
						p->inf.CheckCode = temp % 113;
						f = p; p = p->next;
						if (p == NULL) break;
						temp = (f->inf.CheckCode + p->inf.number + CalAscll(p->inf.info));
					}
					cout << "修改完成！" << endl;
					return;
				}
				else return;
			}
		}
		cout << "文件无误，有效！" << endl;
		return;
	}
	else 
	{ 
		cout <<"第一个无效结点的编号为:"<< p->inf.number << endl<<"校验码应该为:"<< CalAscll(p->inf.info) % 113 << endl;
		cout << "是否进行自动修改?(y/n)";
		char q;
		cin >> q;
		int temp= CalAscll(p->inf.info) % 113;
		p->inf.CheckCode = temp;
		if (q == 'y')
		{
			cout << "正在修改中，请稍后....." << endl;
			BlockChain f = p;
			p = p->next;
			temp = (f->inf.CheckCode + p->inf.number + CalAscll(p->inf.info));//篡改结点使合理
			while (temp % 113 != p->inf.CheckCode)
			{
				p->inf.CheckCode = temp % 113;
				f = p; p = p->next;
				if (p == NULL) break;
				temp = (f->inf.CheckCode + p->inf.number + CalAscll(p->inf.info));
			}
			cout << "修改完成！" << endl;
			return;
		}
		else return;
	}
}

//读取数据
void ReadData(fstream& file, BlockChain& head)
{
	file.open("data.txt", ios::in | ios::out);
	if (file.fail())
	{
		cout << "文件打开失败!";
		exit(0);
	}
	file >> head->inf.number;
	
	file.ignore();
	getline(file, head->inf.info);
	
	file >> head->inf.CheckCode;
	BlockChain c = head;
	while (!file.eof())
	{
		BlockChain p = new Block;
		p->next = NULL;
		file >> p->inf.number;
		file.ignore();
		getline(file, p->inf.info);
		file >> p->inf.CheckCode;
		c->next = p;
		c = p;
	}
	file.close();
}

//将更新的数据写入文件
void UpdataData(BlockChain c)
{
	fstream file;
	file.open("temp.txt", ios::in | ios::out | ios::app);
	BlockChain p = c;
	while (p->next)
	{
		file << p->inf.number << endl;
		file << p->inf.info << endl;
		file << p->inf.CheckCode << endl;
		p = p->next;
	}
	file << p->inf.number << endl;
	file << p->inf.info << endl;
	file << p->inf.CheckCode ;

	file.close();
	system("del data.txt");
	system("ren temp.txt data.txt");
	
}

//查找编号相同的结点
BlockChain Find(BlockChain c, int des)
{
	BlockChain p = c;
	int sign = 0;
	while (p)
	{
		
		if (p->inf.number == des) { sign = 1; break; }
		p = p->next;
	}
	if (sign)
		return p;
	else return NULL;
}

//添加
void Insert(BlockChain& c)
{
	BlockChain p = c;
	while (p->next) p = p->next;//找到末尾结点
	BlockChain n =new Block;

	cout << "请输入新插入的结点的编号:";
	int num;
	cin >> num;//输入结点编号
	while (Find(c, num) != NULL)
	{
		cout << "编号重复！请重新输入编号:";
		cin >> num;
	}
	cout << "请输入新插入的结点的信息：";

	string information;
	cin.ignore();
	getline(cin, n->inf.info);//输入结点信息

	n->inf.number = num;
	n->inf.CheckCode = (p->inf.CheckCode + n->inf.number + CalAscll(n->inf.info)) % 113;
	cout << "该结点的校验码为" << n->inf.CheckCode << endl;
	p->next = n;
	n->next = NULL;
}

//修改
void Revise(BlockChain& c)
{
	int num;
	cout << "输入需要更改的信息编号:";
	cin >> num;

	BlockChain p,f=c;
	p = Find(c, num);
	
	while (!p)
	{
		cout << "未找到该编号！请重新输入编号:";
		cin >> num;
		p = Find(c, num);
	}
	
	while (f->next != p && f->next)f = f->next;

	string s;
	cout << "请输入修改后的结点信息:";
	getchar();
	getline(cin, s);
	p->inf.info = s;
	
	int temp = (f->inf.CheckCode + p->inf.number + CalAscll(p->inf.info));//篡改结点使合理
	while (p && temp % 113 != p->inf.CheckCode)
	{
		p->inf.CheckCode = temp % 113;
		f = p; p = p->next;
		if(p)
			temp = (f->inf.CheckCode + p->inf.number + CalAscll(p->inf.info));
	}

}

//打印所有
void Print_All(BlockChain c)
{
	BlockChain p = c;
	cout << "**********************" << endl;
	while (p)
	{
		cout << "编号为:" << p->inf.number <<endl ;
		cout << "信息为:" << p->inf.info << endl;
		cout << "校验码为:" << p->inf.CheckCode << endl;
		p = p->next;
		cout << "**********************" << endl;
	}
}
