#pragma once
#include"File.h"
#include"Index.h"
#include"Index_s.h"
#include"StructOP.h"
#include"Filter.h"
#include"Printer.h"
#include"DataManage.h"
using namespace std;

Index<systemID> User_i_;
Index<systemID> Family_i_;
Index<systemID> Member_i_;
Index<systemID> User_family_i_;
Index<systemID> Record_i_;
Index<systemID> Record_user_i_;

Index_s<systemID> User_i;
Index_s<systemID> Family_i;
Index_s<systemID> Member_i;
Index_s<systemID> User_family_i;
Index_s<systemID> Record_i;
Index_s<systemID> Record_user_i;

namespace mode
{
    int normal = 1;
    int range = 2;
}
namespace object
{
    int name = 1;
    int id = 2;
    int age = 4;
    int income = 8;
    int phone = 16;
    int Date = 32;
    int Category = 64;
    int Time = 128;
};
class IndexManage
{
    template<class ST, class DT>
    void IndexConstruct(fmanage<ST>& file, Index< DT >& index, DT(*opfunc)(ST&))
    {
        Line<unsigned long>& DataLine = file.FileTraversal_i();
        ST data;
        DT key;
        unsigned long DataPointer;
        while (DataLine.Nempty())
        {
            DataPointer = DataLine.OUT();
            file.ReadFile_i(DataPointer, data);
            key = opfunc(data);
            index.append(DataPointer, key);
        }
    }

    void BuildIndex()
    {
        IndexConstruct(recordfile, Record_i_, record_id);
        IndexConstruct(recordfile, Record_user_i_, record_mid);
        IndexConstruct(memberfile, Member_i_, member_id);
        IndexConstruct(userfile, User_family_i_, user_fid);
        IndexConstruct(familyfile, Family_i_, family_id);
        IndexConstruct(userfile, User_i_, user_id);
    }


    static const int index_num = 6;
    string index_name[index_num] =
    { "user_id","family_id","member_id","user_fid","record_id","record_mid"};

    bool is_exi(string& name)
    {
        ifstream test;
        string path[3];
        path[0] = "Index/" + name + "LayerN.bin";
        path[1] = "Index/" + name + "LayerData.bin";
        path[2] = "Index/" + name + "info.bin";
        for (int i = 0; i < 3; i++)
        {
            test.open(path[i]);
            if (!test.is_open())
            {
                return false;
                cout << "Fail to import index , building index...." << endl;
            }
            test.close();
        }
        return true;
    }

    void ImportIndex_p()
    {
        bool error = false;
        for (int i = 0; i < index_num; i++)
        {
            if (!is_exi(index_name[i]))
            {
                error = true;
                memorymode = true;
                break;
            }
        }
        if (error)
        {
            ReBuild();
        }
        else
        {
            User_i.Import("user_id");
            Family_i.Import("family_id");
            Member_i.Import("member_id");
            User_family_i.Import("user_fid");
            Record_i.Import("record_id");
            Record_user_i.Import("record_mid");

        }
    }

public:
    bool memorymode = false;

    void ImportIndex()
    {
        ImportIndex_p();
    }
    void modeswitch(bool mode)
    {
        cout << "ModeSwitched, memorymode: " << memorymode << endl;
        if (memorymode == true && mode == false)
        {
            memorymode = false;
            SaveIndex();
        }
    }
    void SaveIndex()
    {
        User_i_.SaveIndex("user_id");
        Family_i_.SaveIndex("family_id");
        Member_i_.SaveIndex("member_id");
        User_family_i_.SaveIndex("user_fid");
        Record_i_.SaveIndex("record_id");
        Record_user_i_.SaveIndex("record_mid");
    }
    void ReBuild()
    {
        BuildIndex();
        SaveIndex();
        ImportIndex_p();
    }
    void search(int mode,int obj)
    {
        if (mode % 2)
        {
            cout << "percice search" << endl;
        }
        else
        {
            cout << "range search" << endl;
        }
        if (mode & 1)
        {

        }
    }
    void satistic(int mode, int obj)
    {

    }
    void AddRecord(unsigned long DataPointer,systemID userid, systemID recordid)
    {
        Record_i_.append(DataPointer, recordid); 
        Record_user_i_.append(DataPointer, userid);
    }
    void DeleteRecord(unsigned long DataPointer, systemID userid, systemID recordid)
    {
        Record_i_.data_delete(DataPointer, recordid);
        Record_user_i_.data_delete(DataPointer, userid);
    }
};
