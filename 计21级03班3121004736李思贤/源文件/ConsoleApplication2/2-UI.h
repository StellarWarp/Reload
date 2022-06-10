#pragma once
//依赖Element
//输入窗口
void InputWin_update(struct InputWin* inwin)
{
	Element_update(&inwin->input);
	Element_update(&inwin->title);
}
struct InputWin InputWin_init(int x, int y, int len1, int len2, char* title)
{
	struct InputWin inwin;
	inwin.data.i = -1;
	inwin.title = Element_init(x, y, len1, 0);
	strcpy_s(inwin.title.info, 101, title);
	strcpy_s(inwin.title.frame_r, 4, "|");
	inwin.input = Element_init(x + len1 - 1, y, len2, 0);
	strcpy_s(inwin.input.frame_l, 4, "|");
	InputWin_update(&inwin);
	return inwin;
}
bool InputWin_isin(struct InputWin* inwin, COORD pos)
{
	return Element_isin(&inwin->input, pos);
}
int InputWin_input(
	struct InputWin* inwin,
	void (*input)(), void(*str_trans)(),
	int limit, bool cerretion)
{
	Element_setblank(&inwin->input);
	int message = 0;
	if (limit)
	{
		input(limit, &inwin->data, &message);
	}
	else
	{
		input(&inwin->data, &message);
	}
	if (cerretion)
	{
		if (message == NULL && str_trans != NULL)
		{
			str_trans(&inwin->input.info, inwin->data);
		}
	}
	else
	{
		if (limit) strcpy_s(&inwin->input.info, limit + 1, inwin->data.s);
		else strcpy_s(&inwin->input.info, strlen(inwin->data.s)+1, inwin->data.s);
	}
	Element_update(&inwin->input);
	return message;
}
void InputWin_close(struct InputWin* inwin)
{
	Element_close(&inwin->title);
	Element_close(&inwin->input);
}

