#ifndef SUPPORT_H
#define SUPPORT_H
#include<iostream>
using namespace std;

template<class DT>
class Stack
{
	struct block
	{
		DT num;
		block* next;

		block(DT n, block* p)
		{
			num = n;
			next = p;
		}
	};
	block* C = NULL;
	DT* ReadLog;

public:
	void IN(DT n)
	{
		if (C)
		{
			C = new block(n, C);
		}
		else
		{
			C = new block(n, NULL);
		}
	}
	DT OUT()
	{
		if (C)
		{
			DT n = C->num;
			block* D = C;
			C = C->next;
			free(D);
			return n;
		}
		else
		{
			cout << "栈空" << endl;
			exit(1);
		}
	}
	DT Read0()
	{
		return C->num;
	}
	//返回创建的数组
	DT* Read(int n)
	{
		ReadLog = new DT[n];
		block* P = C;
		for (int i = 0; i < n; i++)
		{
			ReadLog[i] = P->num;
			P = P->next;
		}
		return ReadLog;
	}
	void ReleasesReadLog()
	{
		delete[]ReadLog;
	}

	bool Nempty()
	{
		return C;
	}
};
template<class DT>
class Line
{
	struct block
	{
		DT num;
		block* next;

		block(DT n, block* p)
		{
			num = n;
			next = p;
		}
	};
	block* C = NULL;
	block* E = NULL;

public:
	void IN(DT n)
	{
		if (E)
		{
			E->next = new block(n, NULL);
			E = E->next;
		}
		else
		{
			C = E = new block(n, NULL);
		}
	}
	DT OUT()
	{
		if (C)
		{
			DT n = C->num;//要返回的值
			block* D = C;
			C = C->next;
			if (!C)
			{
				E = NULL;
			}
			free(D);
			return n;
		}
		else
		{
			cout << "列队空" << endl;
			exit(1);
		}
	}
	bool Nempty()
	{
		return C;
	}
};
//数组操作
template<class DT>//从前开始
void inline MoveArray(DT a[], int alow, int atop, DT b[], int blow = 0)
{
	DT* ap = a + alow;
	DT* bp = b + blow;
	for (int i = alow; i <= atop; i++)
	{
		*bp = *ap;
		ap++;
		bp++;
	}
}
template<class DT>//从后开始
void inline MoveArray_b(DT a[], int alow, int atop, DT b[], int btop)
{
	DT* ap = a + atop;
	DT* bp = b + btop;
	for (int i = atop; i >= alow; i--)
	{
		*bp = *ap;
		ap--;
		bp--;
	}
}
template<class DT>
void inline SetArray(DT a[],int low, int top, DT x)
{
	DT* p = a + low;
	for (int i = low; i <= top; i++)
	{
		*p = x;
		p++;
	}
}
template<class DT>
void inline InsertArray(DT a[], int instert, int top, DT value)
{
	DT* p1 = a + top - 1;
	DT* p2 = a + top;
	for (int i = top-1; i >= instert; i--)
	{
		*p2 = *p1;
		p2--;
		p1--;
	}
	*p2 = value;
}
template<class DT>
void inline DeleteArray(DT a[], int dele, int top, DT evalue)
{
	if (dele == top)
	{
		a[top] = evalue;
	}
	else
	{
		DT* p1 = a + dele;
		DT* p2 = a + dele + 1;
		for (int i = dele; i < top; i++)
		{
			*p1 = *p2;
			p1++;
			p2++;
		}
		*p1 = evalue;
	}
}
//快速排序
template<class DT>
DT Paritition(DT array[], int a, int b)//下界与上界
{
	//a,b作为引索
	DT pivot = array[a];
	while (a < b)
	{
		while (a < b && pivot <= array[b])
		{
			b--;//后大
		}
		if (a != b) swap(array[a], array[b]);
		while (a < b && pivot >= array[a])
		{
			a++;//前小
		}
		if (a != b) swap(array[a], array[b]);
	}
	return a;
}
template<class DT>
void quicksort(DT array[], int low, int high)
{
	if (low < high)
	{
		DT pivot = Paritition(array, low, high);
		quicksort(array, low, pivot - 1);
		quicksort(array, pivot + 1, high);
	}
}

