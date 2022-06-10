#include "DisplayArray.h"
#include<stdio.h>
#include<malloc.h>
#include"MODE.h"
#include"QuickSort.h"
#include"Printer.h"
//表格初始化
struct Sheet Sheet_init()
{
	struct Sheet array;
	array.Roll = NULL;
	array.num = 0;
	array.currentpos = NULL;
	array.len = 0;
	return array;
}
void inline DeleteEelemp(struct Roll a[], int dele, int top)
{
	struct Roll* p1 = a + dele;
	struct Roll* p2 = a + dele + 1;
	for (int i = dele; i < top; i++)
	{
		*p1 = *p2;
		p1++;
		p2++;
	}
}
//添加行（动态结构体数组）
void add_Sheet(struct Sheet* sheetArray, struct RecordList* recordlist, struct Record* record)
{
	int lenth = sheetArray->len;
	int input_lenth = sheetArray->num;
	struct Roll* currentpos = sheetArray->currentpos;
	if (lenth)
	{
		currentpos->record = record;
		currentpos->recordlist = recordlist;
		currentpos->select = false;
		//struct Roll(*test)[20] = (struct Roll(*)[20])sheetArray->Roll;//测试
		input_lenth++;
		currentpos++;
		if (input_lenth == lenth)
		{
			sheetArray->Roll = (struct Roll*)realloc(sheetArray->Roll, (lenth *= 2) * sizeof(struct Roll));
			currentpos = sheetArray->Roll + input_lenth;
		}
		sheetArray->len = lenth;
		sheetArray->num++;
		sheetArray->currentpos = currentpos;
	}
	else
	{
		sheetArray->len = 4;
		sheetArray->currentpos = sheetArray->Roll = (struct Roll*)calloc(4, sizeof(struct Roll));
		sheetArray->Roll->record = record;
		sheetArray->Roll->recordlist = recordlist;
		sheetArray->Roll->select = false;
		sheetArray->num = 1;
		sheetArray->currentpos++;
	}
}
//删除行
void dele_Sheet(struct Sheet* sheetArray, int i)
{
	DeleteEelemp(sheetArray->Roll, i, sheetArray->num);
	sheetArray->currentpos--;
	sheetArray->num--;
}

//二分法数组查找
//int DilitarySearch_p_c(struct bar array[], int low, int high, char* value, int* out)
//{
//	int p;//搜索位置
//	char* cvalue;
//	int a = low;
//	int b = high;
//
//	while (1)
//	{
//		p = (a + b) / 2;
//		cvalue = array[p].term;
//
//		if (a == b && strcmp(cvalue, value) != 0)
//		{
//			if (strcmp(cvalue, value) > 0)
//			{
//				*out = p;
//			}
//			else
//			{
//				*out = p + 1;
//			}
//			return 1;
//		}
//		if (strcmp(cvalue, value) > 0) b = p - 1;
//		else if (strcmp(cvalue, value) < 0) a = p + 1;
//		else if (strcmp(cvalue, value) == 0)
//		{
//			*out = p;
//			return 0;
//		}
//	}
//}

