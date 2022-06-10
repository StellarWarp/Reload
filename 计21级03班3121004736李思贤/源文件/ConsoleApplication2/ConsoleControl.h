#pragma once
#include <stdio.h>  
#include <windows.h>  
#include <conio.h>  
#define FORE_BLUE        FOREGROUND_BLUE           //蓝色文本属性  
#define FORE_GREEN      FOREGROUND_GREEN          //绿色文本属性  
#define FORE_RED          FOREGROUND_RED            //红色文本属性  
#define FORE_PURPLE   (FORE_BLUE | FORE_RED)      //紫色文本属性  
#define FORE_CYAN        (FORE_BLUE | FORE_GREEN)    //青色文本属性  
#define FORE_YELLOW   (FORE_RED | FORE_GREEN)     //黄色文本属性  
#define FORE_GRAY         FOREGROUND_INTENSITY      //灰色文本属性  
#define FORE_WHITE   (FORE_GREEN | FORE_BLUE | FORE_RED)     //白色文本属性  

#define BACK_BLUE         BACKGROUND_BLUE           //蓝色背景属性  
#define BACK_GREEN       BACKGROUND_GREEN          //绿色背景属性  
#define BACK_RED            BACKGROUND_RED            //红色背景属性  
#define BACK_PURPLE    (BACK_BLUE | BACK_RED)      //紫色背景属性  
#define BACK_CYAN         (BACK_BLUE | BACK_GREEN)    //青色背景属性  
#define BACK_YELLOW     (BACK_RED | BACK_GREEN)     //黄色背景属性  
#define BACK_GRAY          BACKGROUND_INTENSITY      //灰色背景属性 
#define BACK_WHITE   (BACK_GREEN | BACK_BLUE | BACK_RED)     //白色背景属性

HANDLE handle_in;
HANDLE handle_out;
CONSOLE_SCREEN_BUFFER_INFO csbi;        //定义窗口缓冲区信息结构体 

void gotoxy(int x, int y);
void gotopos(COORD pos);
void printc(COORD pos, char* str);
void DisplayMousePosition(COORD pos);
char getche_Console();
void InitializeUI();
void UI_off();




