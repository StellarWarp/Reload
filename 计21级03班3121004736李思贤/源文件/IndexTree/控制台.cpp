#include"控制台.h"
#include<iostream>
#include <windows.h>  
#include <conio.h>  
using namespace std;

HANDLE handle_in;
HANDLE handle_out;
CONSOLE_SCREEN_BUFFER_INFO csbi;        //定义窗口缓冲区信息结构体 

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

void printc(int p[2], const char* str)
{
	COORD pos = { p[0],p[1] };
	SetConsoleCursorPosition(handle_out, pos);
	cout << str;
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

	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof cfi;
	cfi.nFont = 0;
	cfi.dwFontSize.X = 1;
	cfi.dwFontSize.Y = 1;
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = FW_NORMAL;

	/*wcscpy_s(cfi.FaceName, L"Lucida Console");
	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);*/

	handle_out = GetStdHandle(STD_OUTPUT_HANDLE);    //获得标准输出设备句柄  
	//COORD size = { 100, 50 };  //窗口缓冲区大小  
	//SetConsoleScreenBufferSize(handle_out, size);   //设置窗口缓冲区大小  
	GetConsoleScreenBufferInfo(handle_out, &csbi);  //获得窗口缓冲区信息  
}
//关闭
void UI_off()
{
	CloseHandle(handle_out);
	CloseHandle(handle_in);
}
