#pragma once
#include"Date.h"
#define LEN_NAME 40
typedef unsigned long systemID;

enum ConsumeType
{
	食品, 日用品, 服装, 电器, 水电煤气, 房贷, 学费, 交通费, ？
};
struct Record
{
	systemID systemid;
	char block;
	systemID membersystemid;
	struct Date date;
	char name[LEN_NAME];
	int amount;//x100
	enum ConsumeType type;
	char method[20];
	char site[30];
	char detail[50];
	struct Record* next;
	struct Record* prev;
};
struct Record initRecord();

//使用前要初始化
struct RecordList
{
	struct Record* C_Record;
	struct Record* E_Record;
	unsigned int len;
};
struct Member
{
	systemID systemid;
	char name[LEN_NAME];
	int MemberSpending;
	struct RecordList record;
	struct Member* next;
	struct Member* prev;
};
struct MemberList
{
	struct Member* C_Member;
	struct Member* E_Member;
	unsigned int len;
};
struct Family
{
	systemID systemid;
	char FamilyName[LEN_NAME];
	struct MemberList menber;
};

struct User
{
	systemID systemid;
	systemID familyid;
	char password[7];
	char passwordf[7];
	char name[LEN_NAME];
};

struct UserOP
{
	int optype;
	struct User info;
	struct Record data;
};

struct User user;
struct UserOP operation;