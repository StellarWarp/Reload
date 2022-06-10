#pragma once
#include<math.h>
#include"Date.h"
//初始化
void initDate(struct Date* date)
{
	date->y = 1970;
	date->m = 1;
	date->d = 0;
	date->td = 0;
}
//绝对日期转换
void update_Date(struct Date* date)
{
	int tday = date->td;
	//计算年
	int year = 1970;
	int yearday = 365;
	while (tday > yearday)
	{
		tday -= yearday;
		year++;
		if (year % 400 == 0 || year % 100 != 0 && year % 4 == 0) yearday = 366;
		else yearday = 365;
	}
	date->y = year;
	//计算月
	int month[12] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
	if (year % 400 == 0 || year % 100 != 0 && year % 4 == 0) month[1] = 29; else month[1] = 28;
	int i = 0;
	while (tday > month[i])
	{
		tday -= month[i];
		i++;
	}
	date->m = i + 1;
	//计算日
	date->d = tday;
}
void up_td(struct Date* date)
{
	int year = date->y;
	int month = date->m;
	int day = date->d;
	int tday = 0;
	int month_l[12] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
	if (year % 400 == 0 || year % 100 != 0 && year % 4 == 0) month_l[1] = 29; else month_l[1] = 28;
	for (int i = 0; i < month - 1; i++)
	{
		tday += month_l[i];
	}
	tday += day;
	//年
	int yearday;
	for (int cyear = 1970; cyear < year; cyear++)
	{
		if (cyear % 400 == 0 || cyear % 100 != 0 && cyear % 4 == 0) yearday = 366; else yearday = 365;
		tday += yearday;
	}
	date->td = tday;
}
//字符转日期
struct Date date_trans(char* t)
{
	int year = 0;
	int month = 0;
	int day = 0;
	int tday = 0;
	struct Date date;
	//转化
	for (int i = 0, p = 3; i < 4; i++)
	{
		year += (t[i] - '0') * pow(10, p);
		p--;
	}
	for (int i = 5, p = 1; i < 7; i++)
	{
		month += (t[i] - '0') * pow(10, p);
		p--;
	}
	for (int i = 8, p = 1; i < 10; i++)
	{
		day += (t[i] - '0') * pow(10, p);
		p--;
	}
	date.y = year;
	date.m = month;
	date.d = day;
	//计算td
	//月、日
	int month_l[12] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
	if (year % 400 == 0 || year % 100 != 0 && year % 4 == 0) month_l[1] = 29; else month_l[1] = 28;
	for (int i = 0; i < month - 1; i++)
	{
		tday += month_l[i];
	}
	tday += day;
	//年
	int yearday;
	for (int cyear = 1970; cyear < year; cyear++)
	{
		if (cyear % 400 == 0 || cyear % 100 != 0 && cyear % 4 == 0) yearday = 366; else yearday = 365;
		tday += yearday;
	}
	date.td = tday;

	return date;
}
//日期加减运算
struct Date add_Date(const struct Date t1, const int t2)
{
	struct Date rt;
	rt.td = t1.td + t2;
	update_Date(&rt);
	return rt;
}
struct Date mine_Date(const struct Date t1, const struct Date t2)
{
	struct Date rt;
	rt.td = t1.td - t2.td;
	update_Date(&rt);
	return rt;
}
//日期比较
int datecmp(const struct Date t1, const struct Date t2)
{
	return t1.td - t2.td;
}
//获取今天日期
struct Date today()
{
	struct Date date;
	time_t now;
	time(&now);
	struct tm time;
	localtime_s(&time, &now);
	date.y = time.tm_year + 1900;
	date.m = time.tm_mon + 1;
	date.d = time.tm_mday;
	up_td(&date);
	return date;
}


