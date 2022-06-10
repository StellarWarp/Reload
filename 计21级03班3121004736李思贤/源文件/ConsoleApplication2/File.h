#pragma once
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"LinkedList.h"
#include"Printer.h"


//获取和储存本地用户信息
void readfile_getinfo(struct User* user, int* state)
{
	FILE* infile = NULL;
	if (fopen_s(&infile, "Data/user.info", "rb"))
	{
		printf("Fail to open user.info\n");
		exit(-1);
	}
	fread(user, sizeof(struct User), 1, infile);
	fread(state, sizeof(int), 1, infile);
	fclose(infile);
}
void writefile_userinfo(struct User* user, int* state)
{
	FILE* out = NULL;
	if (fopen_s(&out, "Data/user.info", "wb"))
	{
		printf("Fail to open user.info\n");
		exit(-1);
	}
	fwrite(user, sizeof(struct User), 1, out);
	fwrite(state, sizeof(int), 1, out);
	fclose(out);
}
//记录读写
void wirtefile_Record(struct RecordList recordlist)
{
	struct Record* record = recordlist.C_Record;
	FILE* outfile = NULL;
	if (fopen_s(&outfile, "Data/fee.dat", "wb"))
	{
		printf("Fail to open fee.dat\n");
		exit(-1);
	}
	while (record)
	{
		fwrite(record, sizeof(struct Record), 1, outfile);
		record = record->next;
	}
	fclose(outfile);
}
void wirtefile_Record_all(struct MemberList memberlist)
{
	struct Member* member = memberlist.C_Member;
	FILE* outfile = NULL;
	if (fopen_s(&outfile, "Data/fee.dat", "wb"))
	{
		printf("Fail to open fee.dat\n");
		exit(-1);
	}
	while (member)
	{
		struct Record* record = member->record.C_Record;
		while (record)
		{
			fwrite(record, sizeof(struct Record), 1, outfile);
			record = record->next;
		}
		member = member->next;
	}
	fclose(outfile);
}
void readfile_Record_all(struct RecordList* recordlist)
{
	struct Record record;
	FILE* infile = NULL;
	if (fopen_s(&infile, "Data/fee.dat", "rb"))
	{
		printf("Fail to open fee.dat\n");
		exit(-1);
	}
	fread(&record, sizeof(struct Record), 1, infile);
	while (!feof(infile))
	{
		in_Record(recordlist, recordlist->E_Record, record);
		fread(&record, sizeof(struct Record), 1, infile);
	}
	fclose(infile);
}

//记录导入
struct Member* search_member(struct MemberList memberlist, systemID id)
{
	struct Member* member = memberlist.C_Member;
	while (member)
	{
		if (member->systemid == id)
		{
			return member;
		}
		member = member->next;
	}
	return NULL;
}

void import_to_member(struct RecordList* source, struct MemberList memberlist)
{
	struct Record* record = source->C_Record;
	int num = 0;
	while (record)
	{
		struct Member* member = search_member(memberlist, record->membersystemid);
		if (!member)
		{
			record = record->next;
			continue;
		}
		member->record = *source;
		while (record && member->systemid == record->membersystemid)
		{
			record = record->next;
			num++;
		}
		//???
		if (record)
		{
			record->prev->next = NULL;
			member->record.E_Record = record->prev;
			member->record.len += num;
			source->len -= num;
			num = 0;
			source->C_Record = record;
			record->prev = NULL;
		}
	}
}
//成员读写
void readfile_Member(struct MemberList* memberlist)
{
	struct Member member;
	FILE* infile = NULL;
	if (fopen_s(&infile, "Data/member.bin", "rb"))
	{
		printf("Fail to open member.bin\n");
		exit(-1);
	}
	fread(&member, sizeof(struct Member), 1, infile);
	while (!feof(infile))
	{
		member.record = initRecordList();
		in_Member(memberlist, memberlist->E_Member, member);
		fread(&member, sizeof(struct Member), 1, infile);
	}
	fclose(infile);
}
void wirtefile_Member(struct MemberList memberlist)
{
	struct Member* member = memberlist.C_Member;
	FILE* outfile = NULL;
	if (fopen_s(&outfile, "Data/member.bin", "wb"))
	{
		printf("Fail to open member.bin\n");
		exit(-1);
	}
	while (member)
	{
		fwrite(member, sizeof(struct Member), 1, outfile);
		member = member->next;
	}
	fclose(outfile);
}
//导出
void exportreocrd(struct Record* record,int reset)
{
	static int firstwite = 1;
	if (reset == 1)
	{
		firstwite = 1;
		return;
	}
	FILE* outfile = NULL;
	if (firstwite == 1)
	{
		fopen_s(&outfile, "Export/record.txt", "w");
		firstwite = 0;
	}
	else
	{
		fopen_s(&outfile, "Export/record.txt", "a");
	}
	char date[11] = { 0 };
	getstr_Date(date, record->date);
	char type[20] = { 0 };
	getstr_Type(type, record->type);
	char amount[20] = { 0 };
	getstr_Amount(amount, record->amount);
	fprintf(outfile," %s	%s	%s	%s	%s	%s	%s\n",
		date, record->name, amount, type,
		record->method, record->site, record->detail);
	fclose(outfile);
}



