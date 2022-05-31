#include<iostream>
#include<fstream>
#include<string>
#include"Blockchain.h"
using namespace std;


//����ascllֵ
int CalAscll(string s)
{
	int sum = 0;
	for (int i = 0; i < s.length(); i++)
		sum += s[i];
	return sum;
}

//�ж���Ч
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
				cout <<"��һ����Ч���ı��Ϊ:"<< p->next->inf.number<<endl; 
				cout << "У����Ӧ��Ϊ:" << temp % 113<<endl; 
				cout << "�Ƿ�����Զ��޸�?(y/n)";
				char q;
				cin >> q;
				if (q == 'y')
				{
					cout << "�����޸��У����Ժ�....." << endl;
					BlockChain f = p;
					p = p->next;
					temp = (f->inf.CheckCode + p->inf.number + CalAscll(p->inf.info));//�۸Ľ��ʹ����
					while (temp % 113 != p->inf.CheckCode)
					{
						p->inf.CheckCode = temp % 113;
						f = p; p = p->next;
						if (p == NULL) break;
						temp = (f->inf.CheckCode + p->inf.number + CalAscll(p->inf.info));
					}
					cout << "�޸���ɣ�" << endl;
					return;
				}
				else return;
			}
		}
		cout << "�ļ�������Ч��" << endl;
		return;
	}
	else 
	{ 
		cout <<"��һ����Ч���ı��Ϊ:"<< p->inf.number << endl<<"У����Ӧ��Ϊ:"<< CalAscll(p->inf.info) % 113 << endl;
		cout << "�Ƿ�����Զ��޸�?(y/n)";
		char q;
		cin >> q;
		int temp= CalAscll(p->inf.info) % 113;
		p->inf.CheckCode = temp;
		if (q == 'y')
		{
			cout << "�����޸��У����Ժ�....." << endl;
			BlockChain f = p;
			p = p->next;
			temp = (f->inf.CheckCode + p->inf.number + CalAscll(p->inf.info));//�۸Ľ��ʹ����
			while (temp % 113 != p->inf.CheckCode)
			{
				p->inf.CheckCode = temp % 113;
				f = p; p = p->next;
				if (p == NULL) break;
				temp = (f->inf.CheckCode + p->inf.number + CalAscll(p->inf.info));
			}
			cout << "�޸���ɣ�" << endl;
			return;
		}
		else return;
	}
}

//��ȡ����
void ReadData(fstream& file, BlockChain& head)
{
	file.open("data.txt", ios::in | ios::out);
	if (file.fail())
	{
		cout << "�ļ���ʧ��!";
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

//�����µ�����д���ļ�
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

//���ұ����ͬ�Ľ��
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

//���
void Insert(BlockChain& c)
{
	BlockChain p = c;
	while (p->next) p = p->next;//�ҵ�ĩβ���
	BlockChain n =new Block;

	cout << "�������²���Ľ��ı��:";
	int num;
	cin >> num;//��������
	while (Find(c, num) != NULL)
	{
		cout << "����ظ���������������:";
		cin >> num;
	}
	cout << "�������²���Ľ�����Ϣ��";

	string information;
	cin.ignore();
	getline(cin, n->inf.info);//��������Ϣ

	n->inf.number = num;
	n->inf.CheckCode = (p->inf.CheckCode + n->inf.number + CalAscll(n->inf.info)) % 113;
	cout << "�ý���У����Ϊ" << n->inf.CheckCode << endl;
	p->next = n;
	n->next = NULL;
}

//�޸�
void Revise(BlockChain& c)
{
	int num;
	cout << "������Ҫ���ĵ���Ϣ���:";
	cin >> num;

	BlockChain p,f=c;
	p = Find(c, num);
	
	while (!p)
	{
		cout << "δ�ҵ��ñ�ţ�������������:";
		cin >> num;
		p = Find(c, num);
	}
	
	while (f->next != p && f->next)f = f->next;

	string s;
	cout << "�������޸ĺ�Ľ����Ϣ:";
	getchar();
	getline(cin, s);
	p->inf.info = s;
	
	int temp = (f->inf.CheckCode + p->inf.number + CalAscll(p->inf.info));//�۸Ľ��ʹ����
	while (p && temp % 113 != p->inf.CheckCode)
	{
		p->inf.CheckCode = temp % 113;
		f = p; p = p->next;
		if(p)
			temp = (f->inf.CheckCode + p->inf.number + CalAscll(p->inf.info));
	}

}

//��ӡ����
void Print_All(BlockChain c)
{
	BlockChain p = c;
	cout << "**********************" << endl;
	while (p)
	{
		cout << "���Ϊ:" << p->inf.number <<endl ;
		cout << "��ϢΪ:" << p->inf.info << endl;
		cout << "У����Ϊ:" << p->inf.CheckCode << endl;
		p = p->next;
		cout << "**********************" << endl;
	}
}
