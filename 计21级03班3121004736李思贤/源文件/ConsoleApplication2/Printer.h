#pragma once
#include <conio.h>  
#include <windows.h>  
void print_Date(struct Date date);
void print_Record(struct Record record, COORD pos, int printform[]);
void print_miniRecord(struct Record record, COORD pos, int printform[]);
void setblank(COORD pos, int lenth);

void strcpy7(char* buffer, char* str);

void getstr_Date(char* buffer, struct Date date);
void getstr_Date_td(char* buffer, int td);
void getstr_Amount(char* buffer, int amount);
void getstr_int(char* buffer, int amount);
void getstr_Type(char* buffer, int type);

void print_bar(struct bar bar, float scale, COORD pos, int termlen);
