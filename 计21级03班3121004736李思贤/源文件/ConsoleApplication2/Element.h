#pragma once
void close_Element(struct Element* element)
{
	COORD pos = { element->area.Left - 1 ,element->area.Bottom };     //移动位置
	CHAR_INFO chFill;       //定义填充字符
	chFill.Char.AsciiChar = ' ';
	chFill.Attributes = csbi.wAttributes;
	ScrollConsoleScreenBuffer(handle_out, &element->area, &element->area, pos, &chFill);
}
void setblank_Element(struct Element* element)
{
	COORD pos = { element->area.Left ,element->area.Bottom };     //移动位置
	setblank(pos, element->area.Right - element->area.Left + 1);
	gotopos(pos);
}
void update_Element(struct Element* element)
{
	COORD pos = { element->area.Left - 1,element->area.Top };
	setblank(pos, element->area.Right - element->area.Left + 2);
	printc(pos, element->frame_l);
	pos.X = element->area.Right + 1;
	printc(pos, element->frame_r);
	pos.X = element->area.Left + element->infoshift;
	printc(pos, element->info);
}
void goto_Element(struct Element element)
{
	gotoxy(element.area.Left, element.area.Top);
};
bool  in_Element(COORD pos, struct Element* element)
{
	if (pos.X >= element->area.Left && pos.X <= element->area.Right
		&& pos.Y == element->area.Top)
	{
		return 1;
	}
	return 0;
}