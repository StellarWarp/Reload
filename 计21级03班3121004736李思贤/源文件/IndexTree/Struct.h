#ifndef STRUCT_H
#define STRUCT_H
#include<iostream>
#include"TypeSupport.h"
#include"Define.h"
using namespace std;

const int LEN_NAME = 40;
typedef unsigned long systemID;
enum ConsumeType
{
	食品, 日用品, 服装, 电器, 水电煤气, 房贷, 学费, 交通费
};
struct Record_user
{
	systemID systemid;
	char block = 0;
	systemID membersystemid;
	struct Date date;
	char name[LEN_NAME];
	int amount;//x100
	enum ConsumeType type;
	char method[20];
	char site[30];
	char detail[50];
	struct Record_user* next;
	struct Record_user* prev;
};
struct RecordList_user
{
	struct Record_user* C_Record;
	struct Record_user* E_Record;
	unsigned int len;
};
struct Member_user
{
	systemID systemid;
	char name[LEN_NAME];
	int MemberSpending;
	struct RecordList_user record;
	struct Member* next;
	struct Member* prev;
};
struct Family
{
	systemID systemid;
	char password[7];
	char FamilyName[LEN_NAME];
};

struct User
{
	systemID systemid = -1;
	systemID familyid = -1;
	char password[7] = "000000";
	char passwordf[7];
	char name[LEN_NAME];
};




template<class DT>
struct Index_LayerData
{
	DT key[M-1] = {};
	unsigned long DataPointer[M-1] = {};
	unsigned long next = {};
	int num = 0;
};

template<class DT>
struct Index_LayerN
{
	DT key[M - 1] = {};
	unsigned long DataPointer[M - 1] = {};//用于辨识
	unsigned long NodePointer[M] = {};
	int num = 0;
};


#endif // !STRUCT_H

