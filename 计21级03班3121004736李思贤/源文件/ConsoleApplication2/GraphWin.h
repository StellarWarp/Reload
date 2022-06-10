#pragma once
//依赖winf、Element
struct graphWin GraphWin_init(int x, int y, int lenth, int hight, char* title, struct Sheet sheet)
{
	struct graphWin graphwin;
	COORD pos = { x + 1,y + 1 };
	graphwin.frame = window_init(x, y, x + lenth - 1, y + hight - 1);
	graphwin.Graph = initGraph();
	graphwin.area.Left = graphwin.frame.pos1.X + 1;
	graphwin.area.Top = graphwin.frame.pos1.Y + 1;
	graphwin.area.Right = graphwin.frame.pos2.X - 1;
	graphwin.area.Bottom = graphwin.frame.pos2.Y - 1;
	graphwin.capacity = graphwin.area.Bottom - graphwin.area.Top;//显示容量
	graphwin.top_display = 0;
	graphwin.bottom_display = graphwin.capacity - 1;
	graphwin.mode = mode_date | mode_range;
	graphwin.gap = 30;
	strcpy_s(graphwin.frame.name, 101, "Graph");
	window_frame(graphwin.frame);
	int gap0 = 10;
	int gap = 14;
	int ini = 2;
	int len = 6;
	pos.X += ini;
	graphwin.date = InputWin_init(pos.X, pos.Y, 6, len, (char*)"日期");
	graphwin.date.data.i = 30;
	strcpy_s(graphwin.date.input.info, 101, "30");
	pos.X += gap;
	graphwin.name = Element_init(pos.X, pos.Y, len, 0);
	strcpy_s(graphwin.name.info, 101, "名字");
	pos.X += gap0;
	graphwin.amount = InputWin_init(pos.X, pos.Y, 6, len, (char*)"额度");
	graphwin.amount.data.i = 10000;
	strcpy_s(graphwin.amount.input.info, 101, "100");
	pos.X += gap;
	graphwin.type = Element_init(pos.X, pos.Y, len, 0);
	strcpy_s(graphwin.type.info, 101, "类型");
	pos.X += gap0;
	graphwin.method = Element_init(pos.X, pos.Y, len, 0);
	strcpy_s(graphwin.method.info, 101, "方式");
	pos.X += gap0;
	graphwin.site = Element_init(pos.X, pos.Y, len, 0);
	strcpy_s(graphwin.site.info, 101, "地点");
	pos.X += gap0;
	graphwin.detail = Element_init(pos.X, pos.Y, len, 0);
	strcpy_s(graphwin.detail.info, 101, "细节");
	pos.Y++;
	pos.X += 10;
	Element_update(&graphwin.name);
	Element_update(&graphwin.type);
	Element_update(&graphwin.method);
	Element_update(&graphwin.site);
	Element_update(&graphwin.detail);
	return graphwin;
}
//图像
void graphWin_close(struct graphWin* graphwin)
{
	window_close(graphwin->frame);
}
void graphWin_updateOptions(struct graphWin* graphwin)
{
	Element_update(&graphwin->name);
	Element_update(&graphwin->type);
	Element_update(&graphwin->method);
	Element_update(&graphwin->site);
	Element_update(&graphwin->detail);
	InputWin_update(&graphwin->amount);
	InputWin_update(&graphwin->date);
}
void graphWin_reflash(struct graphWin* graphwin, int mode)
{

	COORD pos;//显示位置
	pos.X = graphwin->area.Left;
	pos.Y = graphwin->area.Top;
	int top;
	int bottom;
	if (mode == REFLASH_ALL)
	{
		top = graphwin->top_display = 0;
		if (graphwin->bottom_display > graphwin->Graph.num - 1
			|| graphwin->Graph.num - 1 < graphwin->capacity - 1
			|| graphwin->bottom_display == -1)
		{
			bottom = graphwin->bottom_display = graphwin->Graph.num - 1;
		}
		else
		{
			bottom = graphwin->bottom_display = graphwin->capacity - 1;
		}
	}
	else if (mode == REFLASH_CUR)
	{
		if (graphwin->bottom_display > graphwin->Graph.num - 1)
		{
			bottom = graphwin->bottom_display = graphwin->Graph.num - 1;
			top = graphwin->top_display = graphwin->bottom_display - graphwin->Graph.num + 1;
		}
		else
		{
			top = graphwin->top_display;
			bottom = graphwin->bottom_display;
		}
	}
	struct bar* bars = graphwin->Graph.bars;
	for (int i = top; i <= bottom && i < top + graphwin->capacity; i++)
	{
		pos.X = graphwin->area.Left;
		pos.Y++;
		setblank(pos, graphwin->area.Right - graphwin->area.Left + 1);
		print_bar(bars[i], graphwin->Graph.scale, pos, 10);
	}
	for (int i = bottom + 1; i < top + graphwin->capacity; i++)
	{
		pos.X = graphwin->area.Left;
		pos.Y++;
		gotopos(pos);
		setblank(pos, graphwin->area.Right - graphwin->area.Left + 1);
	}
}
void graphWin_scroll(int dir, struct graphWin* graphwin)
{
	//显示区域
	SMALL_RECT winarea = graphwin->area;
	win_f win = graphwin->frame;
	winarea.Left = graphwin->area.Left;
	winarea.Top = graphwin->area.Top;
	winarea.Right = graphwin->area.Right;
	winarea.Bottom = graphwin->area.Bottom;
	//首尾端控制
	int maxi = graphwin->Graph.num - 1;
	if (graphwin->top_display <= 0 && dir == -1) return;
	if (graphwin->bottom_display >= maxi && dir == 1) return;
	//滚动
	window_scroll(win, dir, 1);
	COORD pos;
	int i;
	if (dir == 1)
	{
		pos.X = winarea.Left;
		pos.Y = winarea.Bottom;
		gotopos(pos);
		graphwin->top_display++;
		graphwin->bottom_display++;
		i = graphwin->bottom_display;
	}
	else if (dir == -1)
	{
		pos.X = winarea.Left;
		pos.Y = winarea.Top + 1;
		gotopos(pos);
		graphwin->top_display--;
		graphwin->bottom_display--;
		i = graphwin->top_display;
	}
	print_bar(graphwin->Graph.bars[i], graphwin->Graph.scale, pos, 10);

}
void graphWin_control(
	INPUT_RECORD input,
	struct sheetWin* sheetwin,
	struct graphWin* graphwin,
	struct MsgWin* msgwin)
{
	COORD pos;      //用于存储鼠标当前位置  
	pos = input.Event.MouseEvent.dwMousePosition;
	//如果在窗体内
	if (pos.X >= graphwin->area.Left && pos.X <= graphwin->area.Right
		&& pos.Y >= graphwin->area.Top && pos.Y <= graphwin->area.Bottom)
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
		//点击
		if (input.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
		{
			event = 2;
		}
		//窗体滚动
		if (event == -1 && pos.Y != graphwin->area.Top)graphWin_scroll(-1, graphwin);
		else if (event == 1 && pos.Y != graphwin->area.Top)graphWin_scroll(1, graphwin);

		int change = false;
		if (Element_isin(&graphwin->date.title, pos))
		{
			if (event == 2)
			{
				graphwin->mode = mode_date | mode_range;
				graphwin->gap = graphwin->date.data.i;
				change = true;
			}
		}
		else if (Element_isin(&graphwin->date.input, pos))
		{
			if (event == 1)graphwin->date.data.i++;
			if (event == -1 && graphwin->date.data.i >= 1)graphwin->date.data.i--;
			if (event == -1 || event == 1)
			{
				graphwin->mode = mode_date | mode_range;
				getstr_int(graphwin->date.input.info, graphwin->date.data.i);
				Element_update(&graphwin->date.input);
				graphwin->gap = graphwin->date.data.i;
			}
		}
		else if (Element_isin(&graphwin->amount.title, pos))
		{
			if (event == 2)
			{
				graphwin->mode = mode_amount | mode_range;
				graphwin->gap = graphwin->amount.data.i;
				change = true;
			}
		}
		else if (Element_isin(&graphwin->amount.input, pos))
		{
			if (event == 1)graphwin->amount.data.i += 1000;
			if (event == -1 && graphwin->amount.data.i >= 1000)graphwin->amount.data.i -= 1000;
			if (event == -1 || event == 1)
			{
				graphwin->mode = mode_amount | mode_range;
				getstr_Amount_(graphwin->amount.input.info, graphwin->amount.data.i);
				Element_update(&graphwin->amount.input);
				graphwin->gap = graphwin->amount.data.i;
			}
		}
		else if (Element_isin(&graphwin->name, pos))
		{
			if (event == 2)
			{
				graphwin->mode = mode_name | mode_precise;
				change = true;
			}
		}
		else if (Element_isin(&graphwin->type, pos))
		{
			if (event == 2)
			{
				graphwin->mode = mode_type | mode_precise;
				change = true;
			}
		}
		else if (Element_isin(&graphwin->method, pos))
		{
			if (event == 2)
			{
				graphwin->mode = mode_method | mode_precise;
				change = true;
			}
		}
		else if (Element_isin(&graphwin->site, pos))
		{
			if (event == 2)
			{
				graphwin->mode = mode_site | mode_precise;
				change = true;
			}
		}
		else if (Element_isin(&graphwin->detail, pos))
		{
			if (event == 2)
			{
				graphwin->mode = mode_detail | mode_precise;
				change = true;
			}
		}

		if (pos.Y == graphwin->area.Top && event == 0 && (lastevent == -1 || lastevent == 1) || change)
		{
			set_barGraph(&graphwin->Graph, sheetwin->sheet, graphwin->mode, graphwin->gap, graphwin->area.Right - graphwin->area.Left);
			graphWin_reflash(graphwin, REFLASH_ALL);
		}
		lastevent = event;
	}
}
