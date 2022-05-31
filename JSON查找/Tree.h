#include <iostream>
#include<string>
using namespace std;
typedef struct TreeNode
{
	string type;//¿‡–Õ
	string keyvalue;//º¸÷µ
	struct TreeNode* firstchild, * nextbrother, * farther;
}TreeNode,*Tree;
void InitTree(Tree& T);
void format(string& s);
void Search_json(Tree T, string s);
void CreatTree(Tree& T);
#pragma once
