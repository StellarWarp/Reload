#pragma once


//窗口框
win_f window_init(int x1, int y1, int x2, int y2)
{
	struct windowframe win;
	win.pos1.X = x1;
	win.pos1.Y = y1;
	win.pos2.X = x2;
	win.pos2.Y = y2;
	strcpy_s(win.name, 20, "");
	strcpy_s(win.top, 5, "-");
	strcpy_s(win.frame_h, 5, "-");
	strcpy_s(win.frame_v, 5, "|");
	strcpy_s(win.corner, 5, "+");
	win.a_top = NULL;
	win.a_frame_h = NULL;
	win.a_frame_v = NULL;
	win.baclground = FORE_WHITE;
	return win;
}
void window_frame(win_f  win)
{
	COORD pos = win.pos1;
	DWORD written;
	int hight = win.pos2.Y - win.pos1.Y;
	int lenth = win.pos2.X - win.pos1.X + 1;
	GetConsoleScreenBufferInfo(handle_out, &csbi);
	WriteConsoleOutputCharacter(handle_out, win.corner, 1, pos, &written);
	pos.X++;
	for (int i = 2; i < lenth; i++)
	{
		WriteConsoleOutputCharacter(handle_out, win.frame_h, 1, pos, &written);
		pos.X++;
	}
	WriteConsoleOutputCharacter(handle_out, win.corner, 1, pos, &written);
	//FillConsoleOutputAttribute(handle_out, win.top, lenth, pos, &written);
	pos = win.pos1;
	pos.X += 2;
	WriteConsoleOutputCharacter(handle_out, win.name, strlen(win.name), pos, &written);
	pos = win.pos1;
	pos.Y++;
	for (int i = 1; i < hight; i++)
	{
		WriteConsoleOutputCharacter(handle_out, win.frame_v, 1, pos, &written);
		pos.X++;
		FillConsoleOutputAttribute(handle_out, win.baclground, lenth - 2, pos, &written);
		pos.X += lenth - 2;
		WriteConsoleOutputCharacter(handle_out, win.frame_v, 1, pos, &written);
		pos.X -= lenth - 1;
		pos.Y++;
	}
	WriteConsoleOutputCharacter(handle_out, win.corner, 1, pos, &written);
	pos.X++;
	for (int i = 2; i < lenth; i++)
	{
		WriteConsoleOutputCharacter(handle_out, win.frame_h, 1, pos, &written);
		pos.X++;
	}
	WriteConsoleOutputCharacter(handle_out, win.corner, 1, pos, &written);
}
void window_close(win_f  win)
{
	SMALL_RECT winarea;      //定义移动区域
	winarea.Left = win.pos1.X;
	winarea.Top = win.pos1.Y;
	winarea.Right = win.pos2.X;
	winarea.Bottom = win.pos2.Y;
	COORD pos = { winarea.Left ,winarea.Bottom + 1 };     //移动位置
	CHAR_INFO chFill;       //定义填充字符
	chFill.Char.AsciiChar = ' ';
	chFill.Attributes = csbi.wAttributes;
	ScrollConsoleScreenBuffer(handle_out, &winarea, &winarea, pos, &chFill);
}
bool window_isin(COORD pos, win_f  win)
{
	if (pos.X >= win.pos1.X && pos.X <= win.pos2.X
		&& pos.Y >= win.pos1.Y && pos.Y <= win.pos2.Y)
		return true;
	return false;
}
void window_scroll(win_f  win, int dir, int top_modify)//滚动窗口
{
	SMALL_RECT winarea;      //定义移动区域
	winarea.Left = win.pos1.X + 1;
	winarea.Top = win.pos1.Y + 1 + top_modify;
	winarea.Right = win.pos2.X - 1;
	winarea.Bottom = win.pos2.Y - 1;
	COORD pos;     //移动位置
	CHAR_INFO chFill;       //定义填充字符
	chFill.Char.AsciiChar = ' ';
	chFill.Attributes = csbi.wAttributes;
	if (dir == 1)
	{
		pos.X = winarea.Left;
		pos.Y = winarea.Top - 1;
	}
	else if (dir == -1)
	{
		pos.X = winarea.Left;
		pos.Y = winarea.Top + 1;
	}
	ScrollConsoleScreenBuffer(handle_out, &winarea, &winarea, pos, &chFill); //移动文本
}

