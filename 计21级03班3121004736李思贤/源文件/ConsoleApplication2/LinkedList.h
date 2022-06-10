#pragma once
#include"Struct.h"
struct RecordList initRecordList();
struct MemberList initMemberList();

void in_Record(struct RecordList* recordlist, struct Record* p_record, struct Record record);
void dele_Record(struct RecordList* recordlist, struct Record* record);
void modify_Record(struct Record* precord, struct Record record);

void in_Member(struct MemberList* recordlist, struct Member* p_record, struct Member record);
void dele_Member(struct MemberList* recordlist, struct Member* record);