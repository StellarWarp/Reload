#pragma once
#include<stdio.h>
#include"ConsoleControl.h"
#include"DisplayArray.h"
#include"Printer.h"
#include"Input.h"
#include"UserInterface.h"
#include"QuickSort.h"
#include"Search.h"
#include"Struct.h"


struct windowframe
{
	COORD pos1;
	COORD pos2;
	char name[101];
	char top[5];
	char frame_v[5];
	char frame_h[5];
	char corner[5];
	WORD a_top;
	WORD a_frame_v;
	WORD a_frame_h;
	WORD baclground;
};
typedef struct windowframe win_f;

//窗口参数
struct sheetWin
{
	//窗口属性
	win_f frame;
	SMALL_RECT area;
	int capacity;//显示容量
	int top_display;
	int bottom_display;
	int last_selected;
	int state_sort[7];
	//数据
	struct Sheet sheet;
};
#define REFLASH_ALL 1
#define REFLASH_CUR 2

//按钮&信息窗口
struct MsgWin
{
	SMALL_RECT size;
	char title[20];
	char info[101];
	char top[4];
	char frame_v[4];
	char frame_h[4];
	char coner[4];
	WORD name_a;
	WORD baclground;
};
//简化按钮&信息窗口
struct Element
{
	SMALL_RECT area;
	char info[101];
	int infoshift;
	char frame_l[4];
	char frame_r[4];
};
//输入窗口
union indata
{
	int i;
	__int64 i64;
	char s[101];
	struct Date d;
};
struct InputWin
{
	struct Element title;
	struct Element input;
	int input_lenth;
	union indata data;
};
//输入栏
struct InputBox
{
	win_f frame;
	struct InputWin date;
	struct InputWin amount;
	struct InputWin type;
	struct InputWin method;
	struct InputWin site;
	struct InputWin detail;
	struct Element OK;
	struct Record record;
};
struct ConditionBox
{
	win_f frame;
	struct InputWin date_low;
	struct InputWin date_top;
	struct InputWin name;
	struct InputWin amount_low;
	struct InputWin amount_top;
	struct InputWin type;
	struct InputWin method;
	struct InputWin site;
	struct InputWin detail;
	struct Searchinfo Searchinfo;
	struct Member* cmember;
};

struct graphWin
{
	win_f frame;
	SMALL_RECT area;
	int capacity;//显示容量
	int top_display;
	int bottom_display;
	struct barGraph Graph;
	int mode;
	int gap;
	//按钮
	struct InputWin date;
	struct Element name;
	struct InputWin amount;
	struct Element type;
	struct Element method;
	struct Element site;
	struct Element detail;
};

struct layoutFrame
{
	int top;
	int bottom;
	int right;
	int left;
	int lenth;
	int hight;
};

void UI();
