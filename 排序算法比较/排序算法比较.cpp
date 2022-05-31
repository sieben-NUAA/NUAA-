#include<iostream>
#include<time.h>
#include<algorithm>
#include<Windows.h>
#include<fstream>
#include"Queue.h"
#include<queue>
using namespace std;

const int AMOUNT = 50000;
string Rand_File[10] = {
	"rand_ascend_1.txt","rand2.txt","rand3.txt","rand4.txt","rand5.txt",
	"rand6.txt","rand7.txt","rand8.txt","rand9.txt","rand_descend_10.txt"
};

//获取随机数
void GetRand()
{
	int temp[AMOUNT + 1] = { 0 };
	srand((int)time(0));
	system("del rand_ascend_1.txt");
	system("del rand2.txt");
	system("del rand3.txt");
	system("del rand4.txt");
	system("del rand5.txt");
	system("del rand6.txt");
	system("del rand7.txt");
	system("del rand8.txt");
	system("del rand9.txt");
	system("del rand_descend_10.txt");
	fstream datafile;
	cout << "正在生成第1组数据....."<<endl;
	//datafile.open("rand_ascend_1.txt", ios::in | ios::out | ios::app);
	datafile.open(Rand_File[0].c_str(), ios::app | ios::in);
	for (int i = 0; i < AMOUNT; i++)
	{
		datafile << i+1 << " ";
		//cout << t << " ";
	}
	cout << "第1组数据生成完成" << endl;
	datafile.close();;

	for (int i = 1; i < 9; i++)
	{
		cout << "正在生成第" << i << "组数据....." << endl;
		datafile.open(Rand_File[i].c_str(), ios::app | ios::in);
		for (int j = 0; j < AMOUNT; j++)
		{
			datafile << rand() << " ";
		}
		cout << "第" << i << "组数据生成完成" << endl;
		datafile.close();
	}

	cout << "正在生成第10组数据....." << endl;
	datafile.open(Rand_File[9].c_str(), ios::app | ios::in);
	for (int i = AMOUNT; i > 0; i--)
	{
		datafile << i  << " ";
		//cout << t << " ";
	}
	cout << "第10组数据生成完成" << endl;
	datafile.close();
	system("pause");
	system("cls");
}

//判断是否已经有序
bool Is_Order(int a[])
{
	for (int i = 1; i < AMOUNT; i++)
		if (a[i] > a[i + 1]) return false;
	return true;
}

//读取数据到ORIG数组中
void Read_Data(int a[],string filename)
{
	int max = 0;
	fstream file;
	file.open(filename.c_str(), ios::in);
	if (file.fail())
	{
		cout << "文件打开失败!\n";
		exit(0);
	}
	for (int i = 1; i <= AMOUNT; i++)
	{
		file >> a[i];
		//if (a[i] > max) max = a[i];
	}
	//cout << "最大值为" << max << endl;
	file.close();
}

//直接插入排序
void Insert_Sort(int a[])
{
	for (int i = 2; i <= AMOUNT; i++)
	{
		int j;
		a[0] = a[i];
		for (j = i; j > 1; j--)
			if (a[0] < a[j - 1]) a[j] = a[j - 1];
			else break;
		a[j] = a[0];
	}
	//for (int i = 1; i <= AMOUNT; i++)
	//	cout << ORIG[i] << " ";
}

//希尔排序
void Shell_Insert(int a[], int d)
{
	for (int i = d + 1; i <= AMOUNT; i++)
	{
		int j = i;
		if (a[i] < a[i - d])
		{
			a[0] = a[i];
			for (j = i; j > d; j = j - d)
			{
				if (a[0] < a[j - d]) a[j] = a[j - d];
				else break;
			}
			a[j] = a[0];
		}
	}
}
void Shell(int a[], int d[], int t)
{
	for (int k = 0; k < t; k++)
	{
		Shell_Insert(a, d[k]);
	}
}
void Shell_Sort(int a[])
{
	int d[20];
	int cnt = AMOUNT / 2;
	int i = 0;
	while (cnt > 0)
	{
		d[i++] = cnt;
		cnt /= 2;
	}
	Shell(a, d, i);
}

//冒泡排序
void Bubble_Sort(int a[])
{
	for (int i = 1; i < AMOUNT; i++)
	{
		for(int j=1;j<=AMOUNT-i;j++)
			if (a[j] > a[j + 1])
			{
				int temp = a[j];
				a[j] = a[j + 1];
				a[j + 1] = temp;
			}
	}
}

//快速排序
int Partition(int a[], int low, int high)
{
	a[0] = a[low];//作为枢轴
	while (low < high)
	{
		while (low < high && a[high] >= a[0]) high--;
		a[low] = a[high];//将比枢轴小的数移到前面
		while (low < high && a[low] <= a[0]) low++;
		a[high] = a[low];//将比枢轴大的数移到后面
	}
	a[low] = a[0];//将枢轴放到该放的位置
	return low;//返回枢轴
}
void Qsort(int a[], int low, int high)//递归再次快排
{
	if (low < high)
	{
		int p = Partition(a, low, high);//一趟快速排序
		Qsort(a, low, p - 1);//对前部进行快排
		Qsort(a, p + 1, high);//对后部进行快排
	}
}
void QuickSort(int a[])
{
	Qsort(a, 1, AMOUNT);
}

