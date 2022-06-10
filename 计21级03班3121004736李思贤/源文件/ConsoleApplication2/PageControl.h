#pragma once
#define mode_MainPage 1
#define mode_RecordPage 2
#define mode_Resign 3
#define mode_ChangeInfo 8
#define mode_ChangePassword 4
#define mode_ChangeName 5
#define mode_ChangeFamily 6
#define mode_Exit 7

//更改密码
void ChangeInfo(int* mode)
{
	INPUT_RECORD mouserec;      //定义输入事件结构体  
	DWORD res;      //用于存储读取记录  
	COORD pos;      //用于存储鼠标当前位置  
	COORD size = csbi.dwSize;
	struct msgWin PasswordOption = initmsgWin(size.X / 2 - 25, size.Y / 2 - 3, size.X / 2 - 4, size.Y / 2 + 3);
	struct msgWin NameOption = initmsgWin(size.X / 2 + 4, size.Y / 2 - 3, size.X / 2 + 25, size.Y / 2 + 3);
	strcpy_s(PasswordOption.info, 20, "Change PassWord");
	strcpy_s(NameOption.info, 20, "Change Name");
	PasswordOption.name_a = FORE_WHITE;
	NameOption.name_a = FORE_WHITE;
	frame_MsgWin(PasswordOption);
	frame_MsgWin(NameOption);

	for (;;)
	{
		ReadConsoleInput(handle_in, &mouserec, 1, &res);      //读取输入事件  
		COORD windowsize = csbi.dwSize;
		pos = mouserec.Event.MouseEvent.dwMousePosition;    //获得当前鼠标位置
		GetConsoleScreenBufferInfo(handle_out, &csbi);
		gotoxy(0, 0);  //设置鼠标位置  
		DisplayMousePosition(pos);      //显示鼠标位置  
		if (ispress_msgwin(mouserec, &PasswordOption))
		{
			*mode = mode_ChangePassword;
			break;
		}
		else if (ispress_msgwin(mouserec, &NameOption))
		{
			*mode = mode_ChangeName;
			break;
		}
		else if (mouserec.Event.KeyEvent.wVirtualKeyCode == VK_ESCAPE)
		{
			*mode = mode_MainPage;
			break;
		}
	}
	close_MegWin(PasswordOption);
	close_MegWin(NameOption);
REIN:
	{
		if (*mode == mode_MainPage)
		{
			goto END;
		}
		struct windowframe acc = initwindow(size.X / 2 - 15, size.Y / 2 - 3, size.X / 2 + 20, size.Y / 2 + 3);

		if (*mode == mode_ChangePassword) strcpy_s(acc.name, 20, "Change-PassWord");
		if (*mode == mode_ChangeName) strcpy_s(acc.name, 20, "Change-Name");

		acc.corner[0] = '*';
		frame_window(acc);
		char* title = NULL;
		if (*mode == mode_ChangePassword) title = "新密码";
		if (*mode == mode_ChangeName) title = "新名称";
		struct inWin  Password = initinWin(size.X / 2 - 5, size.Y / 2 - 1, 8, 8, " 密码 ");
		int ilen;
		if (*mode == mode_ChangePassword) ilen = 8;
		if (*mode == mode_ChangeName) ilen = 12;
		struct inWin  Info = initinWin(size.X / 2 - 5, size.Y / 2 + 1, 8, ilen, title);
		char OldPassword[7] = { 0 };
		char NewInfo[LEN_NAME] = { 0 };
		//输入
		{
			int i = 0;
			char in;
			int len1 = 6;
			int len2 = 10;
			if (*mode == mode_ChangePassword) len2 = 6;
			if (*mode == mode_ChangeName) len2 = 40;
			while (1)
			{
				in = _getch();

				if (in == '\r')
				{
					printf("\b");
					update_Element(&Password.input);
					update_Element(&Info.input);
					if (i >= len1)
					{
						break;
					}
				}
				else if (in == '\b')
				{
					update_Element(&Password.input);
					if (i > 0)
					{
						i--;
						if (i < len1)
						{
							Password.input.info[i] = 0;
							OldPassword[len1-1 - i] = 0;
							update_Element(&Password.input);
						}
						if (i >= len1)
						{
							Info.input.info[i - len1] = 0;
							NewInfo[i - len1] = 0;
							update_Element(&Info.input);
						}
					}

				}
				else if (in == 27)
				{
					*mode = mode_MainPage;
					close_window(acc);
					goto END;
				}
				else if (i < len1)
				{
					Password.input.info[i] = in;
					OldPassword[i] = in;
					i++;
					update_Element(&Password.input);
				}
				else if (i >= len1 && i < len1+len2)
				{
					Info.input.info[i - len1] = in;
					NewInfo[i - len1] = in;
					i++;
					update_Element(&Info.input);
				}
			}
		}

		close_window(acc);

		operation.info = user;
		strcpy_s(operation.info.password, 7, OldPassword);
		if (*mode == mode_ChangePassword)
		{
			strcpy_s(operation.info.passwordf, 7, NewInfo);
			operation.optype = T_CPASS;
		}
		if (*mode == mode_ChangeName)
		{
			strcpy_s(operation.info.name, LEN_NAME, NewInfo);
			operation.optype = T_CNAME;
		}
		setData();
		setRequest(1);

		//等待
		time_t now;
		time_t startime;
		time_t lastime;
		time(&now);
		startime = lastime = now;
		serverOff = 0;
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
				serverOff = 1;
				break;
			}
		}
		if (servermsg_global == R_SDT && *mode == mode_ChangePassword)
		{
			strcpy_s(user.password, 7, NewInfo);
		}
		else if (servermsg_global == R_SDT && *mode == mode_ChangeName)
		{
			strcpy_s(user.name, LEN_NAME, NewInfo);
		}
		else if (servermsg_global == R_N)
		{
			goto REIN;
		}
		else if (servermsg_global == 0)
		{
			COORD pos = csbi.dwSize;
			pos.Y += 10;
			printc(pos, "Server Timeout !");
			goto REIN;
		}
	END: {}
	}
}
//登入页面
void SigninPage()
{
REIN:
	{
		COORD size = csbi.dwSize;
		struct windowframe acc = initwindow(size.X / 2 - 15, size.Y / 2 - 3, size.X / 2 + 20, size.Y / 2 + 3);
		strcpy_s(acc.name, 10, "Sign-In");
		acc.corner[0] = '*';
		frame_window(acc);
		struct inWin  Account = initinWin(size.X / 2 - 5, size.Y / 2 - 1, 6, 12, "账户");
		struct inWin  PassWord = initinWin(size.X / 2 - 5, size.Y / 2 + 1, 6, 12, "密码");
		char OldPassword[11] = { 0 };
		char NewPassword[7] = { 0 };
		{
			int i = 0;
			char in;
			while (1)
			{
				in = _getch();
				if (in == '\r')
				{
					printf("\b");
					update_Element(&Account.input);
					update_Element(&PassWord.input);
					if (i == 16)
					{
						break;
					}
				}
				else if (in == '\b')
				{
					update_Element(&Account.input);
					if (i > 0)
					{
						i--;
						if (i <= 9)
						{
							Account.input.info[i] = 0;
							OldPassword[9 - i] = 0;
							update_Element(&Account.input);
						}
						if (i >= 10)
						{
							PassWord.input.info[i - 10] = 0;
							NewPassword[i - 10] = 0;
							update_Element(&PassWord.input);
						}
					}

				}
				else if (i <= 9)
				{
					Account.input.info[i] = in;
					OldPassword[9 - i] = in;
					i++;
					update_Element(&Account.input);
				}
				else if (i >= 10 && i <= 15)
				{
					PassWord.input.info[i - 10] = in;
					NewPassword[i - 10] = in;
					i++;
					update_Element(&PassWord.input);
				}
			}
		}
		struct User newuser;
		newuser.systemid = 0;
		close_window(acc);
		for (int i = 0; i < 10; i++)
		{
			newuser.systemid += (OldPassword[i] - '0') * pow(10, i);
		}
		strcpy_s(newuser.password, 7, NewPassword);
		operation.info = newuser;
		operation.optype = T_SIN;
		setData();
		setRequest(1);
		while (memberlist_global.C_Member != NULL) dele_Member(&memberlist_global, memberlist_global.C_Member);
		while (recordlist_global.C_Record != NULL) dele_Record(&recordlist_global, recordlist_global.C_Record);
		time_t now;
		time_t startime;
		time_t lastime;
		time(&now);
		startime = lastime = now;
		serverOff = 0;
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
				serverOff = 1;
				break;
			}
		}
		if (servermsg_global == R_SDT)
		{
			getData(&memberlist_global, &recordlist_global);
		}
		else if (servermsg_global == R_N)
		{
			goto REIN;
		}
		else if (servermsg_global == 0 || memberlist_global.C_Member == NULL)
		{
			if (strcmp(newuser.password, user.password) == 0 && user.systemid == newuser.systemid)
			{
				readfile_Member(&memberlist_global);
				readfile_Record_all(&recordlist_global);
			}
			else
			{
				goto REIN;
			}
		}

		//初始化
		import_to_member(&recordlist_global, memberlist_global);

		member_global = NULL;
		member_global = search_member(memberlist_global, newuser.systemid);
		if (member_global == NULL) exit(-4);
		int state = 1;
		writefile_userinfo(&newuser, &state);
		user = newuser;
	}
}

