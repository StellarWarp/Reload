#pragma once

extern void setRequest(int request);
extern void setData();
extern void getData(struct MemberList* memberlist, struct RecordList* recordlist);
extern int getRespond();
extern struct Member* search_member(struct MemberList memberlist, systemID id);
extern void import_to_member(struct RecordList* source, struct MemberList memberlist);
extern void readfile_Record_all(struct RecordList* recordlist);
extern void readfile_Member(struct MemberList* memberlist);
extern void readfile_getinfo(struct User* user, int* state);
extern void writefile_userinfo(struct User* user, int* state);
extern void wirtefile_Record_all(struct MemberList memberlist);
extern void wirtefile_Member(struct MemberList memberlist);
extern void SigninPage();
extern int serverOff_global;
extern int servermsg_global;

struct MemberList memberlist_global;
struct RecordList recordlist_global;
struct Member* member_global;



//导入数据
void ImportData()
{
	while (memberlist_global.C_Member != NULL) dele_Member(&memberlist_global, memberlist_global.C_Member);
	while (recordlist_global.C_Record != NULL) dele_Record(&recordlist_global, recordlist_global.C_Record);
	
	operation.info = user;
	setRequest(1);
	operation.optype = T_SIN;
	setData();

	time_t now;
	time_t startime;
	time_t lastime;
	time(&now);
	startime = lastime = now;
	for (;;)
	{
		time(&now);
		if (now - lastime > 1)
		{
			servermsg_global = getRespond();
			if (servermsg_global != 0)
			{
				break;
			}
			lastime = now;
		}
		if (now - startime > 5)
		{
			serverOff_global = 1;
			break;
		}
	}
	//servermsg_global = getRespond();
	if (servermsg_global == R_SDT)
	{
		getData(&memberlist_global, &recordlist_global);
	}
	else if (servermsg_global == R_N)
	{
		SigninPage();
		goto END;
	}
	else if (servermsg_global == 0 || memberlist_global.C_Member == NULL)
	{
		readfile_Member(&memberlist_global);
		readfile_Record_all(&recordlist_global);
	}

	//初始化
	import_to_member(&recordlist_global, memberlist_global);

	member_global = NULL;
	member_global = search_member(memberlist_global, user.systemid);
	if (member_global == NULL) exit(-4);
	END:{}
}
