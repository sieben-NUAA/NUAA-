#include<iostream>
#include<string>
#include<vector>
using namespace std;

const int n = 200;
string p[n], r[n], des[n];

bool match(string& p, string& s,int mathod)//pΪ��֪��sΪ���ж�
{
	int len1 = p.length(), len2 = s.length();
	int i = 0, j = 0;
	while (i < len1 && j < len2){						//�����ж�
		if (p[i] == s[j]){								//���������ȣ���һ�������ж�
			i++; j++;													
		}
		else {
			if (p[i++] != '<') return false;			//��һ���ˣ���ʼ�ж��ǲ���<> ������ǾͿ�ʼ�ж�������Ȳ�
			if (mathod) cout << " ";					//method=1�� �����ģʽ�������  
			int flag = 0;
			if (p[i] == 'i') {							//��int������
				while (s[j] && isdigit(s[j]))			//������
				{
					if (s[j] != '0') flag = 1;			//����0 ��¼
					if (flag && mathod) cout << s[j];
					j++;
				}
				if (!flag) return false;				//�������0��û�з�0���֣��ͷ��ش���
				i += 4;
			}
			else if (p[i] == 's') {
				while (s[j] && s[j] != '/')
				{
					flag = 1;
					if (flag && mathod) cout << s[j];
					j++;
				}
				if (!flag) return false;
				i += 4;
			}
			else if (p[i] == 'p')
			{
				if (mathod)
				{
					while (s[j]) {
						cout << s[j];
						j++;
					}
				}
				return true;
			}
		}
	}
	return i == len1 && j == len2;
}

int main()
{
	int n, m;
	cin >> n >> m;
	for (int i = 0; i < n; i++) cin >> p[i] >> r[i];
	for (int i = 0; i < m; i++)
		cin >> des[i];

	for (int i = 0; i < m; i++)
	{
		int sign = 0;
		for (int j = 0; j < n; j++)
		{
			if (match(p[j], des[i], 0)) {
				cout << r[j];
				match(p[j], des[i], 1);
				sign = 1;
				break;
			}
		}
		if (!sign) cout << "404";
		cout << endl;
	}
	return 0;
}