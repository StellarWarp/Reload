#pragma once

struct ConditionBox ConditionBox_init(int x, int y, int lenth, int hight, char* title, struct Member* member)
{
	struct ConditionBox cobox;
	cobox.Searchinfo = Searchinfo_init(member->name);
	COORD pos = { x + 1,y + 1 };
	cobox.frame = window_init(x, y, x + lenth - 1, y + hight - 1);
	strcpy_s(cobox.frame.name, 101, "筛选");
	window_frame(cobox.frame);
	int gap = 15;
	int ini = 0;
	int len = 12;
	int len1 = 10;
	int len2 = 6;
	pos.X += ini;
	cobox.date_low = InputWin_init(pos.X, pos.Y, 6, len, (char*)"日期");
	pos.Y++;
	cobox.date_top = InputWin_init(pos.X, pos.Y, 6, len, (char*)"日期");
	pos.Y--;
	pos.X += gap + 4;
	cobox.name = InputWin_init(pos.X, pos.Y, 6, 8, (char*)"名字");
	pos.X += gap;
	cobox.amount_low = InputWin_init(pos.X, pos.Y, 6, len1, (char*)"额度");
	pos.Y++;
	cobox.amount_top = InputWin_init(pos.X, pos.Y, 6, len1, (char*)"额度");
	pos.Y--;
	pos.X += gap;
	cobox.type = InputWin_init(pos.X, pos.Y, 6, len1, (char*)"类型");
	pos.Y++;
	cobox.method = InputWin_init(pos.X, pos.Y, 6, len1, (char*)"方式");
	pos.Y--;
	pos.X += gap;
	cobox.site = InputWin_init(pos.X, pos.Y, 6, len1, (char*)"地点");
	pos.Y++;
	cobox.detail = InputWin_init(pos.X, pos.Y, 6, len1, (char*)"细节");

