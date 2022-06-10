#pragma once
#include<stdio.h>
#include<time.h>
extern void ImportData();
extern void setRequest(int request);
extern void setData();
extern void getData(struct MemberList* memberlist, struct RecordList* recordlist);
extern int getRespond();
extern void waitRespond();
extern struct Member* member_global;
extern struct MemberList memberlist_global;
extern struct RecordList recordlist_global;
extern int servermsg_global;
extern void readfile_getinfo(struct User* user, int* state);
extern	void writefile_userinfo(struct User* user, int* state);
extern struct Member* search_member(struct MemberList memberlist, systemID id);
extern void import_to_member(struct RecordList* source, struct MemberList memberlist);
extern void readfile_Record_all(struct RecordList* recordlist);
extern void readfile_Member(struct MemberList* memberlist);
extern void exportreocrd(struct Record* record, int reset);
extern int serverOff_global;