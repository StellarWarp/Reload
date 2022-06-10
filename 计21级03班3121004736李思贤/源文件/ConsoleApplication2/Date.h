#pragma once
#include<time.h>
struct Date
{
	int y;
	int m;
	int d;
	int td;
};
void initDate(struct Date* date);
void update_Date(struct Date* date);
struct Date date_trans(char* t);
struct Date add_Date(const struct Date t1, const struct Date t2);
struct Date mine_Date(const struct Date t1, const struct Date t2);
int datecmp(const struct Date t1, const struct Date t2);

struct Date today();

