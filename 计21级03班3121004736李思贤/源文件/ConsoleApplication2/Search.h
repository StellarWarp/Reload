#pragma once
#include"LinkedList.h"
#include"MODE.h"

struct Searchinfo
{
	unsigned int searchmode;
	struct Date date_low;
	struct Date date_top;
	char name[15];
	int amount_low;//x100
	int amount_top;
	enum ConsumeType type;
	char method[20];
	char site[30];
	char detail[50];
};
struct Searchinfo Searchinfo_init(char* name);
void __search_Record__(struct Sheet* result, struct RecordList* record, struct Searchinfo info);
void search_Record(struct Sheet* result, struct MemberList memberlist, struct Searchinfo info);