#include"Tree.h"
#include <iostream>
#include<string>
#include<fstream>
using namespace std;

//����һ���ն�����
void InitTree(Tree& T)
{
	T = new TreeNode;
	T->farther = NULL;
	T->type = "OBJECT";
	T->firstchild = NULL;
	T->nextbrother = NULL;
	
	if (T == NULL) { cout << "�½�ʱ�ռ�������\n" << endl; return; }
}

//ɾ��ת���ַ�
void format(string& s)
{
	//���ַ���s�е�ת���ַ�\ ɾ��������������һ���ַ�
	for (int i = 0; i < s.size(); i++)
	{
		if (s[i] == '\\')
		{
			s.erase(s.begin() + i);
		}
	}
}

void Search_json(Tree T, string s)
{
	int pos = s.find('.', 0);
	Tree json = T;
	string s_part;
	while (pos != -1)//ѭ��Ѱ�ҷָ��  .  Ȼ���ҵ��Ļ���˵���ǽ��뺢��
	{
		json = json->firstchild;//���뺢��Ѱ�� . ���������
		s_part = s.substr(0, pos); //��  .  ǰ����ַ����и������Ѱ����Ӧ������
		//cout << s_part << " ";
		while (json)//Ѱ����  .  ǰ��һ�µ�����
			if (json->type == s_part)
				break;
			else json = json->nextbrother;
		s.erase(0, pos+1);//��  . �Լ�ǰ��Ķ��ӵ���Ѱ��  .  ���������
		pos = s.find('.', 0);
	}

	//�������һ�������һ��  .  ����Ļ���û�в��ҵģ�������Ҫ�������һ��
	json = json->firstchild;
	s_part = s.substr(0, pos);
	//cout << s_part << " ";
	while (json)
		if (json->type == s_part)
			break;
		else json = json->nextbrother;

	if (json != NULL && json->firstchild == NULL) // û�к��ӣ�Ҳ���ǲ��Ǹ���
		cout << "STRING " << json->keyvalue << endl;
	else if (json != NULL && json->firstchild != NULL) //�к��ӣ��Ǹ���
		cout << json->keyvalue << endl;
	else cout << "NOTEXIST" << endl;  // ������
	//cout << s << endl;
}

//������
void CreatTree(Tree& T)
{
	string s;
	fstream file;
	file.open("JSON.txt", ios::in);
	if (file.fail()) { cout << "open failed!" << endl; exit(0); }

	int n, m;
	file >> n >> m;
	file.ignore();

	int sign = 0; // �ж��Ƿ���һ������

	Tree p = T;
	Tree fa = T;// ��¼�������ĸ�
	for (int i = 0; i < n; i++)//n�����ݶ�ȡ
	{
		getline(file, s, '\n');
		if (s.length() == 1 && s[0] == '{')//��һ�У�ֻ��һ��{
		{
			sign = 1;
			continue;
		}
		if (s.length() == 2 && s[0] == '}')//�м�Ķ��������},����,��ô������һ��
		{
			p = fa;
			fa = fa->farther;
			continue;
		}
		if (s.length() == 1 && s[0] == '}')//���һ�н��� �� }
			break;

		int k;
		string KeyType;//��ȡ�����е�������
		string Key;//��ȡ��������
		for (int j = 0; j < s.length(); j++)//һ���ַ����зָ����ͺ�ֵ
		{
			if (s[j] == '"')//�����ſ�ʼ
			{
				k = s.find(':', j + 1);//����: ��λ��
				KeyType = s.substr(j + 1, k - j - 2);//��ȡ�����ַ���
				format(KeyType);

				int len = s.length();
				int pos = s.find('"', k);//��:��ʼ�Ҽ�ֵ��" ���Ϊ-1˵������û��" Ҳ���Ǻ�����һ������
				if (pos == -1)//û��,���Ǹ�����,�½�һ��������
				{
					sign = 1;
					Tree bro = new TreeNode;
					p->nextbrother = bro;
					bro->farther = fa;
					bro->type = KeyType;
					bro->keyvalue = "OBJECT";
					bro->firstchild = NULL;
					bro->nextbrother = NULL;
					p = bro;

				}
				else//�ⲻ�Ƕ���
				{
					int pos_end = len;//��ֵ��λ��
					while (s[pos_end] != '"') pos_end--;

					Key = s.substr(pos + 1, pos_end - pos - 1);//��ȡ��ֵ�ַ���
					format(Key);

					if (sign)//ǰһ����{ Ҫ�½�һ������
					{
						Tree child = new TreeNode;
						child->farther = p;
						child->firstchild = NULL;
						child->keyvalue = Key;
						child->nextbrother = NULL;
						child->type = KeyType;
						fa = p;
						p->firstchild = child;
						p = child;
						sign = 0;
					}
					else//ǰһ������{ �½�һ���ֵ�
					{
						Tree temp = new TreeNode;
						temp->farther = fa;
						temp->firstchild = NULL;
						temp->keyvalue = Key;
						temp->nextbrother = NULL;
						temp->type = KeyType;
						p->nextbrother = temp;
						p = temp;
					}
				}
				break;
			}
		}
	}

	//���в���
	string f;
	for (int i = 0; i < m; i++)
	{
		getline(file, f, '\n');
		Search_json(T, f);
	}
	file.close();
}