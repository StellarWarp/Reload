/*
服务响应处理
*/

#include"File.h"
#include"Index.h"
#include"Index_s.h"
#include"StructOP.h"
#include"Filter.h"
#include"Printer.h"
#include"DataManage.h"
#include"IndexManage.h"
#include"Server.h"
#include<time.h>
#include<thread>
using namespace std;

#define checkAcc User_i_.data_find(User_p, operation.info.systemid);\
User user;\
if (User_p.Nempty())\
{\
	userfile.ReadFile_i(User_p.OUT(), user);\
}\
if (strcmp(user.password, operation.info.password) != 0)\
{\
	server.snetRespond(R_N);\
	continue;\
}\


template<class T>
struct keyandp
{
	unsigned long DataIndex;
	T DataKey;
};

typedef  Line<systemID> Search;

template<class T>
void setEmpty(Line<T>& line)
{
	while (line.Nempty())
	{
		line.OUT();
	}
}

int main()
{
	Open();

	IndexManage DataBase;
	DataBase.ReBuild();
	//DataBase.ImportIndex();
	cout << "Index Loaded" << endl;
	Server server;

	Search Family_p;
	Search User_p;
	Search Member_p;
	Search Record_p;

	Record_user_i_.data_find(Record_p, 123);
	Record_user_i.data_find(User_p, 123);


	Line<Member_user> Member;
	Line<Record_user> Record;

	int request = 0;
	for (;;)
	{
		if(request) this_thread::sleep_for(10ms);
		else this_thread::sleep_for(100ms);
		request = server.getRequest();
		UserOP operation;
		//request = 1;//test
		if (request)
		{
			setEmpty(Family_p);
			setEmpty(User_p);
			setEmpty(Member_p);
			setEmpty(Record_p);
			setEmpty(Member);
			setEmpty(Record);

			operation = server.getData();
			//operation.info.systemid = 100;
			//operation.optype == T_SIN;
			if (operation.optype == T_ADD)
			{
				checkAcc;
				operation.data.systemid = recordfile.FileAppends(operation.data);
				recordfile.FileModify(operation.data, operation.data.systemid);
				Record_i_.append(operation.data.systemid, operation.data.systemid);
				Record_user_i_.append(operation.data.systemid, operation.data.membersystemid);
				server.snetRespond(operation.data.systemid);
				cout << "User:  " << user.name << "    " << user.systemid << "  Added Record " << operation.data.systemid << endl;
			}
			if (operation.optype == T_MOD)
			{
				checkAcc
					Record_i_.data_find(Record_p, operation.data.systemid);
				if (Record_p.Nempty())
				{
					recordfile.FileModify(operation.data, Record_p.OUT());
				}
				server.snetRespond(R_SDT);
				cout << "User:  " << user.name << "    " << user.systemid << "  Modified Record " << operation.data.systemid << endl;
			}
			if (operation.optype == T_DEL)
			{
				checkAcc
					Record_i_.data_find(Record_p, operation.data.systemid);
				unsigned long p = 0;
				Record_user record;
				if (Record_p.Nempty())
				{
					p = Record_p.OUT();
					recordfile.ReadFile_i(p, record);
					record.block = 1;
					recordfile.FileModify(record, p);
					recordfile.FileDelete_i(p);
				}
				DataBase.DeleteRecord(p, operation.data.membersystemid, operation.data.systemid);
				server.snetRespond(R_SDT);
				cout << "User:  " << user.name << "    " << user.systemid << "  Deleted Record " << p << endl;
			}
			if (operation.optype == T_SIN)
			{
				User_i_.data_find(User_p, operation.info.systemid);
				User user;
				if (User_p.Nempty())
				{
					userfile.ReadFile_i(User_p.OUT(), user);
				}
				if (strcmp(user.password, operation.info.password) != 0)
				{
					server.snetRespond(R_N);
					continue;
				}
				cout << "User:  " << user.name << "   " << user.systemid << "    Has successfully sign in";
				if (operation.info.familyid != -1)
				{
					cout << "    FamilyID: " << user.familyid << endl;
				}
				if (user.familyid != -1)
				{
					User_family_i_.data_find(User_p, user.familyid);
				}
				else
				{
					User_i_.data_find(User_p, user.systemid);
				}
				while (User_p.Nempty())
				{
					userfile.ReadFile_i(User_p.OUT(), user);
					cout << "FamilyMember : ";
					cout << user.name << "  ";
					cout << endl;
					Member_i_.data_find(Member_p, user.systemid);
					/*if (Member_p.OUT() == user.systemid)
					{
						cout << "S" << endl;
					}
					else
					{
						cout << "F" << endl;
					}*/

				}
				Member_user member;
				while (Member_p.Nempty())
				{
					memberfile.ReadFile_i(Member_p.OUT(), member);
					Member.IN(member);
					Record_user_i_.data_find(Record_p, member.systemid);
					Record_user record;
					while (Record_p.Nempty())
					{
						recordfile.ReadFile_i(Record_p.OUT(), record);
						if (record.block == 0)
						{
							Record.IN(record);
						}
					}
				}
				server.snetRespond(R_SDT);
				server.sentData(Member, Record);

			}
			if (operation.optype == T_CPASS
				|| operation.optype == T_CNAME
				|| operation.optype == T_ESC)
			{
				User_i_.data_find(User_p, operation.info.systemid);
				User user;
				systemID p;
				if (User_p.Nempty())
				{
					p = User_p.OUT();
					userfile.ReadFile_i(p, user);
					if (strcmp(user.password, operation.info.password) != 0)
					{
						server.snetRespond(R_N);
						continue;
					}
					if (operation.optype == T_CPASS)
					{
						strcpy_s(user.password, 7, operation.info.passwordf);
					}
					else if (operation.optype == T_CNAME)
					{
						strcpy_s(user.name,15, operation.info.name);
						Record_user_i_.data_find(Record_p, user.systemid, 0);
						Member_i_.data_find(Member_p, user.systemid, 0);
						while (Record_p.Nempty())
						{
							unsigned long p = Record_p.OUT();
							Record_user record;
							recordfile.ReadFile_i(p, record);
							strcpy_s(record.name, 15, operation.info.name);
							recordfile.FileModify(record, p);
						}
						if (Member_p.Nempty())
						{
							unsigned long p = Member_p.OUT();
							Member_user member;
							memberfile.ReadFile_i(p, member);
							strcpy_s(member.name, 15, operation.info.name);
							memberfile.FileModify(member, p);
						}
					}
					else if (operation.optype == T_ESC)
					{
						user.familyid = -1;//set empty
					}
					cout << "User:  " << user.name << "   " << user.systemid << "    Has changed userinfo" << endl;
					userfile.FileModify(user, p);
					server.snetRespond(R_SDT);
				}
				else
				{
					server.snetRespond(R_N);
					continue;
				}

			}
			if (operation.optype == T_JOIN)
			{
				//检验用户密码
				User_i_.data_find(User_p, operation.info.systemid);
				User user;
				systemID p;//用户数据位置
				if (User_p.Nempty())
				{
					p = User_p.OUT();
					userfile.ReadFile_i(p, user);
					if (strcmp(user.password, operation.info.password) != 0)
					{
						server.snetRespond(R_N);
						continue;
					}
				}
				else { server.snetRespond(R_N); continue; }
				//检验家庭密码
				Family family;
				Family_i_.data_find(Family_p, operation.info.familyid);
				if (Family_p.Nempty())
				{
					familyfile.ReadFile_i(Family_p.OUT(), family);
					if (strcmp(family.password, operation.info.passwordf) != 0)
					{
						server.snetRespond(R_N);
						continue;
					}
				}
				else { server.snetRespond(R_N); continue; }

				User_family_i_.data_delete(p, user.familyid);
				User_family_i_.append(p, operation.info.familyid);
				user.familyid = operation.info.familyid;
				strcpy_s(user.passwordf, 7, operation.info.passwordf);
				userfile.FileModify(user, p);
				server.snetRespond(R_SDT);
			}
		}

	}


	Close();
}


