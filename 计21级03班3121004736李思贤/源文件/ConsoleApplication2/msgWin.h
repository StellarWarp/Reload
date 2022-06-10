#pragma once

void close_MegWin(struct msgWin msgwin)
{
	COORD pos = { msgwin.size.Left ,msgwin.size.Bottom + 1 };     //移动位置
	CHAR_INFO chFill;       //定义填充字符
	chFill.Char.AsciiChar = ' ';
	chFill.Attributes = csbi.wAttributes;
	ScrollConsoleScreenBuffer(handle_out, &msgwin.size, &msgwin.size, pos, &chFill);
}
void frame_MsgWin(struct msgWin  msgwin)
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
	FillConsoleOutputAttribute(handle_out, msgwin.name_a, strlen(msgwin.info), pos, &written);
}
void update_MsgWin(struct msgWin  msgwin)
{
	DWORD written;
	COORD pos;
	pos.Y = (msgwin.size.Bottom + msgwin.size.Top) / 2;
	pos.X = msgwin.size.Left + 1;
	setblank(pos, msgwin.size.Right - msgwin.size.Left - 2);
	pos.Y = (msgwin.size.Bottom + msgwin.size.Top) / 2;
	pos.X = (msgwin.size.Right + msgwin.size.Left) / 2 - strlen(msgwin.info) / 2;
	WriteConsoleOutputCharacter(handle_out, msgwin.info, strlen(msgwin.info), pos, &written);
	FillConsoleOutputAttribute(handle_out, msgwin.name_a, strlen(msgwin.info), pos, &written);
}
bool  ispressmsgwin(INPUT_RECORD input, struct msgWin* msgwin)
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