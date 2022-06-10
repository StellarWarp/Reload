#include"Struct.h"

struct Record initRecord()
{
	struct Record record;
	record.systemid = -1;
	record.membersystemid = -1;
	record.date = today();
	record.name[0] = 0;
	record.amount = 0;
	record.type = ？;
	strcpy_s(record.method,20,"");
	strcpy_s(record.site, 30, "");
	strcpy_s(record.detail, 50, "");
	record.prev = NULL;
	record.next = NULL;
	return record;
}