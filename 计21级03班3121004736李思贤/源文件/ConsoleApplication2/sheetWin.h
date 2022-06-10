#pragma once
//打印长度
int printform[8] = { 6,14,10,10,10,10,10,10 };
//date,name,amount,type,method,site,detail

//相对位置计算
COORD releventpos(COORD pos, SMALL_RECT area)
{
	COORD rpos = { pos.X - area.Left,pos.Y - area.Top };
	return rpos;
}

//依赖win_f
//表头刷新
void sheetWin_updateTitle(struct sheetWin info, COORD pos)
{
	char titile[7][15] = { " 日期 "," 名字 "," 额度 "," 类型 "," 方式 ","地点","细节" };
	char select[2][15] = { " [ ] "," [■] " };
	char state[3][5] = { "◇","△","▽" };
	gotopos(pos);
	if (info.last_selected == -2) printf(select[1]);
	else printf(select[0]);
	pos.X += printform[0];
	gotopos(pos);
	for (int i = 0; i < 7; i++)
	{
		printf("%s", titile[i]);
		printf("%s", state[info.state_sort[i]]);
		pos.X += printform[i + 1];
		gotopos(pos);
	}
}

struct sheetWin SheetWin_init(int x1, int y1, int x2, int y2, struct Sheet sheet)
{
	struct sheetWin sheetwin;
	win_f win = window_init(x1, y1, x2, y2);
	strcpy_s(win.name, 20, "Rrcord");
	window_frame(win);
	sheetwin.frame = win;
	sheetwin.sheet = sheet;
	sheetwin.area.Left = win.pos1.X + 1;
	sheetwin.area.Top = win.pos1.Y + 1;
	sheetwin.area.Right = win.pos2.X - 1;
	sheetwin.area.Bottom = win.pos2.Y - 1;
	sheetwin.capacity = sheetwin.area.Bottom - sheetwin.area.Top;//显示容量
	sheetwin.top_display = 0;
	sheetwin.bottom_display = sheetwin.capacity - 1;
	sheetwin.last_selected = -1;
	COORD pos;//显示位置
	pos.X = sheetwin.area.Left;
	pos.Y = sheetwin.area.Top;
	for (int i = 0; i < 7; i++)
	{
		sheetwin.state_sort[i] = 0;
	}
	for (int i = 0; i < sheetwin.capacity + 1; i++)
	{
		setblank(pos, sheetwin.area.Right - sheetwin.area.Left + 1);
		pos.Y++;
	};
	pos.X = sheetwin.area.Left;
	pos.Y = sheetwin.area.Top;
	sheetWin_updateTitle(sheetwin, pos);
	for (int i = 0; i < sheetwin.capacity && i < sheet.num; i++)
	{
		pos.X = sheetwin.area.Left;
		pos.Y = sheetwin.area.Top + i + 1;
		gotopos(pos);
		if (sheetwin.sheet.Roll[i].select)
		{
			printf(" [■] ");
		}
		else
		{
			printf(" [ ] ");
		}
		pos.X += printform[0];
		print_Record(*sheet.Roll[i].record, pos, printform);
	}
	return  sheetwin;
}

