#include"UserInterface.h"
#include"SupportForUI.h"

#include"UI-CodeCollections.h"

#include"1-UI.h"
#include"2-UI.h"

#include"sheetWin.h"
#include"GraphWin.h"
#include"InputBox.h"
#include"ConditionBox.h"

//布局控件
struct layoutFrame LayoutFrame_f_init(int x, int y, int lenth, int hight)
{
	struct layoutFrame l;
	l.top = y;
	l.left = x;
	l.bottom = y + hight - 1;
	l.right = x + lenth - 1;
	l.lenth = lenth;
	l.hight = hight;
	return l;
}
struct layoutFrame LayoutFrame_r_init(int x1, int y1, int x2, int y2)
{
	struct layoutFrame l;
	l.top = y1;
	l.left = x1;
	int lenth = x2 - x1 + 1;
	int hight = y2 - y1 + 1;
	l.bottom = y2;
	l.right = x2;
	l.lenth = lenth;
	l.hight = hight;
	return l;
}

#define mode_MainPage 1
#define mode_RecordPage 2
#define mode_Resign 3
#define mode_ChangeInfo 8
#define mode_ChangePassword 4
#define mode_ChangeName 5
#define mode_ChangeFamily 6
#define mode_Exit 7

#include"FunctionPage.h"
#include"Pages.h"

