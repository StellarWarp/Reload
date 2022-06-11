#include"Search.h"
#include"DisplayArray.h"
#include<string.h>

//初始化
struct Searchinfo Searchinfo_init(char* name)
{
	struct Searchinfo info;
	info.searchmode = 0;
	info.date_low.td = 0;
	info.date_top.td = 0;
	info.amount_low = 0;
	info.amount_top = 0;
	info.searchmode = mode_name;
	strcpy_s(info.name, 15, name);
	return info;
}
//成员中搜索
void __search_Record__(struct Sheet* result, struct RecordList* record, struct Searchinfo info)
{
	struct Record* pos = NULL; 
	if (record) pos = record->C_Record;
	while (pos)
	{
		//date search
		if (info.searchmode & mode_date_top && info.searchmode & mode_date)
		{
			if (!(datecmp(pos->date, info.date_top) <= 0 && datecmp(pos->date, info.date_low) >= 0))
			{
				pos = pos->next;
				continue;
			}

		}
		else if (info.searchmode & mode_date_top)//有上限
		{
			if (!(datecmp(pos->date, info.date_top) <= 0))
			{
				pos = pos->next;
				continue;
			}
		}
		else if (info.searchmode & mode_date)//有下限
		{
			if (!(datecmp(pos->date, info.date_low) >= 0))
			{
				pos = pos->next;
				continue;
			}
		}
		//amount search
		if (info.searchmode & mode_amount_top && info.searchmode & mode_amount)
		{
			if (!(pos->amount >= info.amount_low && pos->amount <= info.amount_top))
			{
				pos = pos->next;
				continue;
			}
		}
		else if (info.searchmode & mode_amount_top)//有上限
		{
			if (!(pos->amount <= info.amount_top))
			{
				pos = pos->next;
				continue;
			}
		}
		else if (info.searchmode & mode_amount)//有下限
		{
			if (!(pos->amount >= info.amount_low))
			{
				pos = pos->next;
				continue;
			}
		}
		//type search
		if (info.searchmode & mode_type)
		{
			if (info.type != pos->type)
			{
				pos = pos->next;
				continue;
			}
		}
		//method search
		if (info.searchmode & mode_method)
		{
			if (strcmp(info.method, pos->method) != 0)
			{
				pos = pos->next;
				continue;
			}
		}
		//site search
		if (info.searchmode & mode_site)
		{
			if (strcmp(info.site, pos->site) != 0)
			{
				pos = pos->next;
				continue;
			}
		}
		//detail search
		if (info.searchmode & mode_detail)
		{
			if (strcmp(info.detail, pos->detail) != 0)
			{
				pos = pos->next;
				continue;
			}
		}
		add_Sheet(result, record, pos);
		pos = pos->next;
	}
}
//成员搜索
void search_Record(struct Sheet* result, struct MemberList memberlist, struct Searchinfo info)
{
	result->num = 0;
	result->currentpos = result->Roll;
	if (info.searchmode & mode_name)
	{
		struct RecordList* record = NULL;
		struct Member* member = memberlist.C_Member;
		while (member)
		{
			if (strcmp(member->name, info.name) == 0)
			{
				record = &member->record;
				break;
			}
			member = member->next;
		}
		__search_Record__(result, record, info);
	}
	else
	{
		struct Member* member = memberlist.C_Member;
		while (member)
		{
			__search_Record__(result, &member->record, info);
			member = member->next;
		}
	}


}