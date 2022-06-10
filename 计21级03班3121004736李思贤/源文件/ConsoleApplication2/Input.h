#pragma once
//input message
#define GONEXT 01
#define GOPREV 02
#define END_INPUT 04
#define INVALID_DATE 010
#define INVALID_NAME 020
#define INVALID_AMOUNT 040
#define INVALID_TYPE 0100
#define INVALID_METHOD 0200
#define INVALID_SITE 0400
#define INVALID_DETAIL 01000
#define NOACC 02000
#define NOTFOUND 04000
#define INVALID_LENTH 010000


typedef unsigned int Msg;

void inputDate(struct Date* dateout, unsigned int* message);
void inputName(char* nameout, unsigned int* message);
void inputAmount(int* amountout, unsigned int* message);
void inputType(enum ConsumeType* typeout, unsigned int* message);
void inputMethod(char* methodout, unsigned int* message);
void inputSite(char* siteout, unsigned int* message);
void inputDetail(char* detailout, unsigned int* message);

void inputLimitedChar(int lenth, char* out, unsigned int* message);







