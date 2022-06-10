#pragma once
#include<iostream>
#include <fstream>
#include"Struct.h"
#include"StructOP.h"
#include"Support.h"
using namespace std;
#define T_ADD 1
#define T_MOD 2
#define T_DEL 3
#define T_SIN 4
#define T_CPASS 5
#define T_CNAME 6
#define T_JOIN 7
#define T_ESC 8

#define R_N 1
#define R_SDT 2

struct UserOP
{
	int optype = 0;
	User info;
	Record_user data;
};

class Server
{
	fstream Request;
	fstream Userop;
	ofstream Respond;
	ofstream outfile1;
	ofstream outfile2;
public:
	void sentData(Line<Member_user>& Member, Line<Record_user>& Record)
	{
		outfile1.open("DataExchange/toUser/member.bin", ios::binary);
		outfile2.open("DataExchange/toUser/fee.dat", ios::binary);
		while (Member.Nempty())
		{
			Member_user member = Member.OUT();
			outfile1.write((char*)&(member), sizeof(Member_user));
		}
		while (Record.Nempty())
		{
			Record_user record = Record.OUT();
			outfile2.write((char*)&(record), sizeof(Record_user));
		}
		outfile1.close();
		outfile2.close();
	}
	void snetRespond(int respond = 0)
	{
		Respond.open("DataExchange/toUser/respond.bin", ios::binary);
		Respond.write((char*)&(respond), sizeof(int));
		Respond.close();
	}
	int getRequest()
	{
		int request = 0;
		clock_t start, now, gap;
		now = start = clock();
		for (;;)
		{
			Request.open("DataExchange/toServer/request.bin", ios::binary | ios::in | ios::out);
			if (!Request.is_open())
			{
				cout << "Fail to open " << "DataExchange/toServer/request.bin" << endl;
				now = clock();
				gap = now - start;
				if (gap > 1000) break;
			}
			else break;
		}
		Request.seekg(0);
		Request.read((char*)&request, sizeof(int));
		int request_ = 0;
		Request.seekp(0);
		Request.write((char*)&request_, sizeof(int));
		Request.close();
		return request;
	}
	UserOP getData()
	{
		UserOP userop;
		for (;;)
		{
			Userop.open("DataExchange/toServer/data.bin", ios::binary | ios::in);
			if (!Userop.is_open())
			{
				cout << "Fail to open " << "DataExchange/toServer/data.bin" << endl;
			}
			else
			{
				Userop.read((char*)&userop, sizeof(UserOP));
				Userop.close();
				break;
			}
		}
		return userop;
	}

};