//选择排序
void SelectionSort(int a[])
{
	for (int i = 1; i <= AMOUNT; i++)
	{
		int min = a[i], pos = i;
		for (int j = i; j <= AMOUNT; j++)
			if (a[j] < min)
			{
				min = a[j]; pos = j;
			}
		swap(a[i], a[pos]);
	}
}

//堆排序
void HeapAdjust(int a[], int low, int high)
{
	int temp = a[low];
	int i = low, j = 2 * i;//i记录应插入的位置，j是其左子节点的位置
	while (j <= high)
	{
		if (j + 1 <= high && a[j] < a[j + 1]) j++;//记录子节点较大的位置
		if (temp >= a[j]) break;
		a[i] = a[j]; i = j; j = 2 * i;//循环其子树
	}
	a[i] = temp;
}
void HeapSort(int a[])
{
	for (int i = AMOUNT / 2; i > 0; i--)//第一个非叶子结点为L.length/2
		HeapAdjust(a, i, AMOUNT);
	for (int i = AMOUNT; i > 1; i--)
	{
		int temp = a[1];
		a[1] = a[i]; a[i] = temp;//将根结点移到最后一个结点，再调整堆
		HeapAdjust(a, 1, i - 1);
	}
}

//归并排序
void Merge(int a[], int low, int mid, int high)//合并两个集合
{
	int b[AMOUNT + 1] = { 0 };
	//T.r = (int*)malloc((high - low + 1) * sizeof(int));
	int i = low, j = mid + 1, k = 0;
	while (i <= mid && j <= high)
	{
		if (a[i] <= a[j]) { b[k] = a[i]; i++; k++; }
		else { b[k] = a[j]; j++; k++; }
	}
	while (i <= mid) { b[k] = a[i]; i++; k++; }
	while (j <= high) { b[k] = a[j]; j++; k++; }
	for (int k = 0, i = low; i <= high; k++, i++)
		a[i] = b[k];
}
void Msort(int a[], int len)
{
	int i = 1;
	while (i + 2 * len <= AMOUNT)//有两个集合能够合并
	{
		Merge(a, i, i + len - 1, i + 2 * len - 1);
		i = i + 2 * len;
	}
	if (i + len <= AMOUNT)//有一个len长度加其他长度的集合
		Merge(a, i, i + len - 1, AMOUNT);
}
void MergeSort(int a[])
{
	for (int len = 1; len <= AMOUNT; len *= 2)//分成的集合的元素个数
	{
		Msort(a, len);
	}
}

//基数排序
int GetMax(int a[])
{
	int max = a[1];
	for (int i = 2; i < AMOUNT + 1; i++)
		if (a[i] > max)max = a[i];
	int n=0;
	while (max > 0)
	{
		n++; max /= 10;
	}
	return n;
}
void radixsort(int a[])
{
	queue<int> Q[10];
	int r = GetMax(a);
	int radix = 1;
	int i, j, k;
	int m;
	for (k = 1; k <= r; k++)
	{
		radix *= 10;
		for (i = 1; i < AMOUNT + 1; i++)
		{
			m = (a[i] % radix) / (radix / 10);
			Q[m].push(a[i]);
		}
		for (i = 1, m = 0; m < 10; m++)
		{
			while (!Q[m].empty())
			{
				a[i] = Q[m].front();
				Q[m].pop();
				i++;
			}
		}
	}
}