//信息窗口
struct MsgWin MsgWin_init(int x1, int y1, int x2, int y2)
{
	struct MsgWin msgwin;
	msgwin.size.Left = x1;
	msgwin.size.Top = y1;
	msgwin.size.Right = x2;
	msgwin.size.Bottom = y2;
	strcpy_s(msgwin.info, 20, "MsgWin");
	strcpy_s(msgwin.title, 20, "");
	strcpy_s(msgwin.top, 2, "-");
	strcpy_s(msgwin.frame_h, 2, "-");
	strcpy_s(msgwin.frame_v, 2, "|");
	strcpy_s(msgwin.coner, 2, "+");
	WORD name_a = FORE_WHITE;
	msgwin.baclground = FORE_WHITE;
	return msgwin;
}
void MsgWin_close(struct MsgWin msgwin)
{
	COORD pos = { msgwin.size.Left ,msgwin.size.Bottom + 1 };     //移动位置
	CHAR_INFO chFill;       //定义填充字符
	chFill.Char.AsciiChar = ' ';
	chFill.Attributes = csbi.wAttributes;
	ScrollConsoleScreenBuffer(handle_out, &msgwin.size, &msgwin.size, pos, &chFill);
}
void MsgWin_frame(struct MsgWin  msgwin)
{
	COORD pos = { msgwin.size.Left,msgwin.size.Top };
	DWORD written;
	int hight = msgwin.size.Bottom - msgwin.size.Top;
	int lenth = msgwin.size.Right - msgwin.size.Left;
	GetConsoleScreenBufferInfo(handle_out, &csbi);
	WriteConsoleOutputCharacter(handle_out, msgwin.coner, 1, pos, &written);
	pos.X++;
	for (int i = 2; i < lenth; i++)
	{
		WriteConsoleOutputCharacter(handle_out, msgwin.frame_h, 1, pos, &written);
		pos.X++;
	}
	WriteConsoleOutputCharacter(handle_out, msgwin.coner, 1, pos, &written);
	//FillConsoleOutputAttribute(handle_out, msgwin.top, lenth, pos, &written);
	pos.X = msgwin.size.Left;
	pos.X += 2;
	WriteConsoleOutputCharacter(handle_out, msgwin.title, strlen(msgwin.title), pos, &written);
	pos.X = msgwin.size.Left;
	pos.Y++;
	for (int i = 1; i < hight; i++)
	{
		WriteConsoleOutputCharacter(handle_out, msgwin.frame_v, 1, pos, &written);
		pos.X++;
		FillConsoleOutputAttribute(handle_out, msgwin.baclground, lenth - 2, pos, &written);
		pos.X += lenth - 2;
		WriteConsoleOutputCharacter(handle_out, msgwin.frame_v, 1, pos, &written);
		pos.X -= lenth - 1;
		pos.Y++;
	}
	WriteConsoleOutputCharacter(handle_out, msgwin.coner, 1, pos, &written);
	pos.X++;
	for (int i = 2; i < lenth; i++)
	{
		WriteConsoleOutputCharacter(handle_out, msgwin.frame_h, 1, pos, &written);
		pos.X++;
	}
	WriteConsoleOutputCharacter(handle_out, msgwin.coner, 1, pos, &written);

	pos.Y = (msgwin.size.Bottom + msgwin.size.Top) / 2;
	pos.X = (msgwin.size.Right + msgwin.size.Left) / 2 - strlen(msgwin.info) / 2;
	WriteConsoleOutputCharacter(handle_out, msgwin.info, strlen(msgwin.info), pos, &written);
	//FillConsoleOutputAttribute(handle_out, msgwin.name_a, strlen(msgwin.info), pos, &written);
}
void MsgWin_update(struct MsgWin  msgwin)
{
	DWORD written;
	COORD pos;
	pos.Y = (msgwin.size.Bottom + msgwin.size.Top) / 2;
	pos.X = msgwin.size.Left + 1;
	setblank(pos, msgwin.size.Right - msgwin.size.Left - 2);
	pos.Y = (msgwin.size.Bottom + msgwin.size.Top) / 2;
	pos.X = (msgwin.size.Right + msgwin.size.Left) / 2 - strlen(msgwin.info) / 2;
	WriteConsoleOutputCharacter(handle_out, msgwin.info, strlen(msgwin.info), pos, &written);
	//FillConsoleOutputAttribute(handle_out, msgwin.name_a, strlen(msgwin.info), pos, &written);
}
bool MsgWin_ispress(INPUT_RECORD input, struct MsgWin* msgwin)
{
	COORD pos;      //用于存储鼠标当前位置  
	pos = input.Event.MouseEvent.dwMousePosition;
	if (pos.X >= msgwin->size.Left && pos.X <= msgwin->size.Right
		&& pos.Y >= msgwin->size.Top && pos.Y <= msgwin->size.Bottom)
	{
		if (input.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
		{
			return 1;
		}
	}
	return 0;
}

//简化按钮&信息窗口
struct Element Element_init(int x, int y, int len, int infoshift)
{
	struct Element element;
	for (int i = 0; i < 101; i++)
	{
		element.info[i] = 0;
	}
	element.infoshift = infoshift;
	element.area.Top = element.area.Bottom = y;
	element.area.Left = x + 1;
	element.area.Right = x + len - 2;
	if (element.area.Left > element.area.Right)
	{
		printf("fail");
		exit(1);
	}
	strcpy_s(element.frame_l, 4, "[");
	strcpy_s(element.frame_r, 4, "]");
	COORD pos = { element.area.Left - 1,element.area.Top };
	printc(pos, element.frame_l);
	pos.X += len - 1;
	printc(pos, element.frame_r);
	return element;
}
void Element_close(struct Element* element)
{
	COORD pos = { element->area.Left - 1 ,element->area.Bottom+10 };     //移动位置
	CHAR_INFO chFill;       //定义填充字符
	chFill.Char.AsciiChar = ' ';
	chFill.Attributes = csbi.wAttributes;
	SMALL_RECT area = element->area;
	area.Left--;
	area.Right++;
	ScrollConsoleScreenBuffer(handle_out, &area, &area, pos, &chFill);
}
void Element_setblank(struct Element* element)
{
	COORD pos = { element->area.Left ,element->area.Bottom };     //移动位置
	setblank(pos, element->area.Right - element->area.Left + 1);
	gotopos(pos);
}
void Element_update(struct Element* element)
{
	COORD pos = { element->area.Left - 1,element->area.Top };
	setblank(pos, element->area.Right - element->area.Left + 3);
	printc(pos, element->frame_l);
	pos.X = element->area.Right + 1;
	printc(pos, element->frame_r);
	pos.X = element->area.Left + element->infoshift;
	printc(pos, element->info);
}
void Element_goto(struct Element element)
{
	gotoxy(element.area.Left, element.area.Top);
};
bool Element_isin(struct Element* element, COORD pos)
{
	if (pos.X >= element->area.Left && pos.X <= element->area.Right
		&& pos.Y == element->area.Top)
	{
		return 1;
	}
	return 0;
}