//总控制
void UI()
{
INTI:
	{
		struct MemberList memberlist = memberlist_global;
		struct Member* member = member_global;

		struct Sheet sheet1 = Sheet_init();

		//INPUT_RECORD mouserec;      //定义输入事件结构体  
		//DWORD res;      //用于存储读取记录  
		//COORD pos;      //用于存储鼠标当前位置  
		int page = mode_MainPage;

		for (;;)
		{
			COORD size = csbi.dwSize;
			struct MsgWin 主页 = MsgWin_init(size.X / 2 - 30, size.Y - 2, size.X / 2 - 10, size.Y + 1);
			strcpy_s(主页.info, 20, "Main Page");
			主页.name_a = FORE_WHITE;
			MsgWin_frame(主页);
			struct MsgWin 记录页 = MsgWin_init(size.X / 2 + 10, size.Y - 2, size.X / 2 + 30, size.Y + 1);
			strcpy_s(记录页.info, 20, "Record Page");
			记录页.name_a = FORE_WHITE;
			MsgWin_frame(记录页);
			struct MsgWin 退出 = MsgWin_init(size.X / 2 - 9, size.Y - 2, size.X / 2 + 9, size.Y + 1);
			strcpy_s(退出.info, 20, "Save & Exit");
			退出.name_a = FORE_WHITE;
			MsgWin_frame(退出);

			if (page == mode_RecordPage)
			{
				struct layoutFrame recordFrame = LayoutFrame_r_init(2, 2, size.X, size.Y - 5);

				int printform_s[8] = { 6,14,10,10,10,10,10,10 };
				for (int i = 0; i < 8; i++)
				{
					printform[i] = printform_s[i] * (float)recordFrame.lenth / 84;
				}

				struct Sheet sheet = Sheet_init();
				search_Record(&sheet, memberlist, Searchinfo_init(member->name));

				struct sheetWin 表格窗口 = SheetWin_init(recordFrame.left, 7, size.X - 1, size.Y - 9, sheet);
				window_close(表格窗口.frame);
				struct graphWin 图像窗口 = GraphWin_init(recordFrame.left, 7, size.X - 2, size.Y - 15, "Graph", sheet);
				window_close(图像窗口.frame);

				struct MsgWin 信息窗口 = MsgWin_init(recordFrame.right - 30, recordFrame.bottom - 2, recordFrame.right, recordFrame.bottom);
				strcpy_s(信息窗口.info, 20, "Messeage");
				信息窗口.name_a = FORE_WHITE;
				MsgWin_frame(信息窗口);

				struct MsgWin 删除键 = MsgWin_init(2, recordFrame.bottom - 2, 18, recordFrame.bottom);
				strcpy_s(删除键.info, 20, "Delete");
				删除键.name_a = FORE_WHITE;
				MsgWin_frame(删除键);

				struct MsgWin 切换键 = MsgWin_init(20, recordFrame.bottom - 2, 36, recordFrame.bottom);
				strcpy_s(切换键.info, 20, "Show In Graph");
				切换键.name_a = FORE_WHITE;
				MsgWin_frame(切换键);

				struct MsgWin 导出键 = MsgWin_init(38, recordFrame.bottom - 2, 54, recordFrame.bottom);
				strcpy_s(导出键.info, 20, "Export");
				导出键.name_a = FORE_WHITE;
				MsgWin_frame(导出键);

				struct ConditionBox 条件控制窗口 = ConditionBox_init(recordFrame.left, recordFrame.top, 84, 4, "筛选", member);

				search_Record(&sheet, memberlist, Searchinfo_init(member->name));
				window_frame(表格窗口.frame);
				sheetWin_reflash(&表格窗口, REFLASH_ALL);
				RecordPage(&page, &主页, &退出,
					&表格窗口, &图像窗口,
					&删除键, &切换键, &导出键,
					&信息窗口, &条件控制窗口,
					memberlist, member);
			}
			else if (page == mode_MainPage)
			{
				struct layoutFrame mainFrame = LayoutFrame_f_init((size.X - 82) / 2, 2, 86, size.Y - 5);
				struct MsgWin 信息窗口 = MsgWin_init(mainFrame.left, mainFrame.bottom - 2, mainFrame.left + 84, mainFrame.bottom);
				strcpy_s(信息窗口.info, 20, "Messeage");
				信息窗口.name_a = FORE_WHITE;
				if (servermsg_global == 0)
				{
					strcpy_s(信息窗口.info, 101, "Server Time Out , Resign to reconnect server");
				}
				else if (servermsg_global == R_N)
				{
					strcpy_s(信息窗口.info, 20, "Server Refuse");
				}
				if (servermsg_global == R_SDT)
				{
					strcpy_s(信息窗口.info, 20, "Data Updated");
				}
				MsgWin_frame(信息窗口);

				int l_recordwin = 30;
				struct sheetWin 最近记录 = minisheetWin_init
				(mainFrame.left, mainFrame.top,
					mainFrame.left + l_recordwin, mainFrame.bottom - 10, sheet1);

				window_frame(最近记录.frame);
				minisheetWin_reflash(&最近记录, member->record);

				struct InputBox 输入框 = InputBox_init(mainFrame.left, mainFrame.bottom - 7, 84, 4, "记录");

				int acc_operation_centerx = (size.X - (mainFrame.left + l_recordwin)) / 2 + mainFrame.left + l_recordwin;
				int acc_operation_centery = (mainFrame.top + mainFrame.bottom - 7) / 2;
				int bottom_lenth = 25;
				int acc_operation_edge = acc_operation_centerx - bottom_lenth / 2;
				struct layoutFrame AccLayout = LayoutFrame_r_init
				(acc_operation_edge, mainFrame.top + 2,
					acc_operation_edge + bottom_lenth, acc_operation_centery + 4);

				struct MsgWin userinfo = MsgWin_init(mainFrame.left + 46, 6, mainFrame.left + 71, 10);
				strcpy_s(userinfo.info, 20, "Hi ! ");
				strcat_s(userinfo.info, 30, member->name);
				userinfo.coner[0] = '*';
				userinfo.frame_v[0] = ' ';
				userinfo.name_a = FORE_WHITE;
				MsgWin_frame(userinfo);

				struct MsgWin 登出 = MsgWin_init(mainFrame.left + 46, 12, mainFrame.left + 71, 14);
				strcpy_s(登出.info, 20, "Sign out");
				登出.coner[0] = '*';
				登出.name_a = FORE_WHITE;
				MsgWin_frame(登出);

				struct MsgWin 修改密码 = MsgWin_init(mainFrame.left + 46, 16, mainFrame.left + 71, 18);
				strcpy_s(修改密码.info, 20, "Change Information");
				修改密码.coner[0] = '*';
				修改密码.name_a = FORE_WHITE;
				MsgWin_frame(修改密码);

				struct MsgWin 改变家庭 = MsgWin_init(mainFrame.left + 46, 20, mainFrame.left + 71, 22);
				strcpy_s(改变家庭.info, 20, "Reset Family");
				改变家庭.coner[0] = '*';
				改变家庭.name_a = FORE_WHITE;
				MsgWin_frame(改变家庭);

				MainPage(&page, &记录页,
					&退出,
					&最近记录, &输入框,
					&信息窗口,
					member,
					&登出, &修改密码, &改变家庭);

				MsgWin_close(userinfo);
				MsgWin_close(登出);
				MsgWin_close(修改密码);
				MsgWin_close(改变家庭);

				if (page == mode_Resign)
				{
					MsgWin_close(主页);
					MsgWin_close(记录页);
					MsgWin_close(退出);
					SigninPage();
					goto INTI;
				}
				else if (page == mode_ChangeInfo)
				{
					MsgWin_close(主页);
					MsgWin_close(记录页);
					MsgWin_close(退出);
					ChangeInfo(&page);
					if (page == mode_ChangePassword ||page == mode_ChangeName)
					{
						SigninPage();
						goto INTI;
					}
				}
				else if (page == mode_ChangeFamily)
				{
					MsgWin_close(主页);
					MsgWin_close(记录页);
					MsgWin_close(退出);
					if (ChangeFamily())
					{
						SigninPage();
						goto INTI;
					}
					else
					{
						page = mode_MainPage;
					}
				}

			}
			else if (page == mode_Exit) break;

		}
	}
}