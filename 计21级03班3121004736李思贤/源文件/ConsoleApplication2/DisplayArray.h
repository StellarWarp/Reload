#pragma once
#include<stdbool.h>
#include<math.h>
#include"LinkedList.h"
struct Roll
{
	struct Record* record;
	struct RecordList* recordlist;
	bool select;
};
struct Sheet
{
	struct Roll* Roll;
	//数据信息
	int num;
	int len;
	struct Roll* currentpos;
}; 
struct Sheet Sheet_init();
void add_Sheet(struct Sheet* sheet, struct RecordList* recordlist, struct Record* record);
void dele_Sheet(struct Sheet* sheet, int i);

struct bar
{
	char term[51];
	int value;
};
struct barGraph
{
	char title[51];
	struct bar* bars;
	int gap;
	int len;
	int num;
	float scale;
};
struct barGraph initGraph();
void set_barGraph(struct barGraph* graph, struct Sheet sheet, int mode, int gap, int WindowLenth);