//获取各个排序花费的时间
void Get_Time()
{
	int* a = new int[AMOUNT + 1];
	
	clock_t start, end;
	double duration;

	cout << "1.直接排序算法:" << endl;
	duration = 0;
	for (int i = 0; i < 10; i++) {
		cout << "\t正在处理第" << i + 1 << "组数据....." << endl;
		Read_Data(a,Rand_File[i]);
		start = clock();
		Insert_Sort(a);
		end = clock();
		duration = (double)end - start;
		if (Is_Order(a)) cout << "\t\t排序完成\n";
		else cout << "\t\t排序失败\n";
		printf("\t第%d组数据处理时间为%.3lf毫秒\n\n", i + 1, duration);
		//cout << "\t第"<<i+1<<"组数据插入时间为:" << duration << " 毫秒" << endl<<endl;
	}
	
	cout << "2.希尔排序算法:" << endl;
	duration = 0;
	for (int i = 0; i < 10; i++) {
		cout << "\t正在处理第" << i + 1 << "组数据....." << endl;
		Read_Data(a, Rand_File[i]);
		start = clock();
		Shell_Sort(a);
		end = clock();
		duration = (double)end - start;
		if (Is_Order(a)) cout << "\t\t排序完成\n";
		else cout << "\t\t排序失败\n";
		printf("\t第%d组数据处理时间为%.3lf毫秒\n\n", i + 1, duration);
		//cout << "\t第" << i + 1 << "组数据处理时间为:" << duration << " 毫秒" << endl << endl;
	}


	cout << "3.冒泡排序算法:" << endl;
	duration = 0;
	for (int i = 0; i < 10; i++) {
		cout << "\t正在处理第" << i + 1 << "组数据....." << endl;
		Read_Data(a, Rand_File[i]);
		start = clock();
		Bubble_Sort(a);
		end = clock();
		duration = (double)end - start;
		if (Is_Order(a)) cout << "\t\t排序完成\n";
		else cout << "\t\t排序失败\n";
		printf("\t第%d组数据处理时间为%.3lf毫秒\n\n", i + 1, duration);
		//cout << "\t第" << i + 1 << "组数据处理时间为:" << duration << " 毫秒" << endl << endl;
	}
	

	cout << "4.快速排序算法:" << endl;
	duration = 0;
	for (int i = 0; i < 10; i++) {
		cout << "\t正在处理第" << i + 1 << "组数据....." << endl;
		Read_Data(a, Rand_File[i]);
		start = clock();
		QuickSort(a);
		end = clock();
		duration = (double)end - start;
		//duration = t_end - t_start;
		if (Is_Order(a)) cout << "\t\t排序完成\n";
		else cout << "\t\t排序失败\n";
		printf("\t第%d组数据处理时间为%.3lf毫秒\n\n", i + 1, duration);
		//cout << "\t第" << i + 1 << "组数据处理时间为:" << duration << " 毫秒" << endl << endl;
	}

	cout << "5.选择排序算法:" << endl;
	duration = 0;
	for (int i = 0; i < 10; i++) {
		cout << "\t正在处理第" << i + 1 << "组数据....." << endl;
		Read_Data(a, Rand_File[i]);
		start = clock();
		SelectionSort(a);
		end = clock();
		duration = (double)end - start;
		//duration = t_end - t_start;
		if (Is_Order(a)) cout << "\t\t排序完成\n";
		else cout << "\t\t排序失败\n";
		printf("\t第%d组数据处理时间为%.3lf毫秒\n\n", i + 1, duration);
		//cout << "\t第" << i + 1 << "组数据处理时间为:" << duration << " 毫秒" << endl << endl;
	}


	cout << "6.堆排序算法:" << endl;
	duration = 0;
	for (int i = 0; i < 10; i++) {
		cout << "\t正在处理第" << i + 1 << "组数据....." << endl;
		Read_Data(a, Rand_File[i]);
		start = clock();
		HeapSort(a);
		end = clock();
		duration = (double)end - start;
		//duration = t_end - t_start;
		if (Is_Order(a)) cout << "\t\t排序完成\n";
		else cout << "\t\t排序失败\n";
		printf("\t第%d组数据处理时间为%.3lf毫秒\n\n", i + 1, duration);
		//cout << "\t第" << i + 1 << "组数据处理时间为:" << duration << " 毫秒" << endl << endl;
	}

	cout << "7.归并排序算法:" << endl;
	duration = 0;
	for (int i = 0; i < 10; i++) {
		cout << "\t正在处理第" << i + 1 << "组数据....." << endl;
		Read_Data(a, Rand_File[i]);
		start = clock();
		MergeSort(a);
		end = clock();
		duration = (double)end - start;
		//duration = t_end - t_start;
		if (Is_Order(a)) cout << "\t\t排序完成\n";
		else cout << "\t\t排序失败\n";
		printf("\t第%d组数据处理时间为%.3lf毫秒\n\n", i + 1, duration);
		//cout << "\t第" << i + 1 << "组数据处理时间为:" << duration << " 毫秒" << endl << endl;
	}

	cout << "8.基数排序算法:" << endl;
	duration = 0;
	for (int i = 0; i < 10; i++) {
		cout << "\t正在处理第" << i + 1 << "组数据....." << endl;
		Read_Data(a, Rand_File[i]);
		start = clock();
		radixsort(a);
		end = clock();
		duration = (double)end - start;
		//duration = t_end - t_start;
		if (Is_Order(a)) cout << "\t\t排序完成\n";
		else cout << "\t\t排序失败\n";
		printf("\t第%d组数据处理时间为%.3lf毫秒\n\n", i + 1, duration);
		//cout << "\t第" << i + 1 << "组数据处理时间为:" << duration << " 毫秒" << endl << endl;
	}	

	delete a;
}

int main()
{
	//GetRand();
	while (1)
	{
		cout << "1.重新生成数据" << endl << "2.计算时间\n3.退出\n";
		cout << "请输入:";
		int cho;
		cin >> cho;
		switch (cho)
		{
		case 1:GetRand(); system("pause"); system("cls"); break;
		case 2:Get_Time(); system("pause"); system("cls"); break;
		case 3:return 0;
		}
		
	}
	return 0;
}