#include<stdio.h>
#include<string.h>
#include"QuickSort.h"
#include"DisplayArray.h"

//交换行
void swap(struct Roll* a, struct Roll* b)
{
	struct Roll temp = *a;
	*a = *b;
	*b = temp;
}
//比较函数
int cmp_int(union T i1, union T i2)
{
	return i1.i - i2.i;
}
int cmp_str(union T s1, union T s2)
{
	return strcmp(s1.s, s2.s);
}
//访问函数
union T get_date(struct Roll info)
{
	union T data;
	data.i = info.record->date.td;
	return data;
}
union T get_name(struct Roll info)
{
	union T data;
	data.s = info.record->name;
	return data;
}
union T get_amount(struct Roll info)
{
	union T data;
	data.i = info.record->amount;
	return data;
}
union T get_type(struct Roll info)
{
	union T data;
	data.i = info.record->type;
	return data;
}
union T get_method(struct Roll info)
{
	union T data;
	data.s = info.record->method;
	return data;
}
union T get_site(struct Roll info)
{
	union T data;
	data.s = info.record->site;
	return data;
}
union T get_detail(struct Roll info)
{
	union T data;
	data.s = info.record->detail;
	return data;
}

//快速排序
int Paritition(struct Roll array[], int a, int b, union T (*visit)(struct Roll), int (*cmp)(union T, union T))//下界与上界
{
	//a,b作为引索
	union T pivot = visit(array[a]);
	while (a < b)
	{
		while (a < b && cmp(pivot, visit(array[b])) <= 0)
		{
			b--;//后大
		}
		if (a != b) swap(&array[a], &array[b]);
		while (a < b && cmp(pivot, visit(array[a])) >= 0)
		{
			a++;//前小
		}
		if (a != b) swap(&array[a], &array[b]);
	}
	return a;
}
void QuickSort(struct Roll array[], int low, int high, union T(*visit)(struct Roll), int (*cmp)(union T, union T))
{
	if (low < high)
	{
		int pivot = Paritition(array, low, high, visit, cmp);
		QuickSort(array, low, pivot - 1, visit, cmp);
		QuickSort(array, pivot + 1, high, visit, cmp);
	}
}
//逆序
void Reverse(struct Roll array[], int low, int high)
{
	struct Roll* p1 = array + low;
	struct Roll* p2 = array + high;
	int num = (high - low + 1) / 2;
	for (int i = 0; i < num; i++)
	{
		swap(p1, p2);
		p1++;
		p2--;
	}
}
