#pragma once

union T
{
	int i;
	char* s;
};

void QuickSort(struct Roll array[], int low, int high, union T(*visit)(struct Roll), int (*cmp)(union T, union T));

int cmp_int(union T i1, union T i2);
int cmp_str(union T s1, union T s2);
union T get_date(struct Roll info);
union T get_name(struct Roll info);
union T get_amount(struct Roll info);
union T get_type(struct Roll info);
union T get_method(struct Roll info);
union T get_site(struct Roll info);
union T get_detail(struct Roll info);

void Reverse(struct Roll array[], int low, int high);
