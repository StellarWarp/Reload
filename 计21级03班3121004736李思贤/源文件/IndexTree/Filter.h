#pragma once
#include"Index.h"
#include"Support.h"
class Fliter
{
	unsigned long* SearchList;
	unsigned long* FlitList;
	typedef unsigned long unit;
	const unsigned int size_unit = sizeof(unit);
	unsigned int space;
	//或运算
	void BitAppend(unsigned long List[], unsigned long value)
	{
		unsigned long in = 1;
		unsigned int i = value % size_unit;
		for (i = value % size_unit; i > 0; i--)
		{
			in = in << 1;
		}
		List[value / size_unit] |= in;
	}
	//与运算
	void BitFlit(unsigned long List1[], unsigned long List2[])
	{
		unsigned long in = 1;
		for (unsigned int i = 0; i < space; i++)
		{
			List1[i] &= List2[i];
		}
		delete[] List2;
		List2 = new unsigned long[space] {0};
	}
public:
	//初始化
	Fliter(int max_num)
	{
		space = max_num % size_unit == 0 ? max_num / size_unit : max_num / size_unit + 1;
		SearchList = new unsigned long[space] {0};
		FlitList = new unsigned long[space] {0};
	}
	//导出
	Line<unsigned long>& Read()
	{
		Line<unsigned long> Result;
		unsigned long* p = SearchList;
		for (int i = 0; i < space; i++)
		{
			if (*p)
			{
				for (int j = 0; *p; j++)
				{
					if (*p % 2)
					{
						Result.IN(i * size_unit + j);
					}
					*p = *p >> 1;
				}
			}
			p++;
		}
		return Result;
	}
	//添加接口
	void Append(Line<unsigned long>& Result)
	{
		while (Result.Nempty())
		{
			BitAppend(SearchList, Result.OUT());
		}
	}
	//筛选接口
	void Flit(Line<unsigned long>& Result)
	{
		while (Result.Nempty())
		{
			BitAppend(FlitList, Result.OUT());
		}
		BitFlit(SearchList, FlitList);
	}
};
