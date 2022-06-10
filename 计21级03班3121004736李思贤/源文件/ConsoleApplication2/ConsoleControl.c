#include"ConsoleControl.h" 

//将光标移到坐标为(x,y)的位置  
void gotoxy(int x, int y)
{
	COORD pos = { x, y };
	SetConsoleCursorPosition(handle_out, pos);
}
void gotopos(COORD pos)
{
	SetConsoleCursorPosition(handle_out, pos);
}

void printc(COORD pos, char* str)
{
	DWORD written;
	WriteConsoleOutputCharacter(handle_out, str, strlen(str), pos, &written);
}
//显示鼠标所在位置  
void DisplayMousePosition(COORD pos)
{
	COORD dis = { 0, 0 };        //显示鼠标位置  
	WORD att = FORE_CYAN | FOREGROUND_INTENSITY | FOREGROUND_INTENSITY; //文本属性  
	DWORD w = NULL;
	GetConsoleScreenBufferInfo(handle_out, &csbi);  //获得窗口缓冲区信息  
	printf("X = %3d, Y = %3d   %3dx%3d", (int)pos.X, (int)pos.Y, csbi.dwSize.X, csbi.dwSize.Y);
	FillConsoleOutputAttribute(handle_out, att, 16, dis, &w);  //填充文本属性  

}

char getch_Console()
{
	INPUT_RECORD input;      //定义输入事件结构体  
	DWORD res;      //用于存储读取记录  
	COORD pos;      //用于存储鼠标当前位置  
	ReadConsoleInput(handle_in, &input, 1, &res);      //读取输入事件  
	if (input.EventType == KEY_EVENT && input.Event.KeyEvent.bKeyDown)
	{
		char c = input.Event.KeyEvent.uChar.AsciiChar;
		return c;
	}
	return 0;
}

char getche_Console()
{
	INPUT_RECORD input;      //定义输入事件结构体  
	DWORD res;      //用于存储读取记录  
	COORD pos;      //用于存储鼠标当前位置  
	ReadConsoleInput(handle_in, &input, 1, &res);      //读取输入事件  
	if (input.EventType == KEY_EVENT && input.Event.KeyEvent.bKeyDown)
	{
		char c = input.Event.KeyEvent.uChar.AsciiChar;
		if (c != '\n') putchar(c);
		if (c == '\b')
		{
			putchar(' ');
			putchar('\b');
		}
		return c;
	}
}

//初始化
void InitializeUI()
{
	handle_in = GetStdHandle(STD_INPUT_HANDLE);      //获得标准输入设备句柄 
	//设置模式
	DWORD mode = NULL;
	GetConsoleMode(handle_in, &mode);
	mode &= ~ENABLE_QUICK_EDIT_MODE;
	mode &= ~ENABLE_VIRTUAL_TERMINAL_INPUT;
	mode |= ENABLE_MOUSE_INPUT;
	SetConsoleMode(handle_in, mode);
	handle_out = GetStdHandle(STD_OUTPUT_HANDLE);    //获得标准输出设备句柄  
	GetConsoleScreenBufferInfo(handle_out, &csbi);  //获得窗口缓冲区信息  
}
//关闭
void UI_off()
{
	CloseHandle(handle_out);
	CloseHandle(handle_in);
}

char scoll_up(INPUT_RECORD input)
{
	if (input.Event.MouseEvent.dwEventFlags == MOUSE_WHEELED && input.Event.MouseEvent.dwButtonState == 0x00800000
		|| input.Event.KeyEvent.wVirtualKeyCode == VK_UP)
	{
		return 1;
	}
	return 0;
}

char scoll_down(INPUT_RECORD input)
{
	if (input.Event.MouseEvent.dwEventFlags == MOUSE_WHEELED && input.Event.MouseEvent.dwButtonState == 0xff800000
		|| input.Event.KeyEvent.wVirtualKeyCode == VK_DOWN)
	{
		return  1;
	}
	return 0;
}

void ClearScreen()
{
	//SMALL_RECT winarea;      //定义移动区域
	//winarea.Left = 0;
	//winarea.Top = 0;
	//winarea.Right = csbi.dwSize.X;
	//winarea.Bottom = csbi.dwSize.Y;
	//COORD pos = { 1000 ,1000 };     //移动位置
	//CHAR_INFO chFill;       //定义填充字符
	//chFill.Char.AsciiChar = ' ';
	//chFill.Attributes = csbi.wAttributes;
	//ScrollConsoleScreenBuffer(handle_out, &winarea, &winarea, pos, &chFill);
	system("cls");
	InitializeUI();
}
