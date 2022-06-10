#pragma once
#include"File.h"
#include"Struct.h"
using namespace std;
fmanage<Record_user> recordfile;
fmanage<Member_user> memberfile;
fmanage<Family> familyfile;
fmanage<User> userfile;

void Open()
{
    recordfile.OpenFile("Data/fee.dat", "Data/FileManage/record_hole.bin");
    memberfile.OpenFile("Data/member.bin", "Data/FileManage/member_hole.bin");
    familyfile.OpenFile("Data/family.bin", "Data/FileManage /famliy_hole.bin");
    userfile.OpenFile("Data/user.bin", "Data/FileManage /user_hole.bin");

}
void Close()
{
    recordfile.CloseFile();
    memberfile.CloseFile();
    familyfile.CloseFile();
    userfile.CloseFile();
}