#pragma once
#include<iostream>
#include<cmath>
using namespace std;
template<int lenth>
class fstring
{
public:
	char str_in[lenth];
	fstring(){};
	fstring(string& s)
	{
		strcpy_s(str_in, s.c_str());
	}
	fstring(const char* s)
	{
		strcpy_s(str_in, s);
	}
	bool inline operator >(const fstring& str_out)
	{
		if (strcmp(str_in, str_out.str_in) > 0)
		{
			return true;
		}
		else return false;
		
	}
	bool inline operator >=(const fstring& str_out)
	{
		if (strcmp(str_in, str_out.str_in) >= 0)
		{
			return true;
		}
		else return false;

	}
	bool inline operator < (const fstring& str_out)
	{
		if (strcmp(str_in, str_out.str_in) < 0)
		{
			return true;
		}
		else return false;
	}
	bool inline operator <= (const fstring & str_out)
	{
		if (strcmp(str_in, str_out.str_in) <= 0)
		{
			return true;
		}
		else return false;
	}
	bool inline operator ==(const fstring& str_out)
	{
		if (strcmp(str_in, str_out.str_in) == 0)
		{
			return true;
		}
		else return false;
	}

	void inline operator = (string & s)
	{
		strcpy_s(str_in, s.c_str());
	}
	void inline operator =(const char* s)
	{
		strcpy_s(str_in, s);
	}
};

class Date
{
public:

	int y=1970;
	int m=1;
	int d=0;
	int td=0;

	Date() {};
	Date(string t)
	{
		int year = 0;
		int month = 0;
		int day = 0;
		int tday = 0;
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
		y = year;
		m = month;
		d = day;
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
		td = tday;

	}
	void updata()
	{
		int tday = td;
		//计算年
		int year = 1970;
		int yearday = 365;
		while (tday>yearday)
		{
			tday -= yearday;
			year++;
			if (year % 400 == 0 || year % 100 != 0 && year % 4 == 0) yearday = 366;
			else yearday = 365;
		}
		y = year;
		//计算月
		int month[12] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
		if (year % 400 == 0 || year % 100 != 0 && year % 4 == 0) month[1] = 29; else month[1] = 28;
		int i = 0;
		while (tday> month[i])
		{
			tday -= month[i];
			i++;
		}
		m = i+1;
		//计算日
		d = tday;
	}
	Date operator =(string t)
	{
		int year = 0;
		int month = 0;
		int day = 0;
		int tday = 0;
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
		y = year;
		m = month;
		d = day;
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
		td = tday;

		return *this;
	}
	Date operator +(const Date& t)
	{
		Date rt = *this;
		rt.td += t.td;
		rt.updata();
		return rt;
	}
	Date operator +(const int& t)
	{
		Date rt = *this;
		rt.td += t;
		rt.updata();
		return rt;
	}
	Date operator -(const Date& t)
	{
		Date rt = *this;
		rt.td -= t.td;
		rt.updata();
		return rt;
	}
	Date operator -(const int& t)
	{
		Date rt = *this;
		rt.td -= t;
		rt.updata();
		return rt;
	}
	void operator +=(const Date& t)
	{
		td += t.td;
		updata();
	}
	void operator +=(const int& t)
	{
		td += t;
		updata();
	}
	void operator -=(const Date& t)
	{
		td -= t.td;
		updata();
	}
	void operator -=(const int& t)
	{
		td -= t;
		updata();
	}
	bool operator >(const Date& t)
	{
		if (td > t.td) return true;
		else return false;
	}
	bool operator >=(const Date& t)
	{
		if (td >= t.td) return true;
		else return false;
	}
	bool operator <(const Date& t)
	{
		if (td < t.td) return true;
		else return false;
	}
	bool operator <=(const Date& t)
	{
		if (td <= t.td) return true;
		else return false;
	}
	bool operator ==(const Date& t)
	{
		if (td == t.td) return true;
		else return false;
	}
	friend ostream& operator << (ostream& output, Date& D)
	{
		output << D.y << '.';
		output.width(2);
		output << D.m << '.';
		output.width(2);
		output << D.d;
		return output;
	}
};

