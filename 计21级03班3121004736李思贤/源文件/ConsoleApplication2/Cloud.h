#pragma once
#include"Struct.h"


void WaitClock(int time);

void readfile_Record_all_cloud(struct RecordList* recordlist);

void readfile_Member_cloud(struct MemberList* memberlist);

void getData(struct MemberList* memberlist, struct RecordList* recordlist);

int getRespond();
void waitRespond();

void setRequest(int request);

void setData();