//二分法数组查找
template<class DT>//精准搜索，若无报错
int DilitarySearch_p(DT array[], int low, int high, DT value)
{
	int p;//搜索位置
	DT cvalue;
	int a = low;
	int b = high;

	while (1)
	{
		p = (a + b) / 2;
		cvalue = array[p];

		if (a == b && cvalue != value)
		{
			cout << "DilitarySearch_p: NotFound!" << endl;
			exit(-1);
		}
		if (cvalue > value) b = p - 1;
		else if (cvalue < value) a = p + 1;
		else if (cvalue == value) return p;
	}
}
//<=无用
template<class DT>
int DilitarySearch_r_l(DT array[], int low, int high, DT value)
{
	int p;//搜索位置
	DT cvalue;
	int a = low;
	int b = high;

	while (1)
	{
		p = (a + b) / 2;
		cvalue = array[p];

		if (a == b) return a;
		else if (cvalue < value) a = p + 1;
		else if (cvalue > value) b = p - 1;
		else if (cvalue == value) return p;
	}
}
//>=无用
template<class DT>
int DilitarySearch_r_s(DT array[], int low, int high, DT value)
{
	int p;//搜索位置
	DT cvalue;
	int a = low;
	int b = high;

	while (1)
	{
		p = (a + b) / 2;
		cvalue = array[p];

		if (cvalue == value) return p;
		if (a == b) return a-1;
		else if (cvalue < value) a = p + 1;
		else if (cvalue > value) b = p - 1;
	}
}
template<class DT>//当value大于最大值时，返回最大index
int inline DilitarySearch_down(DT array[], int low, int high, DT value)
{
	if (value > array[high])
	{
		return high;
	}
	if (value <= array[low])
	{
		return low;
	}
	int p;//搜索位置
	DT cvalue;
	int a = low;
	int b = high;

	while (1)
	{
		p = (a + b) / 2;
		cvalue = array[p];

		if (a == p) return p + 1;
		else if (cvalue < value) a = p;
		else if (cvalue >= value) b = p;
	}
}
template<class DT>//当value大于最大值时，返回最大index
int inline DilitarySearch_up(DT array[], int low, int high, DT value)
{
	if (value > array[high])
	{
		return high;
	}
	if (value == array[high])
	{
		return high;
	}
	if (value < array[low])
	{
		return low;
	}
	int p;//搜索位置
	DT cvalue;
	int a = low;
	int b = high;

	while (1)
	{
		p = (a + b) / 2;
		cvalue = array[p];

		if (a == p) return a;
		else if (cvalue <= value) a = p;
		else if (cvalue > value) b = p;
	}
}
template<class DT>//范围搜索
int inline DilitarySearch_range(DT array[], int low, int high, DT low_value, DT high_value, int& ia, int& ib)
{
	if (high_value > array[high])
	{
		ia = ++high;
		ib = high;
		return 0;
	}
	if (low_value < array[low])
	{
		ia = low;
		ib = low;
		return 0;
	}
	int p;//搜索位置
	DT cvalue;
	int a = low;
	int b = high;
	while (1)
	{
		p = (a + b) / 2;
		cvalue = array[p];

		if (a > b)
		{
			ia = a;
			ib = a;
			return 0;
		}
		if (cvalue < low_value) a = p + 1;
		else if (cvalue > high_value) b = p - 1;
		else
		{
			ia = DilitarySearch_down(array, a, p, low_value);
			ib = DilitarySearch_up(array, p, b, high_value);
			break;
		}
	}
}

template<class T1, class T2>//精确搜索//兼容同值搜索//无同值不核对//1是关键词数组，2是辅助搜索数组
int inline DilitarySearch_SecondOrder_p(T1 array1[], T2 array2[], int low, int high, T1 value1, T2 value2)
{
	int a, b;
	DilitarySearch_range(array1, low, high, value1, value1, a, b);
	if (a == b)return a;
	else if (a < b)
	{
		return DilitarySearch_p(array2, a, b, value2);
	}
	else if (a > b)
	{
		cout << "DilitarySearch_SecondOrder_p: NotFound!" << endl;
		exit (1);
	}

}

template<class T1, class T2>//返回下限//兼容同值搜索//无同值不核对//1是关键词数组，2是辅助搜索数组
int inline DilitarySearch_SecondOrder_down(T1 array1[], T2 array2[], int low, int high, T1 value1, T2 value2)
{
	int a, b;
	if (high == -1)return low;
	DilitarySearch_range(array1, low, high, value1, value1, a, b);
	if (a >= b)return a;
	else if (a < b)
	{
		if (value2 == -1)return a;
		T2 (*test)[10] = (T2(*)[10] )&array2;
		return DilitarySearch_down(array2, a, b, value2);
	}

}

#endif // !SUPPORT_H





