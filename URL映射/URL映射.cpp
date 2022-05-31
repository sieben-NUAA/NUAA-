#include<iostream>
#include<string>
#include<vector>
using namespace std;

const int n = 200;
string p[n], r[n], des[n];

bool match(string& p, string& s,int mathod)//p为已知，s为待判断
{
	int len1 = p.length(), len2 = s.length();
	int i = 0, j = 0;
	while (i < len1 && j < len2){						//长度判断
		if (p[i] == s[j]){								//如果两个相等，下一个继续判断
			i++; j++;													
		}
		else {
			if (p[i++] != '<') return false;			//不一样了，开始判断是不是<> ，如果是就开始判断类型相等不
			if (mathod) cout << " ";					//method=1， 是输出模式，先输出  
			int flag = 0;
			if (p[i] == 'i') {							//《int》类型
				while (s[j] && isdigit(s[j]))			//是数字
				{
					if (s[j] != '0') flag = 1;			//不是0 记录
					if (flag && mathod) cout << s[j];
					j++;
				}
				if (!flag) return false;				//如果都是0，没有非0数字，就返回错误
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