#pragma once
//依赖InputWin
void inBox_update(struct InputBox* inbox)
{
	getstr_Date(&inbox->date.input.info, inbox->date.data.d);
	getstr_Amount(&inbox->amount.input.info, inbox->amount.data.i);
	getstr_Type(inbox->type.input.info, inbox->type.data.i);
	strcpy_s(inbox->method.input.info, 101, inbox->method.data.s);
	strcpy_s(inbox->site.input.info, 101, inbox->site.data.s);
	strcpy_s(inbox->detail.input.info, 101, inbox->detail.data.s);

	Element_update(&inbox->date.input);
	Element_update(&inbox->amount.input);
	Element_update(&inbox->type.input);
	Element_update(&inbox->method.input);
	Element_update(&inbox->site.input);
	Element_update(&inbox->detail.input);
	Element_update(&inbox->OK);
}
struct InputBox InputBox_init(int x, int y, int lenth, int hight, char* title)
{
	struct InputBox inbox;
	inbox.record = initRecord();
	COORD pos = { x + 1,y + 1 };
	inbox.frame = window_init(x, y, x + lenth - 1, y + hight - 1);
	strcpy_s(inbox.frame.name, 101, "输入");
	window_frame(inbox.frame);
	int gap = 24;
	int ini = 8;
	int len = 12;
	pos.X += ini;
	inbox.date = InputWin_init(pos.X, pos.Y, 6, len, (char*)"日期");
	pos.X += gap;
	inbox.amount = InputWin_init(pos.X, pos.Y, 6, len, (char*)"额度");
	pos.X += gap;
	inbox.type = InputWin_init(pos.X, pos.Y, 6, len, (char*)"类型");
	inbox.type.input.infoshift = 1;
	pos.X = x + 1 + ini;
	pos.Y++;
	inbox.method = InputWin_init(pos.X, pos.Y, 6, len, (char*)"方式");
	pos.X += gap;
	inbox.site = InputWin_init(pos.X, pos.Y, 6, len, (char*)"地点");
	pos.X += gap;
	inbox.detail = InputWin_init(pos.X, pos.Y, 6, len, (char*)"细节");
	pos.Y++;
	pos.X += 10;
	inbox.OK = Element_init(pos.X, pos.Y, 6, 0);
	strcpy_s(inbox.OK.info, 10, "确定");
	inbox.date.data.d = inbox.record.date;
	inbox.amount.data.i = inbox.record.amount;
	inbox.type.data.i = inbox.record.type;
	strcpy_s(inbox.method.data.s, 101, inbox.record.method);
	strcpy_s(inbox.site.data.s, 101, inbox.record.site);
	strcpy_s(inbox.detail.data.s, 101, inbox.record.detail);
	inBox_update(&inbox);
	return inbox;
}
//输入栏
void inBox_close(struct InputBox* inbox)
{
	window_close(inbox->frame);
}
void inBox_control(
	INPUT_RECORD input,
	struct InputBox* inbox,
	struct sheetWin* sheetwin,
	struct MsgWin* msgwin,
	struct Member* member)
{
	const int back = -1;
	const int front = 1;
	const int click = 2;
	int event = 0;
	COORD pos = input.Event.MouseEvent.dwMousePosition;
	//后退
	if (scoll_up(&input))
	{
		event = -1;
	}
	//前进
	if (scoll_down(&input))
	{
		event = 1;
	}
	//选择&排序
	if (input.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
	{
		event = 2;
	}
	Msg message = NULL;
	if (Element_isin(&inbox->date.input, pos))
	{
		if (event == click)
		{
			Element_setblank(&inbox->date.input);
			inputDate(&inbox->date.data.d, &message);
			if (message == NULL)
			{
				getstr_Date(&inbox->date.input.info, inbox->date.data.d);
			}
			Element_update(&inbox->date.input);
		}
		else if (event == front)
		{
			inbox->date.data.d.td++;
			update_Date(&inbox->date.data.d);
			getstr_Date(&inbox->date.input.info, inbox->date.data.d);
			Element_update(&inbox->date.input);
		}
		else if (event == back)
		{
			inbox->date.data.d.td--;
			update_Date(&inbox->date.data.d);
			getstr_Date(&inbox->date.input.info, inbox->date.data.d);
			Element_update(&inbox->date.input);
		}
	}
	else if (Element_isin(&inbox->amount.input, pos))
	{
		if (event == click)
		{
			Element_setblank(&inbox->amount.input);
			inputAmount(&inbox->amount.data.i, &message);
			if (message == NULL)
			{
				getstr_Amount(&inbox->amount.input.info, inbox->amount.data.i);
			}
			Element_update(&inbox->amount.input);
		}
		else if (event == front)
		{
			inbox->amount.data.i += 1000;
			getstr_Amount(&inbox->amount.input.info, inbox->amount.data.i);
			Element_update(&inbox->amount.input);
		}
		else if (event == back && inbox->amount.data.i >= 10)
		{
			inbox->amount.data.i -= 100;
			getstr_Amount(&inbox->amount.input.info, inbox->amount.data.i);
			Element_update(&inbox->amount.input);
		}
	}
	else if (Element_isin(&inbox->type.input, pos))
	{
		if (event == click)
		{
			Element_setblank(&inbox->type.input);
			inputType(&inbox->type.data.i, &message);
			if (message == NULL)
			{
				getstr_Type(inbox->type.input.info, inbox->type.data.i);
			}
			Element_update(&inbox->type.input);
		}
		else if (event == front)
		{
			inbox->type.data.i++;
			if (inbox->type.data.i >= 8) inbox->type.data.i = 0;
			getstr_Type(inbox->type.input.info, inbox->type.data.i);
			Element_update(&inbox->type.input);
		}
		else if (event == back)
		{
			inbox->type.data.i--;
			if (inbox->type.data.i <= -1) inbox->type.data.i = 7;
			getstr_Type(inbox->type.input.info, inbox->type.data.i);
			Element_update(&inbox->type.input);
		}
	}
	else if (Element_isin(&inbox->method.input, pos))
	{
		if (event == click)
		{
			Element_setblank(&inbox->method.input);
			inputMethod(&inbox->method.data.s, &message);
			if (message == NULL)
			{
				strcpy_s(inbox->method.input.info, 101, inbox->method.data.s);
			}
			Element_update(&inbox->method.input);
		}
	}
	else if (Element_isin(&inbox->site.input, pos))
	{
		if (event == click)
		{
			Element_setblank(&inbox->site.input);
			inputSite(&inbox->site.data.s, &message);
			if (message == NULL)
			{
				strcpy_s(inbox->site.input.info, 101, inbox->site.data.s);
			}
			Element_update(&inbox->site.input);
		}
	}
	else if (Element_isin(&inbox->detail.input, pos))
	{
		if (event == click)
		{
			Element_setblank(&inbox->detail.input);
			inputSite(&inbox->detail.data.s, &message);
			if (message == NULL)
			{
				strcpy_s(inbox->detail.input.info, 101, inbox->detail.data.s);
			}
			Element_update(&inbox->detail.input);

		}
	}
	if (event)
	{
		if (message == NULL) strcpy_s(msgwin->info, 101, "Modified");
		else if (message == INVALID_DATE) strcpy_s(msgwin->info, 101, "Invalid date");
		else if (message == NOACC) strcpy_s(msgwin->info, 101, "No access");
		else if (message == INVALID_AMOUNT) strcpy_s(msgwin->info, 101, "Invalid amount");
		else if (message == INVALID_TYPE) strcpy_s(msgwin->info, 101, "Invalid type");
		else if (message == INVALID_METHOD) strcpy_s(msgwin->info, 101, "Invalid method");
		else if (message == INVALID_SITE) strcpy_s(msgwin->info, 101, "Invalid site");
		else if (message == INVALID_DETAIL) strcpy_s(msgwin->info, 101, "Invalid detail");
		MsgWin_update(*msgwin);
		if (Element_isin(&inbox->OK, pos) && message == NULL)
		{
			inbox->record.date = inbox->date.data.d;
			inbox->record.amount = inbox->amount.data.i;
			inbox->record.type = inbox->type.data.i;
			strcpy_s(inbox->record.name, 15, member->name);
			strcpy_s(inbox->record.method, 20, inbox->method.data.s);
			strcpy_s(inbox->record.site, 20, inbox->site.data.s);
			strcpy_s(inbox->record.detail, 20, inbox->detail.data.s);
			inbox->record.membersystemid = member->systemid;
			inbox->record.block = 0;
			operation.optype = T_ADD;
			operation.data = inbox->record;
			setData();
			setRequest(1);
			waitRespond();
			inbox->record.systemid = getRespond();
			if (inbox->record.systemid == 0)
			{
				inbox->record.systemid = -1;
				strcpy_s(msgwin->info, 101, "Server Time Out");
				MsgWin_update(*msgwin);
			}
			else
			{
				strcpy_s(msgwin->info, 101, "Synchronized");
				MsgWin_update(*msgwin);
			}
			in_Record(&member->record, member->record.E_Record, inbox->record);
			minisheetWin_reflash(sheetwin, member->record);
		}
	}
}