//主页
void MainPage(
	int* page,
	struct msgWin* buttonPage,
	struct msgWin* buttonSave,
	struct sheetWin* sheetwin,
	struct inBox* inbox,
	struct msgWin* msgwin,
	struct Member* member,
	struct msgWin* signout,
	struct msgWin* Password,
	struct msgWin* family)
{
	INPUT_RECORD mouserec;      //定义输入事件结构体  
	DWORD res;      //用于存储读取记录  
	COORD pos;      //用于存储鼠标当前位置  
	for (;;)
	{
		ReadConsoleInput(handle_in, &mouserec, 1, &res);      //读取输入事件  
		COORD windowsize = csbi.dwSize;
		pos = mouserec.Event.MouseEvent.dwMousePosition;    //获得当前鼠标位置
		GetConsoleScreenBufferInfo(handle_out, &csbi);
		gotoxy(0, 0);  //设置鼠标位置  
		DisplayMousePosition(pos);      //显示鼠标位置  

		if (in_window(pos, inbox->frame))
		{
			inBox_control(mouserec, inbox, sheetwin, msgwin, member);
		}

		if (!(windowsize.X == csbi.dwSize.X && windowsize.Y == csbi.dwSize.Y))
		{
			*page = mode_MainPage;
			ClearScreen();
			break;
		}

		if (ispress_msgwin(mouserec, buttonPage))
		{
			*page = mode_RecordPage;
			close_window(inbox->frame);
			close_MegWin(*msgwin);
			close_window(sheetwin->frame);
			break;
		}
		if (ispress_msgwin(mouserec, signout))
		{
			close_window(inbox->frame);
			close_MegWin(*msgwin);
			close_window(sheetwin->frame);
			*page = mode_Resign;
			int accstate = 0;
			writefile_userinfo(&user, &accstate);
			break;
		}
		if (ispress_msgwin(mouserec, Password))
		{
			close_window(inbox->frame);
			close_MegWin(*msgwin);
			close_window(sheetwin->frame);
			*page = mode_ChangeInfo;
			break;
		}
		if (ispress_msgwin(mouserec, family))
		{
			close_window(inbox->frame);
			close_MegWin(*msgwin);
			close_window(sheetwin->frame);
			/**page = ;*/
			break;
		}
		if (ispress_msgwin(mouserec, buttonSave))
		{
			*page = mode_Exit;
			break;
		}
	}
}