	cobox.date_low.data.d = cobox.Searchinfo.date_low;
	cobox.date_top.data.d = cobox.Searchinfo.date_top;
	cobox.amount_low.data.i = cobox.Searchinfo.amount_low;
	cobox.amount_top.data.i = cobox.Searchinfo.amount_top;
	cobox.type.data.i = 8;
	strcpy_s(cobox.name.data.s, 15, member->name);
	strcpy_s(cobox.name.input.info, 15, member->name);
	strcpy_s(cobox.name.input.info, 15, member->name);
	strcpy_s(cobox.Searchinfo.name, 15, member->name);
	/*strcpy_s(cobox.method.data.s, 101, cobox.Searchinfo.method);
	strcpy_s(cobox.site.data.s, 101, cobox.Searchinfo.site);
	strcpy_s(cobox.detail.data.s, 101, cobox.Searchinfo.detail);*/
	//update_coBox(&cobox);
	Element_update(&cobox.name.input);
	cobox.cmember = member;
	return cobox;
}
//条件栏
void coBox_close(struct ConditionBox* cobox)
{
	window_close(cobox->frame);
}
void coBox_update(struct ConditionBox* cobox)
{
	getstr_Date(&cobox->date_low.input.info, cobox->date_low.data.d);
	getstr_Amount(&cobox->amount_low.input.info, cobox->amount_low.data.i);
	getstr_Date(&cobox->date_top.input.info, cobox->date_top.data.d);
	getstr_Amount(&cobox->amount_top.input.info, cobox->amount_top.data.i);
	getstr_Type(cobox->type.input.info, cobox->type.data.i);
	strcpy_s(cobox->name.input.info, 101, cobox->name.data.s);
	strcpy_s(cobox->method.input.info, 101, cobox->method.data.s);
	strcpy_s(cobox->site.input.info, 101, cobox->site.data.s);
	strcpy_s(cobox->detail.input.info, 101, cobox->detail.data.s);

	Element_update(&cobox->date_low.input);
	Element_update(&cobox->amount_low.input);
	Element_update(&cobox->name.input);
	Element_update(&cobox->date_top.input);
	Element_update(&cobox->amount_top.input);
	Element_update(&cobox->type.input);
	Element_update(&cobox->method.input);
	Element_update(&cobox->site.input);
	Element_update(&cobox->detail.input);
}
void coBox_control(
	INPUT_RECORD input,
	struct sheetWin* sheetwin,
	struct graphWin* graphwin,
	struct ConditionBox* cobox,
	struct MsgWin* msgwin,
	struct MemberList memberlist,
	int mode)
{
	const int back = -1;
	const int front = 1;
	const int click = 2;
	int event = 0;
	static int lastevent = 0;
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
	//选择
	if (input.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
	{
		event = 2;
	}
	Msg message = NULL;
	bool changed = false;
	bool imchange = false;
	if (event == click)
	{
		if (Element_isin(&cobox->date_low.title, pos))
		{
			if (cobox->Searchinfo.searchmode & mode_date)
			{
				cobox->Searchinfo.searchmode &= ~mode_date;
				imchange = true;
				Element_setblank(&cobox->date_low.input);
			}
		}
		else if (Element_isin(&cobox->date_top.title, pos))
		{
			if (cobox->Searchinfo.searchmode & mode_date_top)
			{
				cobox->Searchinfo.searchmode &= ~mode_date_top;
				imchange = true;
				Element_setblank(&cobox->date_top.input);
			}
		}
		else if (Element_isin(&cobox->amount_low.title, pos))
		{
			if (cobox->Searchinfo.searchmode & mode_amount)
			{
				cobox->Searchinfo.searchmode &= ~mode_amount;
				imchange = true;
				cobox->amount_low.data.i = 0;
				Element_setblank(&cobox->amount_low.input);
			}
		}
		else if (Element_isin(&cobox->amount_top.title, pos))
		{
			if (cobox->Searchinfo.searchmode & mode_amount_top)
			{
				cobox->Searchinfo.searchmode &= ~mode_amount_top;
				imchange = true;
				cobox->amount_top.data.i = 0;
				Element_setblank(&cobox->amount_top.input);
			}
		}
		else if (Element_isin(&cobox->name.title, pos))
		{
			if (cobox->Searchinfo.searchmode & mode_name)
			{
				cobox->Searchinfo.searchmode &= ~mode_name;
				imchange = true;
				Element_setblank(&cobox->name.input);
			}
		}
		else if (Element_isin(&cobox->type.title, pos))
		{
			if (cobox->Searchinfo.searchmode & mode_type)
			{
				cobox->Searchinfo.searchmode &= ~mode_type;
				imchange = true;
				Element_setblank(&cobox->type.input);

			}
		}
		else if (Element_isin(&cobox->method.title, pos))
		{
			if (cobox->Searchinfo.searchmode & mode_method)
			{
				cobox->Searchinfo.searchmode &= ~mode_method;
				imchange = true;
				Element_setblank(&cobox->method.input);

			}
		}
		else if (Element_isin(&cobox->site.title, pos))
		{
			if (cobox->Searchinfo.searchmode & mode_site)
			{
				cobox->Searchinfo.searchmode &= ~mode_site;
				imchange = true;
				Element_setblank(&cobox->site.input);

			}
		}
		else if (Element_isin(&cobox->detail.title, pos))
		{
			if (cobox->Searchinfo.searchmode & mode_detail)
			{
				cobox->Searchinfo.searchmode &= ~mode_detail;
				imchange = true;
				Element_setblank(&cobox->detail.input);
			}
		}
	}

	if (Element_isin(&cobox->date_low.input, pos))
	{
		if (event == click)
		{
			Element_setblank(&cobox->date_low.input);
			inputDate(&cobox->date_low.data.d, &message);
			if (message == NULL)
			{
				getstr_Date(&cobox->date_low.input.info, cobox->date_low.data.d);
				CCint(date_low, d, mode_date);
				imchange = true;
			}
			Element_update(&cobox->date_low.input);
		}
		else if (event == front)
		{
			if (!(cobox->Searchinfo.searchmode & mode_date))
			{
				cobox->date_low.data.d = today();
			}
			else
			{
				cobox->date_low.data.d.td++;
			}
			update_Date(&cobox->date_low.data.d);
			getstr_Date(&cobox->date_low.input.info, cobox->date_low.data.d);
			CCint(date_low, d, mode_date);
		}
		else if (event == back)
		{
			if (!(cobox->Searchinfo.searchmode & mode_date))
			{
				cobox->date_low.data.d = today();
			}
			else
			{
				cobox->date_low.data.d.td--;
			}
			update_Date(&cobox->date_low.data.d);
			getstr_Date(&cobox->date_low.input.info, cobox->date_low.data.d);
			CCint(date_low, d, mode_date);
		}
	}
	else if (Element_isin(&cobox->date_top.input, pos))
	{
		if (event == click)
		{
			Element_setblank(&cobox->date_top.input);
			inputDate(&cobox->date_top.data.d, &message);
			if (message == NULL)
			{
				getstr_Date(&cobox->date_top.input.info, cobox->date_top.data.d);
				CCint(date_top, d, mode_date_top);
				imchange = true;
			}
			Element_update(&cobox->date_top.input);
		}
		else if (event == front)
		{
			if (!(cobox->Searchinfo.searchmode & mode_date_top))
			{
				cobox->date_top.data.d = today();
			}
			else
			{
				cobox->date_top.data.d.td++;
			}
			update_Date(&cobox->date_top.data.d);
			getstr_Date(&cobox->date_top.input.info, cobox->date_top.data.d);
			CCint(date_top, d, mode_date_top);
		}
		else if (event == back)
		{
			if (!(cobox->Searchinfo.searchmode & mode_date_top))
			{
				cobox->date_top.data.d = today();
			}
			else
			{
				cobox->date_top.data.d.td--;
			}
			update_Date(&cobox->date_top.data.d);
			getstr_Date(&cobox->date_top.input.info, cobox->date_top.data.d);
			CCint(date_top, d, mode_date_top);
		}
	}
	else if (Element_isin(&cobox->amount_low.input, pos))
	{
		if (event == click)
		{
			Element_setblank(&cobox->amount_low.input);
			inputAmount(&cobox->amount_low.data.i, &message);
			if (message == NULL)
			{
				getstr_Amount(&cobox->amount_low.input.info, cobox->amount_low.data.i);
				CCint(amount_low, i, mode_amount);
				imchange = true;
			}
			Element_update(&cobox->amount_low.input);
		}
		else if (event == front)
		{
			cobox->amount_low.data.i += 1000;
			getstr_Amount(&cobox->amount_low.input.info, cobox->amount_low.data.i);
			CCint(amount_low, i, mode_amount);
		}
		else if (event == back && cobox->amount_low.data.i >= 10)
		{
			cobox->amount_low.data.i -= 100;
			getstr_Amount(&cobox->amount_low.input.info, cobox->amount_low.data.i);
			CCint(amount_low, i, mode_amount);
		}
	}
	else if (Element_isin(&cobox->amount_top.input, pos))
	{
		if (event == click)
		{
			Element_setblank(&cobox->amount_top.input);
			inputAmount(&cobox->amount_top.data.i, &message);
			if (message == NULL)
			{
				getstr_Amount(&cobox->amount_top.input.info, cobox->amount_top.data.i);
				CCint(amount_top, i, mode_amount_top);
				imchange = true;

			}
			Element_update(&cobox->amount_top.input);
		}
		else if (event == front)
		{
			cobox->amount_top.data.i += 1000;
			getstr_Amount(&cobox->amount_top.input.info, cobox->amount_top.data.i);
			CCint(amount_top, i, mode_amount_top);
		}
		else if (event == back && cobox->amount_top.data.i >= 10)
		{
			cobox->amount_top.data.i -= 100;
			getstr_Amount(&cobox->amount_top.input.info, cobox->amount_top.data.i);
			CCint(amount_top, i, mode_amount_top);
		}
	}
	else if (Element_isin(&cobox->name.input, pos))
	{
		if (event == click)
		{
			Element_setblank(&cobox->name.input);
			inputName(&cobox->name.data.i, &message);
			if (message == NULL)
			{
				strcpy_s(cobox->name.input.info, 101, cobox->name.data.s);
				imchange = true;
				CCstr(name, s, mode_name);
			}
			Element_update(&cobox->name.input);
		}
		else if (event == front)
		{
			if (cobox->cmember->next)
			{
				cobox->cmember = cobox->cmember->next;
			}
			else
			{
				while (cobox->cmember->prev)
				{
					cobox->cmember = cobox->cmember->prev;
				}
			}
			imchange = true;
			strcpy_s(cobox->name.data.s, 101, cobox->cmember->name);
			strcpy_s(cobox->name.input.info, 101, cobox->name.data.s);
			CCstr(name, s, mode_name);
		}
		else if (event == back)
		{
			if (cobox->cmember->prev)
			{
				cobox->cmember = cobox->cmember->prev;
			}
			else
			{
				while (cobox->cmember->next)
				{
					cobox->cmember = cobox->cmember->next;
				}
			}
			imchange = true;
			strcpy_s(cobox->name.data.s, 101, cobox->cmember->name);
			strcpy_s(cobox->name.input.info, 101, cobox->name.data.s);
			CCstr(name, s, mode_name);
		}
	}
	else if (Element_isin(&cobox->type.input, pos))
	{
		if (event == click)
		{
			Element_setblank(&cobox->type.input);
			inputType(&cobox->type.data.i, &message);
			if (message == NULL)
			{
				getstr_Type(cobox->type.input.info, cobox->type.data.i);
				CCint(type, i, mode_type);
				imchange = true;
			}
			Element_update(&cobox->type.input);
		}
		else if (event == front)
		{
			cobox->type.data.i++;
			if (cobox->type.data.i >= 8) cobox->type.data.i = 0;
			getstr_Type(cobox->type.input.info, cobox->type.data.i);
			CCint(type, i, mode_type);
			imchange = true;
		}
		else if (event == back)
		{
			cobox->type.data.i--;
			if (cobox->type.data.i <= -1) cobox->type.data.i = 7;
			getstr_Type(cobox->type.input.info, cobox->type.data.i);
			CCint(type, i, mode_type);
			imchange = true;
		}
	}
	else if (Element_isin(&cobox->method.input, pos))
	{
		if (event == click)
		{
			Element_setblank(&cobox->method.input);
			inputMethod(&cobox->method.data.s, &message);
			if (message == NULL)
			{
				strcpy_s(cobox->method.input.info, 101, cobox->method.data.s);
				CCstr(method, s, mode_method);
				imchange = true;
			}
			Element_update(&cobox->method.input);
		}
	}
	else if (Element_isin(&cobox->site.input, pos))
	{
		if (event == click)
		{
			Element_setblank(&cobox->site.input);
			inputSite(&cobox->site.data.s, &message);
			if (message == NULL)
			{
				strcpy_s(cobox->site.input.info, 101, cobox->site.data.s);
				CCstr(site, s, mode_site);
				imchange = true;
			}
			Element_update(&cobox->site.input);
		}
	}
	else if (Element_isin(&cobox->detail.input, pos))
	{
		if (event == click)
		{
			Element_setblank(&cobox->detail.input);
			inputSite(&cobox->detail.data.s, &message);
			if (message == NULL)
			{
				strcpy_s(cobox->detail.input.info, 101, cobox->detail.data.s);
				CCstr(detail, s, mode_detail);
				imchange = true;
			}
			Element_update(&cobox->detail.input);

		}
	}

	if (event == 0 && (lastevent == -1 || lastevent == 1) || imchange == true)
	{
		for (int i = 0; i < 7; i++)
		{
			sheetwin->state_sort[i] = 0;
		}
		search_Record(&sheetwin->sheet, memberlist, cobox->Searchinfo);
		if (mode == 0)
			sheetWin_reflash(sheetwin, REFLASH_ALL);
		else
		{
			set_barGraph(&graphwin->Graph, sheetwin->sheet, graphwin->mode, graphwin->gap, graphwin->area.Right - graphwin->area.Left);
			graphWin_reflash(graphwin, REFLASH_ALL);
		}

	}
	lastevent = event;
	if (event)
	{
		if (sheetwin->sheet.num == 0) strcpy_s(msgwin->info, 101, "Not found");
		else if (message == NULL) strcpy_s(msgwin->info, 101, "Modified");
		else if (message == INVALID_DATE) strcpy_s(msgwin->info, 101, "Invalid date");
		else if (message == NOACC) strcpy_s(msgwin->info, 101, "No access");
		else if (message == INVALID_AMOUNT) strcpy_s(msgwin->info, 101, "Invalid amount");
		else if (message == INVALID_TYPE) strcpy_s(msgwin->info, 101, "Invalid type");
		else if (message == INVALID_METHOD) strcpy_s(msgwin->info, 101, "Invalid method");
		else if (message == INVALID_SITE) strcpy_s(msgwin->info, 101, "Invalid site");
		else if (message == INVALID_DETAIL) strcpy_s(msgwin->info, 101, "Invalid detail");
		MsgWin_update(*msgwin);
	}
}