#include"Tree.h"
#include <iostream>
#include<string>
#include<fstream>
using namespace std;

//构造一个空二叉树
void InitTree(Tree& T)
{
	T = new TreeNode;
	T->farther = NULL;
	T->type = "OBJECT";
	T->firstchild = NULL;
	T->nextbrother = NULL;
	
	if (T == NULL) { cout << "新建时空间分配错误！\n" << endl; return; }
}

//删除转义字符
void format(string& s)
{
	//把字符串s中的转义字符\ 删除，并保留其下一个字符
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
	while (pos != -1)//循环寻找分割的  .  然后找到的话就说明是进入孩子
	{
		json = json->firstchild;//进入孩子寻找 . 后面的类型
		s_part = s.substr(0, pos); //将  .  前面的字符串切割出来，寻找相应的类型
		//cout << s_part << " ";
		while (json)//寻找与  .  前面一致的类型
			if (json->type == s_part)
				break;
			else json = json->nextbrother;
		s.erase(0, pos+1);//将  . 以及前面的都扔掉，寻找  .  后面的类型
		pos = s.find('.', 0);
	}

	//由于最后一遍是最后一个  .  后面的还有没有查找的，所以需要再来最后一遍
	json = json->firstchild;
	s_part = s.substr(0, pos);
	//cout << s_part << " ";
	while (json)
		if (json->type == s_part)
			break;
		else json = json->nextbrother;

	if (json != NULL && json->firstchild == NULL) // 没有孩子，也就是不是个类
		cout << "STRING " << json->keyvalue << endl;
	else if (json != NULL && json->firstchild != NULL) //有孩子，是个类
		cout << json->keyvalue << endl;
	else cout << "NOTEXIST" << endl;  // 不存在
	//cout << s << endl;
}

//输入树
void CreatTree(Tree& T)
{
	string s;
	fstream file;
	file.open("JSON.txt", ios::in);
	if (file.fail()) { cout << "open failed!" << endl; exit(0); }

	int n, m;
	file >> n >> m;
	file.ignore();

	int sign = 0; // 判断是否是一个对象

	Tree p = T;
	Tree fa = T;// 记录父亲是哪个
	for (int i = 0; i < n; i++)//n行数据读取
	{
		getline(file, s, '\n');
		if (s.length() == 1 && s[0] == '{')//第一行，只有一个{
		{
			sign = 1;
			continue;
		}
		if (s.length() == 2 && s[0] == '}')//中间的对象结束有},两个,那么返回上一层
		{
			p = fa;
			fa = fa->farther;
			continue;
		}
		if (s.length() == 1 && s[0] == '}')//最后一行结束 有 }
			break;

		int k;
		string KeyType;//读取引号中的属性名
		string Key;//读取属性内容
		for (int j = 0; j < s.length(); j++)//一行字符串中分割类型和值
		{
			if (s[j] == '"')//从引号开始
			{
				k = s.find(':', j + 1);//查找: 的位置
				KeyType = s.substr(j + 1, k - j - 2);//截取类型字符串
				format(KeyType);

				int len = s.length();
				int pos = s.find('"', k);//从:开始找键值的" 如果为-1说明后面没有" 也就是后面是一个对象
				if (pos == -1)//没有,这是个对象,新建一个对象结点
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
				else//这不是对象
				{
					int pos_end = len;//键值的位置
					while (s[pos_end] != '"') pos_end--;

					Key = s.substr(pos + 1, pos_end - pos - 1);//截取键值字符串
					format(Key);

					if (sign)//前一个是{ 要新建一个孩子
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
					else//前一个不是{ 新建一个兄弟
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

	//进行查找
	string f;
	for (int i = 0; i < m; i++)
	{
		getline(file, f, '\n');
		Search_json(T, f);
	}
	file.close();
}