//记录页
void RecordPage(
	int* page,
	struct msgWin* buttonPage,
	struct msgWin* buttonSave,
	struct sheetWin* sheetwin,
	struct graphWin* graphwin,
	struct msgWin* buttonDelete,
	struct msgWin* buttonSwitch,
	struct msgWin* buttonExport,
	struct msgWin* msgwin,
	struct coBox* cobox,
	struct MemberList memberlist,
	struct Member* member)
{
	INPUT_RECORD mouserec;      //定义输入事件结构体  
	DWORD res;      //用于存储读取记录  
	COORD pos;      //用于存储鼠标当前位置  
	int mode = 0;
	for (;;)
	{
		ReadConsoleInput(handle_in, &mouserec, 1, &res);      //读取输入事件  
		COORD windowsize = csbi.dwSize;
		pos = mouserec.Event.MouseEvent.dwMousePosition;    //获得当前鼠标位置  
		GetConsoleScreenBufferInfo(handle_out, &csbi);
		gotoxy(0, 0);  //设置鼠标位置  
		DisplayMousePosition(pos);      //显示鼠标位置  
		if (mouserec.EventType == MOUSE_EVENT)    //如果当前为鼠标事件  
		{
			if (mode == 0)
				sheetWin_control(mouserec, sheetwin, msgwin, member);
			if (mode == 1)
				graphWin_control(mouserec, sheetwin, graphwin, msgwin);
			//删除
			if (ispress_msgwin(mouserec, buttonDelete))
				delete_sheetWin(sheetwin, msgwin, member);
			if (in_window(pos, cobox->frame))
				coBox_control(mouserec, sheetwin, graphwin, cobox, msgwin, memberlist, mode);

			if (ispress_msgwin(mouserec, buttonSwitch))
			{
				mode = !mode;
				if (mode == 1)
				{
					close_window(sheetwin->frame);
					frame_window(graphwin->frame);
					update_options(graphwin);
					set_barGraph(&graphwin->Graph, sheetwin->sheet, graphwin->mode, graphwin->gap, graphwin->area.Right - graphwin->area.Left);
					reflash_graphWin(graphwin, REFLASH_ALL);
					strcpy_s(buttonSwitch->info, 101, "Show In Sheet");
					update_MsgWin(*buttonSwitch);
				}
				else if (mode == 0)
				{
					close_window(graphwin->frame);
					frame_window(sheetwin->frame);
					reflash_sheetWin(sheetwin, REFLASH_ALL);
					strcpy_s(buttonSwitch->info, 101, "Show In Graph");
					update_MsgWin(*buttonSwitch);
				}
			}

			if (ispress_msgwin(mouserec, buttonPage))
			{
				close_window(sheetwin->frame);
				close_window(cobox->frame);
				close_MegWin(*msgwin);
				close_MegWin(*buttonDelete);
				close_MegWin(*buttonSwitch);
				close_MegWin(*buttonExport);
				*page = mode_MainPage;
				break;
			}
			if (ispress_msgwin(mouserec, buttonExport))
			{
				exportreocrd(NULL, 1);
				for (int i = 0; i < sheetwin->sheet.num; i++)
				{
					if (sheetwin->sheet.Roll[i].select == true)
					{
						exportreocrd(sheetwin->sheet.Roll[i].record, 0);
						sheetwin->sheet.Roll[i].select = false;
					}
				}
				reflash_sheetWin(sheetwin, REFLASH_CUR);
			}
			if (ispress_msgwin(mouserec, buttonSave))
			{
				*page = mode_Exit;
				break;
			}

		}
		if (windowsize.X != csbi.dwSize.X || windowsize.Y != csbi.dwSize.Y)
		{
			ClearScreen();
			*page = mode_RecordPage;
			break;
		}
	}
}

