#include<stdio.h>
#include<string.h>
#include"ConsoleControl.h"
#include"Struct.h"
#include"Transcode.h"
#include"Input.h"
#define MAXINPUT 20
#define PWLENTH 6

void inputDate(struct Date* dateout, unsigned int* message)
{
	char inputcache[101] = { 0 };
	struct Date date;
	*message = NULL;
	scanf_s("%[^\n]", inputcache, 101);
	//if (inputcache[0] == 0)
	getchar();
	if (strlen(inputcache) != 10)
	{
		*message = INVALID_DATE;
		return;
	}
	date = date_trans(inputcache);
	if (date.y < 1970 || date.y>9999)
	{
		*message = INVALID_DATE;
		return;
	}
	if (date.m < 1 || date.m>12)
	{
		*message = INVALID_DATE;
		return;
	}
	int month[12] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
	if (date.y % 400 == 0 || date.y % 100 != 0 && date.y % 4 == 0) month[1] = 29; else month[1] = 28;
	if (date.d<1 || date.d>month[date.m - 1])
	{
		*message = INVALID_DATE;
		return;
	}
	*dateout = date;
}

void inputName(char* nameout, unsigned int* message)
{
	char inputcache[101] = { 0 };
	*message = NULL;
	SetConsoleCP(936);
	scanf_s("%[^\n]", inputcache, 101);
	if (inputcache[0] != 0)
		transcode(inputcache);

	getchar();
	SetConsoleCP(65001);
	if (strlen(inputcache) >= LEN_NAME)
	{
		*message = INVALID_NAME;
		return;
	}
	strcpy_s(nameout, LEN_NAME, inputcache);
}

void inputAmount(int* amountout, unsigned int* message)
{
	char inputcache[101] = { 0 };
	double inputcachef = -1;
	*message = NULL;
	scanf_s("%[^\n]", inputcache, 101);
	if (inputcache[0] != 0)
	{
		inputcachef = atof(inputcache);
	}
	getchar();

	/**message = NULL;
	int status= scanf_s("%f\n", &inputcache);;*/
	/*if (status == EOF || status  == 0)
	{
		char in;
		while ((in = getchar()) != '\n' && in != EOF);
	}*/
	if (inputcachef < 0)
	{
		*message = INVALID_AMOUNT;
		return;
	}
	*amountout = inputcachef * 100;
}

void inputType(enum ConsumeType* typeout, unsigned int* message)
{
	char inputcache[101] = { 0 };
	*message = NULL;
	SetConsoleCP(936);
	scanf_s("%[^\n]", inputcache, 101);
	if (inputcache[0] != 0)
		transcode(inputcache);

	getchar();
	SetConsoleCP(65001);
	if (strlen(inputcache) > 15)
	{
		*message = INVALID_TYPE;
		return;
	}
	if (strcmp(inputcache, "食品") == 0)
	{
		*typeout = 食品;
		return;
	}
	if (strcmp(inputcache, "日用品") == 0)
	{
		*typeout = 日用品;
		return;
	}
	if (strcmp(inputcache, "服装") == 0)
	{
		*typeout = 服装;
		return;
	}
	if (strcmp(inputcache, "电器") == 0)
	{
		*typeout = 电器;
		return;
	}
	if (strcmp(inputcache, "水电煤气") == 0)
	{
		*typeout = 水电煤气;
		return;
	}
	if (strcmp(inputcache, "房贷") == 0)
	{
		*typeout = 房贷;
		return;
	}
	if (strcmp(inputcache, "学费") == 0)
	{
		*typeout = 学费;
		return;
	}
	if (strcmp(inputcache, "交通费") == 0)
	{
		*typeout = 交通费;
		return;
	}
	*message = INVALID_TYPE;
}

void inputMethod(char* methodout, unsigned int* message)
{
	char inputcache[101] = { 0 };
	*message = NULL;
	SetConsoleCP(936);
	scanf_s("%[^\n]", inputcache, 101);
	if (inputcache[0] != 0)
		transcode(inputcache);

	getchar();
	SetConsoleCP(65001);
	if (strlen(inputcache) >= MAXINPUT)
	{
		*message = INVALID_METHOD;
		return;
	}
	strcpy_s(methodout, 20, inputcache);
}

void inputSite(char* siteout, unsigned int* message)
{
	char inputcache[101] = { 0 };
	*message = NULL;
	SetConsoleCP(936);
	scanf_s("%[^\n]", inputcache, 101);
	if (inputcache[0] != 0)
		transcode(inputcache);

	getchar();
	SetConsoleCP(65001);
	if (strlen(inputcache) >= MAXINPUT)
	{
		*message = INVALID_SITE;
		return;
	}
	strcpy_s(siteout, 30, inputcache);
}

void inputDetail(char* detailout, unsigned int* message)
{
	char inputcache[101] = { 0 };
	*message = NULL;
	SetConsoleCP(936);
	scanf_s("%[^\n]", inputcache, 101);
	if (inputcache[0] != 0)
		transcode(inputcache);

	getchar();
	SetConsoleCP(65001);
	if (strlen(inputcache) >= MAXINPUT)
	{
		*message = INVALID_DETAIL;
		return;
	}
	strcpy_s(detailout, 30, inputcache);
}

void inputLimitedChar(int lenth, char* out, unsigned int* message)
{
	char inputcache[101] = { 0 };
	*message = NULL;
	int inputlenth = 0;
	char* p = inputcache;
	for (;;)
	{
		char c = getch_Console();
		if (c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z' || c >= '0' && c <= '9')
		{
			if (inputlenth < lenth)
			{
				*p = c;
				putchar(c);
				inputlenth++;
				p++;
			}
		}
		else if (c == '\r')
		{
			if(inputlenth !=  lenth)*message |= INVALID_LENTH;
			break;
		}
		else if (c == '\b')
		{
			if (inputlenth > 0)
			{
				putchar('\b');
				putchar(' ');
				putchar('\b');
				inputlenth--;
				p--;
				*p = '/0';
			}
		}
		else if (c == 27)
		{
			*message |= END_INPUT;
			break;
		}
		else if (c == '\t')
		{
			if (inputlenth != lenth)*message |= INVALID_LENTH;
			*message |= GONEXT;
			break;
		}
	}

	strcpy_s(out, lenth + 1, inputcache);
}