//重刷新
void sheetWin_reflash(struct sheetWin* info, int mode)
{

	COORD pos;//显示位置
	pos.X = info->area.Left;
	pos.Y = info->area.Top;
	int top;
	int bottom;
	sheetWin_updateTitle(*info, pos);
	if (mode == REFLASH_ALL)
	{
		top = info->top_display = 0;
		if (info->bottom_display > info->sheet.num - 1
			|| info->sheet.num - 1 < info->capacity - 1
			|| info->bottom_display == -1)
		{
			bottom = info->bottom_display = info->sheet.num - 1;
		}
		else
		{
			bottom = info->bottom_display = info->capacity - 1;
		}
	}
	else if (mode == REFLASH_CUR)
	{
		if (info->bottom_display > info->sheet.num - 1)
		{
			bottom = info->bottom_display = info->sheet.num - 1;
			top = info->top_display = info->bottom_display - info->sheet.num + 1;
		}
		else
		{
			top = info->top_display;
			bottom = info->bottom_display;
		}
	}
	struct Roll* Array = info->sheet.Roll;
	for (int i = top; i <= bottom && i < top + info->capacity; i++)
	{
		pos.X = info->area.Left;
		pos.Y++;
		gotopos(pos);
		setblank(pos, info->area.Right - info->area.Left + 1);
		if (Array[i].select)
		{
			printf(" [■] ");
		}
		else
		{
			printf(" [ ] ");
		}
		pos.X += printform[0];
		print_Record(*Array[i].record, pos, printform);
	}
	for (int i = bottom + 1; i < top + info->capacity; i++)
	{
		pos.X = info->area.Left;
		pos.Y++;
		gotopos(pos);
		setblank(pos, info->area.Right - info->area.Left + 1);
	}
}
//滚动
void sheetWin_scroll(int dir, struct sheetWin* info)
{
	//显示区域
	SMALL_RECT winarea = info->area;
	win_f win = info->frame;
	winarea.Left = info->area.Left;
	winarea.Top = info->area.Top;
	winarea.Right = info->area.Right;
	winarea.Bottom = info->area.Bottom;
	//首尾端控制
	int maxi = info->sheet.num - 1;
	if (info->top_display <= 0 && dir == -1) return;
	if (info->bottom_display >= maxi && dir == 1) return;
	//滚动
	window_scroll(win, dir, 1);
	COORD pos;
	int i;
	if (dir == 1)
	{
		pos.X = winarea.Left;
		pos.Y = winarea.Bottom;
		gotopos(pos);
		info->top_display++;
		info->bottom_display++;
		i = info->bottom_display;
	}
	else if (dir == -1)
	{
		pos.X = winarea.Left;
		pos.Y = winarea.Top + 1;
		gotopos(pos);
		info->top_display--;
		info->bottom_display--;
		i = info->top_display;
	}
	if (info->sheet.Roll[i].select)
	{
		printf(" [■] ");
	}
	else
	{
		printf(" [ ] ");
	}
	pos.X += printform[0];;
	print_Record(*info->sheet.Roll[i].record, pos, printform);
}
//选择器
void sheetWin_selector(int from, int to, struct sheetWin* info)
{
	int top = info->top_display;
	int bottom = info->bottom_display;
	COORD pos = { info->area.Left, info->area.Top + 1 };
	if (from == to || info->last_selected <= -1)
	{
		pos.Y--;
		if (info->last_selected == -2) sheetWin_updateTitle(*info, pos);
		pos.Y++;
		info->sheet.Roll[top + to].select = !info->sheet.Roll[top + to].select;
		info->last_selected = top + to;
		pos.Y += to;
		gotopos(pos);
		if (info->sheet.Roll[top + to].select)
		{
			printf(" [■] ");
		}
		else
		{
			printf(" [ ] ");
		}
	}
	else
	{
		int is = info->last_selected;//开始位置
		int ie = top + to;//结束位置
		if (is > ie)
		{
			int t = is;
			is = ie;
			ie = t;
			info->sheet.Roll[is].select = !info->sheet.Roll[is].select;
			info->sheet.Roll[ie].select = !info->sheet.Roll[ie].select;
		}
		info->sheet.Roll[is].select = !info->sheet.Roll[is].select;
		for (int i = is; i <= ie; i++)
		{
			info->sheet.Roll[i].select = !info->sheet.Roll[i].select;
			if (i >= top && i <= bottom)
			{
				pos.Y = info->area.Top + 1 + i - top;
				gotopos(pos);
				if (info->sheet.Roll[i].select)
				{
					printf(" [■] ");
				}
				else
				{
					printf(" [ ] ");
				}
			}
		}
		info->last_selected = -1;
	}
}
//修改
void sheetWin_modify(int row, int column,
	int PrintRange[7],
	struct sheetWin* info, struct MsgWin* msgwin, struct Member* member)
{
	Msg message = NULL;
	int top = info->top_display;
	int bottom = info->bottom_display;
	COORD pos = { info->area.Left, info->area.Top + 1 + row };
	struct Record* mrecord = info->sheet.Roll[top + row].record;
	if (mrecord->membersystemid == member->systemid)
	{
		if (column == 0)
		{
			pos.X += PrintRange[0];
			setblank(pos, printform[1]);
			gotopos(pos);
			inputDate(&mrecord->date, &message);
		}
		if (column == 1)
		{
			pos.X += PrintRange[1];
			gotopos(pos);
			message = NOACC;
		}
		if (column == 2)
		{
			pos.X += PrintRange[2];
			setblank(pos, printform[3]);
			gotopos(pos);
			inputAmount(&mrecord->amount, &message);
		}
		if (column == 3)
		{
			pos.X += PrintRange[3];
			setblank(pos, printform[4]);
			gotopos(pos);
			inputType(&mrecord->type, &message);
		}
		if (column == 4)
		{
			pos.X += PrintRange[4];
			setblank(pos, printform[5]);
			gotopos(pos);
			inputMethod(mrecord->method, &message);
		}
		if (column == 5)
		{
			pos.X += PrintRange[5];
			setblank(pos, printform[6]);
			gotopos(pos);
			inputSite(mrecord->site, &message);
		}
		if (column == 6)
		{
			pos.X += PrintRange[6];
			setblank(pos, printform[7]);
			gotopos(pos);
			inputDetail(mrecord->detail, &message);
		}
		pos.X = info->area.Left;
		gotopos(pos);
		for (int i = info->area.Left; i < info->area.Right; i++)
		{
			printf(" ");
		}
		gotopos(pos);
		if (info->sheet.Roll[top + row].select)
		{
			printf(" [■] ");
		}
		else
		{
			printf(" [ ] ");
		}
		pos.X += printform[0];
		print_Record(*info->sheet.Roll[top + row].record, pos, printform);
	}
	else message = NOACC;

