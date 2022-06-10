#pragma once
#include"Struct.h"
#include"TypeSupport.h"
#include<iostream>

//访问函数
systemID inline record_id(Record_user& node)
{
	return node.systemid;
}
systemID inline record_mid(Record_user& node)
{
	return node.membersystemid;
}
systemID inline member_id(Member_user& node)
{
	return node.systemid;
}
systemID inline family_id(Family& node)
{
	return node.systemid;
}
systemID inline user_fid(User& node)
{
	return node.familyid;
}
systemID inline user_id(User& node)
{
	return node.systemid;
}
