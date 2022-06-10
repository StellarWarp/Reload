#pragma once
#include<stdlib.h>
//更改密码
void ChangeInfo(int* mode)
{
	INPUT_RECORD mouserec;      //定义输入事件结构体  
	DWORD res;      //用于存储读取记录  
	COORD pos;      //用于存储鼠标当前位置  
	COORD size = csbi.dwSize;
	struct MsgWin PasswordOption = MsgWin_init(size.X / 2 - 25, size.Y / 2 - 3, size.X / 2 - 4, size.Y / 2 + 3);
	struct MsgWin NameOption = MsgWin_init(size.X / 2 + 4, size.Y / 2 - 3, size.X / 2 + 25, size.Y / 2 + 3);
	strcpy_s(PasswordOption.info, 20, "Change PassWord");
	strcpy_s(NameOption.info, 20, "Change Name");
	PasswordOption.name_a = FORE_WHITE;
	NameOption.name_a = FORE_WHITE;
	MsgWin_frame(PasswordOption);
	MsgWin_frame(NameOption);

	for (;;)
	{
		ReadConsoleInput(handle_in, &mouserec, 1, &res);      //读取输入事件  
		COORD windowsize = csbi.dwSize;
		pos = mouserec.Event.MouseEvent.dwMousePosition;    //获得当前鼠标位置
		GetConsoleScreenBufferInfo(handle_out, &csbi);
		gotoxy(0, 0);  //设置鼠标位置  
		DisplayMousePosition(pos);      //显示鼠标位置  
		if (MsgWin_ispress(mouserec, &PasswordOption))
		{
			*mode = mode_ChangePassword;
			break;
		}
		else if (MsgWin_ispress(mouserec, &NameOption))
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
	MsgWin_close(PasswordOption);
	MsgWin_close(NameOption);
REIN:
	{
		if (*mode == mode_MainPage)
		{
			goto END;
		}
		struct windowframe acc = window_init(size.X / 2 - 15, size.Y / 2 - 3, size.X / 2 + 20, size.Y / 2 + 3);

		if (*mode == mode_ChangePassword) strcpy_s(acc.name, 20, "Change-PassWord");
		if (*mode == mode_ChangeName) strcpy_s(acc.name, 20, "Change-Name");

		acc.corner[0] = '*';
		window_frame(acc);
		char* title = NULL;
		if (*mode == mode_ChangePassword) title = "新密码";
		if (*mode == mode_ChangeName) title = "新名称";
		struct InputWin  Password = InputWin_init(size.X / 2 - 5, size.Y / 2 - 1, 8, 8, " 密码 ");
		int ilen;
		if (*mode == mode_ChangePassword) ilen = 8;
		if (*mode == mode_ChangeName) ilen = 12;
		struct InputWin  Info = InputWin_init(size.X / 2 - 5, size.Y / 2 + 1, 8, ilen, title);
		//char OldPassword[7] = { 0 };
		//char NewInfo[LEN_NAME] = { 0 };
		//输入
		{
			int i = 0;
			WCHAR in;
			int len1 = 6;
			int len2 = 10;
			if (*mode == mode_ChangePassword) len2 = 6;
			if (*mode == mode_ChangeName) len2 = 40;

			INPUT_RECORD input_record;      //定义输入事件结构体  
			DWORD res;      //用于存储读取记录  
			COORD pos;      //用于存储鼠标当前位置  

			bool acceptablePW = false;
			bool acceptableName = false;

			bool operateState = false;

			bool start = true;
			//输入
			for (;;)
			{
				ReadConsoleInput(handle_in, &input_record, 1, &res);      //读取输入事件  
				COORD windowsize = csbi.dwSize;
				pos = input_record.Event.MouseEvent.dwMousePosition;    //获得当前鼠标位置
				GetConsoleScreenBufferInfo(handle_out, &csbi);
				gotoxy(0, 0);  //设置鼠标位置  
				DisplayMousePosition(pos);      //显示鼠标位置  

				bool next = false;

				if (input_record.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED || start)
				{
					if (InputWin_isin(&Password, pos) || start--)
					{
						int msg = InputWin_input(&Password, inputLimitedChar, NULL, 6, false);
						if (msg & END_INPUT)
						{
							*mode = mode_MainPage;
							window_close(acc);
							goto END;
						}
						else if (msg == NULL)
						{
							acceptablePW = true;
						}
						if (msg & GONEXT)
						{
							if (msg == GONEXT) acceptablePW = true;
							next = true;
						}
					}
					if (InputWin_isin(&Info, pos) || next)
					{
						int msg = InputWin_input(&Info, inputName, NULL, 0, false);
						if (msg == NULL)
						{
							acceptableName = true;
						}
						if (msg & GONEXT)
						{
							if (msg == GONEXT) acceptableName = true;
							next = true;
						}
					}
				}
				if (input_record.Event.KeyEvent.uChar.AsciiChar == 27)
				{
					*mode = mode_MainPage;
					window_close(acc);
					goto END;
				}
				if (acceptableName && acceptablePW && input_record.Event.KeyEvent.uChar.AsciiChar == '\r')
				{
					break;
				}
			}
		}

		window_close(acc);

		operation.info = user;
		strcpy_s(operation.info.password, 7, Password.input.info);
		if (*mode == mode_ChangePassword)
		{
			strcpy_s(operation.info.passwordf, 7, Info.input.info);
			operation.optype = T_CPASS;
		}
		if (*mode == mode_ChangeName)
		{
			strcpy_s(operation.info.name, sizeof(Info.input.info) + 1, Info.input.info);
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
		serverOff_global = 0;
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
		if (servermsg_global == R_SDT && *mode == mode_ChangePassword)
		{
			strcpy_s(user.password, 7, Info.input.info);
		}
		else if (servermsg_global == R_SDT && *mode == mode_ChangeName)
		{
			strcpy_s(user.name, sizeof(Info.input.info) + 1, Info.input.info);
		}
		else if (servermsg_global == R_N)
		{
			goto REIN;
		}
		else if (servermsg_global == 0)
		{
			COORD pos = csbi.dwSize;
			pos.X /= 2;
			pos.Y -= 10;
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
		struct windowframe acc = window_init(size.X / 2 - 15, size.Y / 2 - 3, size.X / 2 + 20, size.Y / 2 + 3);
		strcpy_s(acc.name, 10, "Sign-In");
		acc.corner[0] = '*';
		window_frame(acc);
		struct InputWin  Account = InputWin_init(size.X / 2 - 5, size.Y / 2 - 1, 6, 12, "账户");
		struct InputWin  PassWord = InputWin_init(size.X / 2 - 5, size.Y / 2 + 1, 6, 12, "密码");
		struct MsgWin Msg = MsgWin_init(size.X / 2 - 15, size.Y / 2 - 7, size.X / 2 + 20, size.Y / 2 - 5);
		strcpy_s(Msg.info, 4, "Tip");
		MsgWin_frame(Msg);

		{
			INPUT_RECORD input_record;      //定义输入事件结构体  
			DWORD res;      //用于存储读取记录  
			COORD pos;      //用于存储鼠标当前位置  

			bool acceptableID = false;
			bool acceptablePW = false;

			bool operateState = false;

			bool start = true;
			while (1)
			{
				ReadConsoleInput(handle_in, &input_record, 1, &res);      //读取输入事件  
				COORD windowsize = csbi.dwSize;
				pos = input_record.Event.MouseEvent.dwMousePosition;    //获得当前鼠标位置
				GetConsoleScreenBufferInfo(handle_out, &csbi);
				gotoxy(0, 0);  //设置鼠标位置  
				DisplayMousePosition(pos);      //显示鼠标位置  

				bool next = false;

				if (input_record.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED || start)
				{
					if (InputWin_isin(&Account, pos) || start--)
					{
						int msg = InputWin_input(&Account, inputLimitedChar, NULL, 10, false);
						Account.data.i64 = _atoi64(Account.input.info);
						if (msg == NULL)
						{
							acceptableID = true;
						}
						else if (msg & INVALID_LENTH)
						{
						}
						if (msg & GONEXT)
						{
							if (msg == GONEXT) acceptableID = true;
							next = true;
						}
						if (!acceptableID) strcpy_s(Msg.info, 30, "Unacceptable Account");
						else if (!acceptablePW) strcpy_s(Msg.info, 30, "Unacceptable PassWord");
						else strcpy_s(Msg.info, 5, " ");
						MsgWin_update(Msg);
					}
					if (InputWin_isin(&PassWord, pos) || next)
					{
						int msg = InputWin_input(&PassWord, inputLimitedChar, NULL, 6, false);
						if (msg & END_INPUT)
						{
							break;
						}
						else if (msg == NULL)
						{
							acceptablePW = true;
						}
						else if (msg & INVALID_LENTH)
						{
						}
						if (msg & GONEXT)
						{
							if (msg == GONEXT) acceptablePW = true;
							next = true;
						}
						if (!acceptablePW) strcpy_s(Msg.info, 30, "Unacceptable PassWord");
						else if (!acceptableID) strcpy_s(Msg.info, 30, "Unacceptable Account");
						else strcpy_s(Msg.info, 5, " ");
						MsgWin_update(Msg);
					}
				}
				if (acceptableID && acceptablePW && input_record.Event.KeyEvent.uChar.AsciiChar == '\r')
				{
					break;
				}
			}
		}

		{
			struct User newuser;
			newuser.systemid = 0;
			window_close(acc);
			MsgWin_close(Msg);
			newuser.systemid = Account.data.i64;
			strcpy_s(newuser.password, 7, PassWord.input.info);
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
			serverOff_global = 0;
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
}
//
bool ChangeFamily()
{
	COORD size = csbi.dwSize;
	struct windowframe acc = window_init(size.X / 2 - 15, size.Y / 2 - 4, size.X / 2 + 20, size.Y / 2 + 4);
	strcpy_s(acc.name, 20, "Reset-Family");
	acc.corner[0] = '*';
	window_frame(acc);
	struct InputWin  PassWord = InputWin_init(size.X / 2 - 7, size.Y / 2 - 2, 10, 12, " 密码 ");
	struct InputWin  FamilyID = InputWin_init(size.X / 2 - 7, size.Y / 2, 10, 12, "家庭账户");
	struct InputWin  FamilyPassWord = InputWin_init(size.X / 2 - 7, size.Y / 2 + 2, 10, 12, "家庭密码");
	struct Element Confirm = Element_init(acc.pos2.X - 8, acc.pos2.Y, 6, 0);
	strcpy_s(Confirm.info, 10, "确认");
	Element_update(&Confirm);
	struct Element Msg = Element_init(size.X / 2 - 14, size.Y / 2 + 6, 34, 5);

	INPUT_RECORD input_record;      //定义输入事件结构体  
	DWORD res;      //用于存储读取记录  
	COORD pos;      //用于存储鼠标当前位置  

	bool acceptablePW = false;
	bool acceptableFID = false;
	bool acceptableFPW = false;

	bool operateState = false;

	bool start = true;
	//输入
	for (;;)
	{
		ReadConsoleInput(handle_in, &input_record, 1, &res);      //读取输入事件  
		COORD windowsize = csbi.dwSize;
		pos = input_record.Event.MouseEvent.dwMousePosition;    //获得当前鼠标位置
		GetConsoleScreenBufferInfo(handle_out, &csbi);
		gotoxy(0, 0);  //设置鼠标位置  
		DisplayMousePosition(pos);      //显示鼠标位置  

		bool next = false;

		if (input_record.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED || start)
		{
			if (InputWin_isin(&PassWord, pos) || start--)
			{
				int msg = InputWin_input(&PassWord, inputLimitedChar, NULL, 6, false);
				if (msg & END_INPUT)
				{
					break;
				}
				else if (msg == NULL)
				{
					acceptablePW = true;
				}
				else if (msg & INVALID_LENTH)
				{
					strcpy_s(Msg.info, 20, "INVALID");
					Element_update(&Msg);

				}
				if (msg & GONEXT)
				{
					if (msg == GONEXT) acceptablePW = true;
					next = true;
				}
			}
			if (InputWin_isin(&FamilyID, pos) || next)
			{
				int msg = InputWin_input(&FamilyID, inputLimitedChar, NULL, 10, false);
				FamilyID.data.i64 = _atoi64(FamilyID.input.info);
				if (msg & END_INPUT)
				{
					break;
				}
				else if (msg == NULL)
				{
					acceptableFID = true;
				}
				else if (msg & INVALID_LENTH)
				{
					strcpy_s(Msg.info, 20, "INVALID");
					Element_update(&Msg);
				}
				if (msg & GONEXT)
				{
					if (msg == GONEXT) acceptableFID = true;
					next = true;
				}
			}
			if (InputWin_isin(&FamilyPassWord, pos) || next)
			{
				int msg = InputWin_input(&FamilyPassWord, inputLimitedChar, NULL, 6, false);
				if (msg & END_INPUT)
				{
					break;
				}
				else if (msg == NULL)
				{
					acceptableFPW = true;
				}
				else if (msg & INVALID_LENTH)
				{
					strcpy_s(Msg.info, 20, "INVALID");
					Element_update(&Msg);
				}
				if (msg & GONEXT)
				{
					if (msg == GONEXT) acceptableFPW = true;
					next = true;
				}
			}
			if (Element_isin(&Confirm, pos) || next)
			{
				if (acceptablePW && acceptableFID && acceptableFPW)
				{
					operation.info = user;
					strcpy_s(operation.info.password, 7, PassWord.data.s);
					operation.info.familyid = FamilyID.data.i64;
					strcpy_s(operation.info.passwordf, 7, FamilyPassWord.data.s);
					operation.optype = T_JOIN;

					setData();
					setRequest(1);

					//等待
					time_t now;
					time_t startime;
					time_t lastime;
					time(&now);
					startime = lastime = now;
					serverOff_global = 0;
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
					if (servermsg_global == R_SDT)
					{
						user.familyid = FamilyID.data.i64;
						strcpy_s(user.passwordf, 7, FamilyPassWord.data.s);
						operateState = true;
						break;
					}
					else if (servermsg_global == R_N)
					{
						COORD pos = acc.pos2;
						pos.X -= 10;
						pos.Y += 2;
						strcpy_s(Msg.info, 20, "PassWord or ID errea");
						Element_update(&Msg);
					}
					else if (servermsg_global == 0)
					{
						COORD pos = acc.pos2;
						pos.X -= 10;
						pos.Y += 2;
						strcpy_s(Msg.info, 20, "Server Timeout !");
						Element_update(&Msg);
					}
				}
			}

		}
		if (input_record.Event.KeyEvent.uChar.AsciiChar == 27)
		{
			break;
		}
	}
	window_close(acc);
	Element_close(&Msg);
	return operateState;
}