//柱状图初始化
struct bar initbar(char* term, int value)
{
	struct bar newbar;
	strcpy_s(newbar.term, 51, term);
	newbar.value = value;
	return newbar;
}
struct barGraph initGraph()
{
	struct barGraph graph;
	graph.bars = NULL;
	graph.gap = 0;
	graph.len = 0;
	graph.num = 0;
	graph.title[0] = 0;
	graph.scale = 1;
	return graph;
}
//插入
void inline Insertbar(struct bar a[], int instert, int top, struct bar value)
{
	struct bar* p1 = a + top - 1;
	struct bar* p2 = a + top;
	for (int i = top - 1; i >= instert; i--)
	{
		*p2 = *p1;
		p2--;
		p1--;
	}
	*p2 = value;
}
void cpy(char* buffer, char* from)
{
	strcpy_s(buffer, 30, from);
}
//生成柱状图（动态结构体数组）
void set_barGraph(struct barGraph* graph, struct Sheet sheet, int mode, int gap , int WindowLenth)
{
	free(graph->bars);
	graph->bars = calloc(4, sizeof(struct bar));
	graph->len = 4;
	graph->num = 0;
	int maxnum = 0;
	struct Roll* array = sheet.Roll;
	if (sheet.num == 0) return;
	union T(*getData)(struct Roll info) = NULL;
	char* (*getname)() = NULL;
	if (mode & mode_date)
	{
		getname = getstr_Date_td;
		getData = get_date;
	}
	else if (mode & mode_name)
	{
		getname = cpy;
		getData = get_name;
	}
	else if (mode & mode_amount)
	{
		getname = getstr_Amount;
		getData = get_amount;
	}
	else if (mode & mode_type)
	{
		getname = getstr_Type;
		getData = get_type;
	}
	else if (mode & mode_method)
	{
		getname = cpy;
		getData = get_method;
	}
	else if (mode & mode_site)
	{
		getname = cpy;
		getData = get_site;
	}
	else if (mode & mode_detail)
	{
		getname = cpy;
		getData = get_detail;
	}

	if (mode & mode_date)
	{
		QuickSort(sheet.Roll, 0, sheet.num - 1, get_date, cmp_int);
	}
	else if (mode & mode_name)
	{
		QuickSort(sheet.Roll, 0, sheet.num - 1, get_name, cmp_str);
	}
	else if (mode & mode_amount)
	{
		QuickSort(sheet.Roll, 0, sheet.num - 1, get_amount, cmp_int);
	}
	else if (mode & mode_type)
	{
		QuickSort(sheet.Roll, 0, sheet.num - 1, get_type, cmp_int);
	}
	else if (mode & mode_method)
	{
		QuickSort(sheet.Roll, 0, sheet.num - 1, get_method, cmp_str);
	}
	else if (mode & mode_site)
	{
		QuickSort(sheet.Roll, 0, sheet.num - 1, get_site, cmp_str);
	}
	else if (mode & mode_detail)
	{
		QuickSort(sheet.Roll, 0, sheet.num - 1, get_detail, cmp_str);
	}
	char term[51]; 
	char Cterm[51];
	getname(term, getData(array[0]));
	cpy(Cterm, term);
	graph->bars[0] = initbar(term, 1);
	if (mode & mode_precise)
	{
		int p = 0;
		for (int i = 1; i < sheet.num; i++)
		{
			getname(term, getData(array[i]));
			if (strcmp(term, Cterm) == 0)//一样
			{
				graph->bars[p].value++;
			}
			else
			{
				graph->bars[++p] = initbar(term, 1);
				cpy(Cterm, term);
				graph->num = p + 1;
				if (graph->num == graph->len)
				{
					graph->bars = (struct bar*)realloc(graph->bars, (graph->len *= 2) * sizeof(struct bar));
				}
			}
		}
		if (!p)
		{
			graph->num = 1;
		}
	}
	else if (mode & mode_range)
	{
		int CValue = getData(array[0]).i;
		CValue -= CValue % gap;
		getname(graph->bars[0].term, CValue);
		int StepValue = CValue + gap;
		int p = 0;
		for (int i = 1, count = 0;i < sheet.num; i++, count++)
		{
			int CValue = getData(array[i]).i;
			if (CValue <= StepValue)
			{
				graph->bars[p].value++;
			}
			else
			{
				while (CValue > StepValue)
				{
					StepValue += gap;
					getname(term, StepValue - gap);
					graph->bars[++p] = initbar(term, 0);
					graph->num = p + 1;
					if (graph->num == graph->len)
					{
						graph->bars = (struct bar*)realloc(graph->bars, (graph->len *= 2) * sizeof(struct bar));
					}
				}
				graph->bars[p].value++;
			}
		}
		if (!p)
		{
			graph->num = 1;
		}
	}

	for (int i = 0; i < graph->num; i++)
	{
		if (graph->bars[i].value > maxnum)
		{
			maxnum = graph->bars[i].value;
		}
	}
	if (maxnum)
	{
		
		graph->scale = (float)(WindowLenth - 17) / maxnum;
	}
	else
	{
		graph->scale = 1;
	}
}