//总控制
void UI()
{
INTI:
	{
		struct MemberList memberlist = memberlist_global;
		struct Member* member = member_global;

		struct Sheet sheet1 = initsheet();

		//INPUT_RECORD mouserec;      //定义输入事件结构体  
		//DWORD res;      //用于存储读取记录  
		//COORD pos;      //用于存储鼠标当前位置  
		int page = mode_MainPage;
		for (;;)
		{
			COORD size = csbi.dwSize;
			struct msgWin 主页 = initmsgWin(size.X / 2 - 30, size.Y - 2, size.X / 2 - 10, size.Y + 1);
			strcpy_s(主页.info, 20, "Main Page");
			主页.name_a = FORE_WHITE;
			frame_MsgWin(主页);
			struct msgWin 记录页 = initmsgWin(size.X / 2 + 10, size.Y - 2, size.X / 2 + 30, size.Y + 1);
			strcpy_s(记录页.info, 20, "Record Page");
			记录页.name_a = FORE_WHITE;
			frame_MsgWin(记录页);
			struct msgWin 退出 = initmsgWin(size.X / 2 - 9, size.Y - 2, size.X / 2 + 9, size.Y + 1);
			strcpy_s(退出.info, 20, "Save & Exit");
			退出.name_a = FORE_WHITE;
			frame_MsgWin(退出);

			if (page == mode_RecordPage)
			{
				struct layoutFrame recordFrame = initlayoutFrame_r(2, 2, size.X, size.Y - 5);

				int printform_s[8] = { 6,14,10,10,10,10,10,10 };
				for (int i = 0; i < 8; i++)
				{
					printform[i] = printform_s[i] * (float)recordFrame.lenth / 84;
				}

				struct Sheet sheet = initsheet();
				search_Record(&sheet, memberlist, initSearchinfo(member->name));

				struct sheetWin 表格窗口 = initsheetWin(recordFrame.left, 7, size.X - 1, size.Y - 9, sheet);
				close_window(表格窗口.frame);
				struct graphWin 图像窗口 = initgraphWin(recordFrame.left, 7, size.X - 2, size.Y - 15, "Graph", sheet);
				close_window(图像窗口.frame);

				struct msgWin 信息窗口 = initmsgWin(recordFrame.right - 30, recordFrame.bottom - 2, recordFrame.right, recordFrame.bottom);
				strcpy_s(信息窗口.info, 20, "Messeage");
				信息窗口.name_a = FORE_WHITE;
				frame_MsgWin(信息窗口);

				struct msgWin 删除键 = initmsgWin(2, recordFrame.bottom - 2, 18, recordFrame.bottom);
				strcpy_s(删除键.info, 20, "Delete");
				删除键.name_a = FORE_WHITE;
				frame_MsgWin(删除键);

				struct msgWin 切换键 = initmsgWin(20, recordFrame.bottom - 2, 36, recordFrame.bottom);
				strcpy_s(切换键.info, 20, "Show In Graph");
				切换键.name_a = FORE_WHITE;
				frame_MsgWin(切换键);

				struct msgWin 导出键 = initmsgWin(38, recordFrame.bottom - 2, 54, recordFrame.bottom);
				strcpy_s(导出键.info, 20, "Export");
				导出键.name_a = FORE_WHITE;
				frame_MsgWin(导出键);

				struct coBox 条件控制窗口 = initcoBox(recordFrame.left, recordFrame.top, 84, 4, "筛选", member);

				search_Record(&sheet, memberlist, initSearchinfo(member->name));
				frame_window(表格窗口.frame);
				reflash_sheetWin(&表格窗口, REFLASH_ALL);
				RecordPage(&page, &主页, &退出,
					&表格窗口, &图像窗口,
					&删除键, &切换键, &导出键,
					&信息窗口, &条件控制窗口,
					memberlist, member);
			}
			else if (page == mode_MainPage)
			{
				struct layoutFrame mainFrame = initlayoutFrame_f((size.X - 82) / 2, 2, 86, size.Y - 5);
				struct msgWin 信息窗口 = initmsgWin(mainFrame.left, mainFrame.bottom - 2, mainFrame.left + 84, mainFrame.bottom);
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
				frame_MsgWin(信息窗口);

				int l_recordwin = 30;
				struct sheetWin 最近记录 = initminisheetWin
				(mainFrame.left, mainFrame.top,
					mainFrame.left + l_recordwin, mainFrame.bottom - 10, sheet1);

				frame_window(最近记录.frame);
				reflash_minisheetWin(&最近记录, member->record);

				struct inBox 输入框 = initinBox(mainFrame.left, mainFrame.bottom - 7, 84, 4, "记录");

				int acc_operation_centerx = (size.X - (mainFrame.left + l_recordwin)) / 2 + mainFrame.left + l_recordwin;
				int acc_operation_centery = (mainFrame.top + mainFrame.bottom - 7) / 2;
				int bottom_lenth = 25;
				int acc_operation_edge = acc_operation_centerx - bottom_lenth / 2;
				struct layoutFrame AccLayout = initlayoutFrame_r
				(acc_operation_edge, mainFrame.top + 2,
					acc_operation_edge + bottom_lenth, acc_operation_centery + 4);

				struct msgWin userinfo = initmsgWin(mainFrame.left + 46, 6, mainFrame.left + 71, 10);
				strcpy_s(userinfo.info, 20, "Hi ! ");
				strcat_s(userinfo.info, 30, member->name);
				userinfo.coner[0] = '*';
				userinfo.frame_v[0] = ' ';
				userinfo.name_a = FORE_WHITE;
				frame_MsgWin(userinfo);

				struct msgWin 登出 = initmsgWin(mainFrame.left + 46, 12, mainFrame.left + 71, 14);
				strcpy_s(登出.info, 20, "Sign out");
				登出.coner[0] = '*';
				登出.name_a = FORE_WHITE;
				frame_MsgWin(登出);

				struct msgWin 修改密码 = initmsgWin(mainFrame.left + 46, 16, mainFrame.left + 71, 18);
				strcpy_s(修改密码.info, 20, "Change PassWord");
				修改密码.coner[0] = '*';
				修改密码.name_a = FORE_WHITE;
				frame_MsgWin(修改密码);

				struct msgWin 改变家庭 = initmsgWin(mainFrame.left + 46, 20, mainFrame.left + 71, 22);
				strcpy_s(改变家庭.info, 20, "Reset Family *");
				改变家庭.coner[0] = '*';
				改变家庭.name_a = FORE_WHITE;
				frame_MsgWin(改变家庭);

				MainPage(&page, &记录页,
					&退出,
					&最近记录, &输入框,
					&信息窗口,
					member,
					&登出, &修改密码, &改变家庭);

				close_MegWin(userinfo);
				close_MegWin(登出);
				close_MegWin(修改密码);
				close_MegWin(改变家庭);

				if (page == mode_Resign)
				{
					close_MegWin(主页);
					close_MegWin(记录页);
					close_MegWin(退出);
					SigninPage();
					goto INTI;
				}
				else if (page == mode_ChangeInfo)
				{
					close_MegWin(主页);
					close_MegWin(记录页);
					close_MegWin(退出);
					ChangeInfo(&page);
					if (page == mode_ChangePassword ||page == mode_ChangeName)
					{
						SigninPage();
						goto INTI;
					}
				}

			}
			else if (page == mode_Exit) break;

		}
	}
}