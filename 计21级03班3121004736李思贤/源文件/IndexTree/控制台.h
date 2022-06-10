#pragma once

//将光标移到坐标为(x,y)的位置  
void gotoxy(int x, int y);

void printc(int p[2], const char* str);

//初始化
void InitializeUI();
//关闭
void UI_off();
