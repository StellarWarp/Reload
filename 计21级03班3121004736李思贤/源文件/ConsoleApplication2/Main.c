#include"UserInterface.h"
#include"DataImportor.h"

int main()
{
	/*user.familyid = 0;
	user.systemid = 0;
	int accstate = 1;
	strcpy_s(user.password, 7, "000000");
	strcpy_s(user.passwordf, 7, "000000");
	writefile_getinfo(&user, &accstate);*/
	int accstate = 0;
	readfile_getinfo(&user, &accstate);
	//accstate = 0;
	InitializeUI();
	memberlist_global = initMemberList();
	recordlist_global = initRecordList();
	if (accstate == 0)
	{
		SigninPage();
	}
	else
	{
		ImportData();
	}

	UI();

	wirtefile_Record_all(memberlist_global);
	wirtefile_Member(memberlist_global);

}
