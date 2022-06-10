#include"Cloud.h"
#include"File.h"

//#define ServerPath "..\\IndexTree\\IndexTree\\DataExchange"
//
//#define RequestPath "\\toServer\\request.bin"
//#define RespondPath "\\toUser\\respond.bin"
//
//#define SentPath "\\toServer\\data.bin"
//#define GetPath1 "\\toUser\\member.bin"
//#define GetPath2 "\\toUser\\fee.dat"

#define ServerPath "../IndexTree/DataExchange"

#define RequestPath "/toServer/request.bin"
#define RespondPath "/toUser/respond.bin"

#define SentPath "/toServer/data.bin"
#define GetPath1 "/toUser/member.bin"
#define GetPath2 "/toUser/fee.dat"

#define _Path(path1,path2) path1##path2
#define Path(path1,path2) _Path(path1,path2)

FILE* Request = NULL;
FILE* Userop = NULL;
FILE* Respond = NULL;

int trytime = 0;
int serverOff_global = 0;
int servermsg_global;

//等待
void WaitClock(int time)
{
	clock_t start, now, gap;
	now = start = clock();
	do
	{
		now = clock();
		gap = now - start;
	} while (gap >= time);
}
//读取服务端数据
void readfile_Record_all_cloud(struct RecordList* recordlist)
{
	struct Record record;
	FILE* infile = NULL;

	if (fopen_s(&infile, Path(ServerPath , GetPath2), "rb"))
	{
		/*printf("Fail to open fee.dat\n");
		exit(-1);*/
	}
	fread(&record, sizeof(struct Record), 1, infile);
	while (!feof(infile))
	{
		in_Record(recordlist, recordlist->E_Record, record);
		fread(&record, sizeof(struct Record), 1, infile);
	}
	fclose(infile);
}
void readfile_Member_cloud(struct MemberList* memberlist)
{
	struct Member member;
	FILE* infile = NULL;
	if (fopen_s(&infile, Path(ServerPath, GetPath1), "rb"))
	{
		/*printf("Fail to open member.bin\n");
		exit(-1);*/
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
//整合调用
void getData(struct MemberList* memberlist, struct RecordList* recordlist)
{
	readfile_Member_cloud(memberlist);
	readfile_Record_all_cloud(recordlist);
}

//获取响应
int getRespond()
{
	int respond = 0;
	if (!serverOff_global)
	{
		if (fopen_s(&Respond, Path(ServerPath, RespondPath), "rb"))
		{
			WaitClock(10);
			printf("Fail to open respond.bin\n");
		}
		else
		{
			fread(&respond, sizeof(int), 1, Respond);
			fclose(Respond);
		}

		int respond_ = 0;
		if (fopen_s(&Respond, Path(ServerPath, RespondPath), "wb"))
		{
			/*printf("Fail to open respond.bin\n");*/
		}
		else
		{
			fwrite(&respond_, sizeof(int), 1, Respond);
			fclose(Respond);
		}
	}

	return respond;
}
//等待响应
void waitRespond()
{
	clock_t start, now, gap;
	now = start = clock();
	int respond = 0;
	for (; serverOff_global==0;)
	{
		now = clock();
		gap = now - start;
		if (gap > 1000)
		{
			trytime++;
			if (trytime > 5) serverOff_global = 1;
			break;
		}
		if (fopen_s(&Respond, Path(ServerPath, RespondPath), "rb"))
		{
			//printf("Fail to open respond.bin\n");
			WaitClock(10);
		}
		else
		{
			fread(&respond, sizeof(int), 1, Respond);
			fclose(Respond);
			if (respond != 0)
			{
				break;
				trytime = 0;
			}
		}
	}
}
//发送请求
void setRequest(int request)
{
	clock_t start, now, gap;
	now = start = clock();
	for (;;)
	{
		if (fopen_s(&Request, Path(ServerPath,RequestPath), "wb"))
		{
			//printf("Fail to open request.bin\n");
			WaitClock(100);
			now = clock();
			gap = now - start;
			if (gap > 1000) break;
		}
		else
		{
			fwrite(&request, sizeof(struct User), 1, Request);
			fclose(Request);
			break;
		}
	}
}
//发送数据
void setData()
{
	if (fopen_s(&Userop, Path(ServerPath, SentPath), "wb"))
	{
		/*printf("Fail to open data.bin\n");*/
	}
	else
	{
		fwrite(&operation, sizeof(struct UserOP), 1, Userop);
		fclose(Userop);
	}
}

