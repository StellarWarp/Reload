# 我的B+树实现

## 基础逻辑

![image-20220408221843680](C:\Users\Estelle\source\repos\LearnDataStructure\MarkDown\Week4\image-20220408221843680.png)

（勘误：如果修改数据中的值是索引值需要删除并重新添加索引）

具体实现不再赘述

## 内存B+树与外存B+树

### 内存B+树Index类

在内存中建立，可以较快的速度生成索引树，以处理大量的生数据。

建立完成以后通过SaveIndex方法通过迭代的算法将内存中的节点及其指针映射为文件指针，保存到指定位置中

### 外存B+树Index_s类

因为数据量增长后，索引的大小也随之增长，为了应付大量的数据，且为了免去每次启动服务端都要重新生成B+树的工作，需要将索引数据放在外存。

外存B+树不能直接建立，只能从文件中通过Import方法读取

## 文件读写管理fmanage类

为了实现数据频繁的增删操作，需要对文件进行管理

通过对孔的管理实现增删操作（类似于内存管理）

大致功能如下

![image-20220408225958014](C:\Users\Estelle\source\repos\LearnDataStructure\MarkDown\Week4\image-20220408225958014.png)

其主要作用是对储存特定数据的文件进行了封装，将其抽象为数组，并且自动管理增删

也就是说在其它地方（包括B+树中）**文件指针指的是数据在文件中的编号**

## 同值兼容

在处理数据时（比如年龄的索引）不可避免地会有使用相同索引值的数据，为此需要进行同值的区分

我用过使用指针值对同值索引进行区分，因为不同数据的指针必然不相同。

为此在非子叶节点中也会存储数据指针，但是这些指针不用于直接获取数据，而是在遇到有相同索引时加以区分先后

内存节点

```cpp
	struct LayerData//子叶节点
	{
		DT key[max_key] = {};
		unsigned long DataPointer[max_key] = {};
		struct LayerData* next = NULL;
		int num = 0;
	};
	struct LayerN
	{
		DT key[max_key] = {};
		unsigned long DataPointer[max_key] = {};//用于辨识
		Pointer NodePointer[M] = {};
		int num = 0;
	};
```

外存节点

```cpp
	typedef unsigned long Pointer;
	//结构
	struct LayerData//子叶节点
	{
		DT key[max_key] = {};
		Pointer DataPointer[max_key] = {};
		Pointer next = {};
		int num = 0;
	};
	struct LayerN
	{
		DT key[max_key] = {};
		Pointer DataPointer[max_key] = {};//用于辨识
		Pointer NodePointer[M] = {};
		int num = 0;
	};
```

此外为了对两组数据进行查找需要对二分查找进行改进

```cpp
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
```

```cpp
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

```

```cpp
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
```

## 基础查找与统计

查找方法包括精确查找data_find（迭代实现）与范围查找data_rangefind（循环实现），均需要一个队列以储存结果。

统计方法包括（范围）精确值统计Satistic与（范围）等差值统计Satistic_range

### 筛选工具Fliter类

用于高效查找两组结果中相同的或者不同的项，用数字中的位表示该数据的有无，位的位置对应数据在文件中的位置，通过位运算实现筛选。

虽然这看上去挺浪费内存的，但是换算下来1GB的内存可以容纳8^9级别的数据量，但是对于比较的结果较少的数据而言，这种算法有点浪费

```cpp
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
```

## 其它

内存树中子节点与非子节点类型不同，方便操作，可定义一联合体

```cpp
union Pointer//节点间指针
{
LayerData* Dp;
LayerN* Np = NULL;
};
```

为了进行可视化的Debug设计了VisualTree将B+树的架构可视化方便找bug

### 迭代器

遍历子叶节点

```cpp
	//迭代器//用于遍历
	class iterator : public std::iterator<std::forward_iterator_tag, DT>
	{
	public:
		LayerData* ptr;
		int index;
		struct node
		{
			DT indexer;
			unsigned long DataPointer;
		};
		iterator(LayerData* p = nullptr, int index = 0)
		{
			ptr = p;
			this->index = index;
		}
		node operator*()
		{
			return node{ ptr->key[index] , ptr->DataPointer[index]};
		}
		bool operator !=(const iterator& iter)
		{
			return ptr != iter.ptr;
		}
		iterator& operator++()
		{
			index++;
			if (ptr->num <= index)
			{
				ptr = ptr->next;
				index = 0;
			}
			return *this;
		}
		iterator& operator++(int)
		{
			iterator temp = *this;
			index++;
			if (ptr->num <= index)
			{
				ptr = ptr->next;
				index = 0;
			}
			return temp;
		}
	};
	iterator begin()
	{
		Pointer p = *Master;
		int h = H;

		while (h > 1)
		{
			p = p.Np->NodePointer[0];
			h--;
		}
		return iterator(p.Dp);
	}
	iterator end()
	{
		return iterator();
	}
```



## 测试

已对内存B+树与外存B+树进行了10^8级的随机数据的随机增删（修改为删除并从新添加索引）查找，完全删除（内存树）的测试和正确性核对，暂时没有发现问题
