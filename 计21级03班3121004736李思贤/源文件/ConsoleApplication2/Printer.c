#pragma once
#include<stdio.h>
#include<string.h>
#include"Struct.h"
#include"LinkedList.h"
#include"DisplayArray.h"
#include"ConsoleControl.h"
#include"Printer.h"
//日期打印
void print_Date(struct Date date)
{
	printf("%4d.%2d.%2d", date.y, date.m, date.d);
}

//字符转化
void getstr_Date(char* buffer, struct Date date)
{
	sprintf_s(buffer, 11, "%4d.%2d.%2d", date.y, date.m, date.d);
}
void getstr_Date_td(char* buffer, int td)
{
	struct Date date = date_trans("2022.01.01");
	date.td = td;
	update_Date(&date);
	sprintf_s(buffer, 11, "%4d.%2d.%2d", date.y, date.m, date.d);
}
void getstr_int(char* buffer, int amount)
{
	sprintf_s(buffer, 15, "%d", amount);
}
void getstr_Amount(char* buffer, int amount)
{
	float famount = (float)amount / 100;
	sprintf_s(buffer, 15, "%.2f", famount);
}
void getstr_Amount_(char* buffer, int amount)
{
	sprintf_s(buffer, 15, "%d", amount/100);
}
void getstr_Type(char* buffer, int type)
{
	if (type == 食品) strcpy_s(buffer, 15, "食品");
	if (type == 日用品) strcpy_s(buffer, 15, "日用品");
	if (type == 服装)strcpy_s(buffer, 15, "服装");
	if (type == 电器)strcpy_s(buffer, 15, "电器");
	if (type == 水电煤气)strcpy_s(buffer, 15, "水电煤气");
	if (type == 房贷)strcpy_s(buffer, 15, "房贷");
	if (type == 学费)strcpy_s(buffer, 15, "学费");
	if (type == 交通费)strcpy_s(buffer, 15, "交通费");
	if (type == ？)strcpy_s(buffer, 15, "？");
}

//柱状图
void print_bar(struct bar bar, float scale, COORD pos, int termlen)
{
	printc(pos, bar.term);
	pos.X += termlen;
	char temp[30];
	sprintf_s(temp, 30, "%5d", bar.value);
	printc(pos, temp);
	pos.X += 5;
	printc(pos, "|");
	int lenth = bar.value * scale;
	for (int i = 0; i < lenth; i++)
	{
		pos.X++;
		printc(pos, "▧");
	}
}
//行打印
void print_Record(struct Record record, COORD pos, int printform[])
{
	float amount = (float)record.amount / 100;
	char type[15];
	if (record.type == 食品) strcpy_s(type, 15, "食品");
	if (record.type == 日用品) strcpy_s(type, 15, "日用品");
	if (record.type == 服装)strcpy_s(type, 15, "服装");
	if (record.type == 电器)strcpy_s(type, 15, "电器");
	if (record.type == 水电煤气)strcpy_s(type, 15, "水电煤气");
	if (record.type == 房贷)strcpy_s(type, 15, "房贷");
	if (record.type == 学费)strcpy_s(type, 15, "学费");
	if (record.type == 交通费)strcpy_s(type, 15, "交通费");
	if (record.type == ？)strcpy_s(type, 15, "？");
	gotopos(pos);
	print_Date(record.date);
	pos.X += printform[1];
	gotopos(pos);
	printf("%s", record.name);
	pos.X += printform[2];
	gotopos(pos);
	printf("%.2f", amount);
	pos.X += printform[3];
	gotopos(pos);
	printf("%s", type);
	pos.X += printform[4];
	gotopos(pos);
	printf("%s", record.method);
	pos.X += printform[5];
	gotopos(pos);
	printf("%s", record.site);
	pos.X += printform[6];
	gotopos(pos);
	printf("%s", record.detail);
	pos.X += printform[7];
	gotopos(pos);
}
//简略信息打印
void print_miniRecord(struct Record record, COORD pos, int printform[])
{
	float amount = (float)record.amount / 100;
	char type[15];
	if (record.type == 食品) strcpy_s(type, 15, "食品");
	if (record.type == 日用品) strcpy_s(type, 15, "日用品");
	if (record.type == 服装)strcpy_s(type, 15, "服装");
	if (record.type == 电器)strcpy_s(type, 15, "电器");
	if (record.type == 水电煤气)strcpy_s(type, 15, "水电煤气");
	if (record.type == 房贷)strcpy_s(type, 15, "房贷");
	if (record.type == 学费)strcpy_s(type, 15, "学费");
	if (record.type == 交通费)strcpy_s(type, 15, "交通费");
	if (record.type == ？)strcpy_s(type, 15, "？");
	printf("%2d月%2d日", record.date.m, record.date.d);
	pos.X += printform[1] -4;
	gotopos(pos);
	printf("%.2f", amount);
	pos.X += printform[3]-1;
	gotopos(pos);
	printf("%s", type);
	pos.X += printform[4];
	gotopos(pos);
}
//置空
void setblank(COORD pos, int lenth)
{
	DWORD written;
	for (int i = 0; i < lenth; i++)
	{
		WriteConsoleOutputCharacter(handle_out, (char*)" ", 1, pos, &written);
		pos.X++;
	}
}

