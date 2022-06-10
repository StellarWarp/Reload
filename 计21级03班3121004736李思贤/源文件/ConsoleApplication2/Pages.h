#pragma once
//主页
void MainPage(
	int* page,
	struct MsgWin* buttonPage,
	struct MsgWin* buttonSave,
	struct sheetWin* sheetwin,
	struct InputBox* inbox,
	struct MsgWin* msgwin,
	struct Member* member,
	struct MsgWin* signout,
	struct MsgWin* Password,
	struct MsgWin* family)
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

		if (window_isin(pos, inbox->frame))
		{
			inBox_control(mouserec, inbox, sheetwin, msgwin, member);
		}

		if (!(windowsize.X == csbi.dwSize.X && windowsize.Y == csbi.dwSize.Y))
		{
			*page = mode_MainPage;
			ClearScreen();
			break;
		}

		if (MsgWin_ispress(mouserec, buttonPage))
		{
			*page = mode_RecordPage;
			window_close(inbox->frame);
			MsgWin_close(*msgwin);
			window_close(sheetwin->frame);
			break;
		}
		if (MsgWin_ispress(mouserec, signout))
		{
			window_close(inbox->frame);
			MsgWin_close(*msgwin);
			window_close(sheetwin->frame);
			*page = mode_Resign;
			int accstate = 0;
			writefile_userinfo(&user, &accstate);
			break;
		}
		if (MsgWin_ispress(mouserec, Password))
		{
			window_close(inbox->frame);
			MsgWin_close(*msgwin);
			window_close(sheetwin->frame);
			*page = mode_ChangeInfo;
			break;
		}
		if (MsgWin_ispress(mouserec, family))
		{
			window_close(inbox->frame);
			MsgWin_close(*msgwin);
			window_close(sheetwin->frame);
			*page = mode_ChangeFamily;
			break;
		}
		if (MsgWin_ispress(mouserec, buttonSave))
		{
			*page = mode_Exit;
			break;
		}
	}
}

//记录页
void RecordPage(
	int* page,
	struct MsgWin* buttonPage,
	struct MsgWin* buttonSave,
	struct sheetWin* sheetwin,
	struct graphWin* graphwin,
	struct MsgWin* buttonDelete,
	struct MsgWin* buttonSwitch,
	struct MsgWin* buttonExport,
	struct MsgWin* msgwin,
	struct ConditionBox* cobox,
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
			if (MsgWin_ispress(mouserec, buttonDelete))
				sheetWin_delete(sheetwin, msgwin, member);
			if (window_isin(pos, cobox->frame))
				coBox_control(mouserec, sheetwin, graphwin, cobox, msgwin, memberlist, mode);

			if (MsgWin_ispress(mouserec, buttonSwitch))
			{
				mode = !mode;
				if (mode == 1)
				{
					window_close(sheetwin->frame);
					window_frame(graphwin->frame);
					graphWin_updateOptions(graphwin);
					set_barGraph(&graphwin->Graph, sheetwin->sheet, graphwin->mode, graphwin->gap, graphwin->area.Right - graphwin->area.Left);
					graphWin_reflash(graphwin, REFLASH_ALL);
					strcpy_s(buttonSwitch->info, 101, "Show In Sheet");
					MsgWin_update(*buttonSwitch);
				}
				else if (mode == 0)
				{
					window_close(graphwin->frame);
					window_frame(sheetwin->frame);
					sheetWin_reflash(sheetwin, REFLASH_ALL);
					strcpy_s(buttonSwitch->info, 101, "Show In Graph");
					MsgWin_update(*buttonSwitch);
				}
			}

			if (MsgWin_ispress(mouserec, buttonPage))
			{
				window_close(sheetwin->frame);
				window_close(cobox->frame);
				MsgWin_close(*msgwin);
				MsgWin_close(*buttonDelete);
				MsgWin_close(*buttonSwitch);
				MsgWin_close(*buttonExport);
				*page = mode_MainPage;
				break;
			}
			if (MsgWin_ispress(mouserec, buttonExport))
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
				sheetWin_reflash(sheetwin, REFLASH_CUR);
			}
			if (MsgWin_ispress(mouserec, buttonSave))
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