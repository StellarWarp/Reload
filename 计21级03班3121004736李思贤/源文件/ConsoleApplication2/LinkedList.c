#include<stdio.h>
#include<malloc.h>
#include<stdbool.h>
#include"LinkedList.h"
#include"MODE.h"

extern void setRequest(int request);
extern void setData();

//结构体初始化
struct RecordList initRecordList()
{
	struct RecordList recordlist;
	recordlist.C_Record = NULL;
	recordlist.E_Record = NULL;
	recordlist.len = 0;
	return recordlist;
}
struct MemberList initMemberList()
{
	struct MemberList memberlist;
	memberlist.C_Member = NULL;
	memberlist.E_Member = NULL;
	memberlist.len = 0;
	return memberlist;
}

//记录操作
void in_Record(struct RecordList* recordlist, struct Record* p_record, struct Record record)
{
	struct Record* newnode = (struct Record*)malloc(sizeof(struct Record));
	*newnode = record;

	if (recordlist->C_Record)
	{
		//链表插入
		newnode->next = p_record->next;
		if (p_record->next)
			p_record->next->prev = newnode;
		else
			recordlist->E_Record = newnode;
		p_record->next = newnode;
		newnode->prev = p_record;
		recordlist->len++;
	}
	else
	{
		//创建第一节点
		newnode->next = NULL;
		newnode->prev = NULL;
		recordlist->C_Record = recordlist->E_Record = newnode;
		recordlist->len = 1;
	}
}
void dele_Record(struct RecordList* recordlist, struct Record* record)
{
	if (record == recordlist->C_Record)
	{
		recordlist->C_Record = record->next;
	}
	if (record == recordlist->E_Record)
	{
		recordlist->E_Record = record->prev;
	}
	if (record->prev)
		record->prev->next = record->next;
	if (record->next)
		record->next->prev = record->prev;
	recordlist->len--;
	free(record);
}
void modify_Record(struct Record* precord, struct Record record)
{
	*precord = record;
	setRequest(1);
	operation.optype = T_MOD;
	operation.data = record;
	setData();
}

//void swap_Record(struct Record** h, struct Record** a, struct Record** b)
//{
//	if ((*b)->next) (*b)->next->prev = (*a);
//	struct Record** temp = (*a)->next->prev;
//	(*b)->prev->next = (*a);
//	*temp = (*b);
//	if (a == *h) *h = (*b);
//	else (*a)->prev->next = (*b);
//	swap((*a)->next, (*b)->next);
//	swap((*a)->prev, (*b)->prev);
//	swap((*a), (*b));
//}

//成员操作
void in_Member(struct MemberList* recordlist, struct Member* p_record, struct Member record)
{
	struct Member* newnode = (struct Member*)malloc(sizeof(struct Member));
	*newnode = record;

	if (recordlist->C_Member)
	{
		//链表插入
		newnode->next = p_record->next;
		if (p_record->next)
			p_record->next->prev = newnode;
		else
			recordlist->E_Member = newnode;
		p_record->next = newnode;
		newnode->prev = p_record;
		recordlist->len++;
	}
	else
	{
		//创建第一节点
		newnode->next = NULL;
		newnode->prev = NULL;
		recordlist->C_Member = recordlist->E_Member = newnode;
		recordlist->len = 1;
	}
}
void dele_Member(struct MemberList* recordlist, struct Member* record)
{
	if (record == recordlist->C_Member)
	{
		recordlist->C_Member = record->next;
	}
	if (record == recordlist->E_Member)
	{
		recordlist->E_Member = record->prev;
	}
	if (record->prev)
		record->prev->next = record->next;
	if (record->next)
		record->next->prev = record->prev;
	recordlist->len--;
	free(record);
}
void modify_Member(struct Member* precord, struct Member record)
{
	*precord = record;
}

//void swap_Member(struct Member** h, struct Member** a, struct Member** b)
//{
//	if ((*b)->next) (*b)->next->prev = (*a);
//	struct Member** temp = (*a)->next->prev;
//	(*b)->prev->next = (*a);
//	*temp = (*b);
//	if (a == *h) *h = (*b);
//	else (*a)->prev->next = (*b);
//	swap((*a)->next, (*b)->next);
//	swap((*a)->prev, (*b)->prev);
//	swap((*a), (*b));
//}
//使用前要初始化
struct displayArray_Member
{
	struct Member** Array;
	int num;
	int len;
	struct Member** currentpos;
};