	if (message == INVALID_DATE) strcpy_s(msgwin->info, 101, "Invalid date");
	else if (message == NOACC) strcpy_s(msgwin->info, 101, "No access");
	else if (message == INVALID_AMOUNT) strcpy_s(msgwin->info, 101, "Invalid amount");
	else if (message == INVALID_TYPE) strcpy_s(msgwin->info, 101, "Invalid type");
	else if (message == INVALID_METHOD) strcpy_s(msgwin->info, 101, "Invalid method");
	else if (message == INVALID_SITE) strcpy_s(msgwin->info, 101, "Invalid site");
	else if (message == INVALID_DETAIL) strcpy_s(msgwin->info, 101, "Invalid detail");
	else if (message == NULL) strcpy_s(msgwin->info, 101, "Modified");
	MsgWin_update(*msgwin);
	if (message == NULL)
	{
		operation.optype = T_MOD;
		operation.data = *mrecord;
		setData();
		setRequest(1);
	}
}
//排序
void sheetWin_sort(int column, struct sheetWin* info)
{
	if (column == 0)
	{
		QuickSort(info->sheet.Roll, 0, info->sheet.num - 1, get_date, cmp_int);
	}
	else if (column == 1)
	{
		QuickSort(info->sheet.Roll, 0, info->sheet.num - 1, get_name, cmp_str);
	}
	else if (column == 2)
	{
		QuickSort(info->sheet.Roll, 0, info->sheet.num - 1, get_amount, cmp_int);
	}
	else if (column == 3)
	{
		QuickSort(info->sheet.Roll, 0, info->sheet.num - 1, get_type, cmp_int);
	}
	else if (column == 4)
	{
		QuickSort(info->sheet.Roll, 0, info->sheet.num - 1, get_method, cmp_str);
	}
	else if (column == 5)
	{
		QuickSort(info->sheet.Roll, 0, info->sheet.num - 1, get_site, cmp_str);
	}
	else if (column == 6)
	{
		QuickSort(info->sheet.Roll, 0, info->sheet.num - 1, get_detail, cmp_str);
	}
}
//事件响应
void sheetWin_control(
	INPUT_RECORD input,
	struct sheetWin* sheetwin,
	struct MsgWin* msgwin,
	struct Member* member)
{
	COORD pos;      //用于存储鼠标当前位置  
	pos = input.Event.MouseEvent.dwMousePosition;
	//如果在窗体内
	if (pos.X >= sheetwin->area.Left && pos.X <= sheetwin->area.Right
		&& pos.Y >= sheetwin->area.Top && pos.Y <= sheetwin->area.Bottom)
	{
		//窗体滚动
		//if (scoll_up(&input))
		//{
		//	sheetWin_scroll(-1, sheetwin);
		//}
		//if (scoll_down(&input))
		//{
		//	sheetWin_scroll(1, sheetwin);
		//}
		if (scoll_up(&input))
		{
			sheetWin_scroll(-1, sheetwin);
		}
		if (scoll_down(&input))
		{
			sheetWin_scroll(1, sheetwin);
		}
		//选择&排序
		if (input.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED
			&& input.Event.MouseEvent.dwControlKeyState & LEFT_CTRL_PRESSED)
		{
			pos = input.Event.MouseEvent.dwMousePosition;
			pos = releventpos(pos, sheetwin->area);
			if (pos.Y > 0 && pos.X < 3 && pos.X>1)
			{
				sheetWin_selector(-1, pos.Y - 1, sheetwin);
			}
		}
		else
		{
			if (input.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
			{
				pos = input.Event.MouseEvent.dwMousePosition;
				pos = releventpos(pos, sheetwin->area);
				if (pos.Y > 0 && pos.X < 3 && pos.X>1)
				{
					sheetWin_selector(pos.Y - 1, pos.Y - 1, sheetwin);
				}
				if (pos.Y == 0)
				{
					//参数
					int PrintRange[7] = { 0 };
					for (int i = 0; i < 7; i++)
					{
						for (int j = 0; j <= i; j++)
						{
							PrintRange[i] += printform[j];
						}
					}
					if (pos.X == 2)//全选
					{
						if (sheetwin->last_selected != -2)
						{
							sheetwin->last_selected = -2;
							for (int i = 0; i < sheetwin->sheet.num; i++)
							{
								sheetwin->sheet.Roll[i].select = true;
							}
						}
						else
						{
							sheetwin->last_selected = -1;
							for (int i = 0; i < sheetwin->sheet.num; i++)
							{
								sheetwin->sheet.Roll[i].select = false;
							}
						}
						sheetWin_reflash(sheetwin, REFLASH_CUR);
					}
					if (pos.X >= printform[0])
					{
						bool sorted = false;
						for (int i = 0; i < 7; i++)
						{
							if (pos.X >= printform[i] && pos.X < (i == 6 ? PrintRange[6] + printform[7] : PrintRange[i + 1]))
							{
								int mode = 0;
								if (sheetwin->state_sort[i] == 0) { mode = 1; sheetWin_sort(i, sheetwin); }
								else if (sheetwin->state_sort[i] == 1) { mode = 2; Reverse(sheetwin->sheet.Roll, 0, sheetwin->sheet.num - 1); }
								else if (sheetwin->state_sort[i] == 2) { mode = 1; Reverse(sheetwin->sheet.Roll, 0, sheetwin->sheet.num - 1); }
								for (int i = 0; i < 7; i++) sheetwin->state_sort[i] = 0;
								sheetwin->state_sort[i] = mode;
								sorted = true;
								break;
							}
						}
						if (sorted)sheetWin_reflash(sheetwin, REFLASH_ALL);
					}
				}
			}
		}
		//修改
		if (input.Event.MouseEvent.dwEventFlags == DOUBLE_CLICK)
		{
			//print range//start
			/*int date_pr = printform[0];
			int name_pr = date_pr + printform[1];
			int amount_pr = name_pr + printform[2];
			int type_pr = amount_pr + printform[3];
			int method_pr = type_pr + printform[4];
			int site_pr = method_pr + printform[5];
			int detail_pr = site_pr + printform[6];*/
			int PrintRange[7] = { 0 };
			for (int i = 0; i < 7; i++)
			{
				for (int j = 0; j <= i; j++)
				{
					PrintRange[i] += printform[j];
				}
			}
			pos = input.Event.MouseEvent.dwMousePosition;
			pos = releventpos(pos, sheetwin->area);
			if (pos.Y > 0 && pos.X >= printform[0])
			{
				if (pos.X < PrintRange[1])
				{
					sheetWin_modify(pos.Y - 1, 0, PrintRange, sheetwin, msgwin, member);
				}
				else if (pos.X >= PrintRange[1] && pos.X < PrintRange[2])
				{
					sheetWin_modify(pos.Y - 1, 1, PrintRange, sheetwin, msgwin, member);
				}
				else if (pos.X >= PrintRange[2] && pos.X < PrintRange[3])
				{
					sheetWin_modify(pos.Y - 1, 2, PrintRange, sheetwin, msgwin, member);
				}
				else if (pos.X >= PrintRange[3] && pos.X < PrintRange[4])
				{
					sheetWin_modify(pos.Y - 1, 3, PrintRange, sheetwin, msgwin, member);
				}
				else if (pos.X >= PrintRange[4] && pos.X < PrintRange[5])
				{
					sheetWin_modify(pos.Y - 1, 4, PrintRange, sheetwin, msgwin, member);
				}
				else if (pos.X >= PrintRange[5] && pos.X < PrintRange[6])
				{
					sheetWin_modify(pos.Y - 1, 5, PrintRange, sheetwin, msgwin, member);
				}
				else if (pos.X >= PrintRange[6] && pos.X < PrintRange[6] + printform[7])
				{
					sheetWin_modify(pos.Y - 1, 6, PrintRange, sheetwin, msgwin, member);
				}
			}
		}

	}
}
//删除
void sheetWin_delete(struct sheetWin* sheetwin, struct MsgWin* msgwin, struct Member* member)
{
	struct Roll* pos = sheetwin->sheet.Roll;
	int num = sheetwin->sheet.num;
	bool changed = 0;
	for (int i = 0; i < num; i++)
	{
		if (pos->select && pos->record->membersystemid == member->systemid)
		{
			operation.optype = T_DEL;
			operation.data = *pos->record;
			setData();
			setRequest(1);
			waitRespond();
			if (getRespond() == 0)
			{
				strcpy_s(msgwin->info, 101, "Server Time Out");
				MsgWin_update(*msgwin);
			}
			else
			{
				strcpy_s(msgwin->info, 101, "Synchronized");
				MsgWin_update(*msgwin);
			}
			dele_Record(pos->recordlist, pos->record);
			dele_Sheet(&sheetwin->sheet, i);
			num--;
			i--;
			pos--;
			changed = 1;
		}
		pos++;
	}
	if (changed) sheetWin_reflash(sheetwin, REFLASH_CUR);
}

//简化版
struct sheetWin minisheetWin_init(int x1, int y1, int x2, int y2, struct Sheet sheet)
{
	struct sheetWin sheetwin;
	win_f win = window_init(x1, y1, x2, y2);
	strcpy_s(win.name, 20, "最近记录");
	window_frame(win);
	sheetwin.frame = win;
	sheetwin.sheet = sheet;
	sheetwin.area.Left = win.pos1.X + 1;
	sheetwin.area.Top = win.pos1.Y + 1;
	sheetwin.area.Right = win.pos2.X - 1;
	sheetwin.area.Bottom = win.pos2.Y - 1;
	sheetwin.capacity = sheetwin.area.Bottom - sheetwin.area.Top;//显示容量
	sheetwin.top_display = 0;
	sheetwin.bottom_display = sheetwin.capacity - 1;
	sheetwin.last_selected = -1;
	COORD pos;
	pos.X = sheetwin.area.Left;
	pos.Y = sheetwin.area.Top;
	for (int i = 0; i < 7; i++)
	{
		sheetwin.state_sort[i] = 0;
	}
	for (int i = 0; i < sheetwin.capacity + 1; i++)
	{
		setblank(pos, sheetwin.area.Right - sheetwin.area.Left + 1);
		pos.Y++;
	};
	return  sheetwin;
}
void minisheetWin_reflash(struct sheetWin* sheetwin, struct RecordList recordlist)
{
	sheetwin->sheet.num = 0;
	sheetwin->sheet.currentpos = sheetwin->sheet.Roll;
	struct Record* crecord = recordlist.E_Record;
	for (int i = 0; crecord; i++)
	{
		add_Sheet(&sheetwin->sheet, &recordlist, crecord);
		if (crecord)
			crecord = crecord->prev;
	}
	QuickSort(sheetwin->sheet.Roll, 0, sheetwin->sheet.num - 1, get_date, cmp_int);
	Reverse(sheetwin->sheet.Roll, 0, sheetwin->sheet.num - 1);
	COORD pos;//显示位置
	pos.X = sheetwin->area.Left;
	pos.Y = sheetwin->area.Bottom + 1;
	int top = 0;
	int bottom = sheetwin->sheet.num - 1;
	struct Roll* Array = sheetwin->sheet.Roll;
	int printform[8] = { 6,14,10,10,10,10,10,10 };
	for (int i = top; i <= bottom && i < top + sheetwin->capacity; i++)
	{
		pos.X = sheetwin->area.Left + 1;
		pos.Y--;
		gotopos(pos);
		setblank(pos, sheetwin->area.Right - sheetwin->area.Left);
		print_miniRecord(*Array[i].record, pos, printform);
	}
	for (int i = bottom + 1; i < top + sheetwin->capacity; i++)
	{
		pos.X = sheetwin->area.Left;
		pos.Y--;
		gotopos(pos);
		setblank(pos, sheetwin->area.Right - sheetwin->area.Left + 1);
	}
}
