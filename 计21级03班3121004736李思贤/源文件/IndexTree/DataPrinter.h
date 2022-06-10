#ifndef DATAPRINTER_H
#define DATAPRINTER_H
#include"Struct.h"
using namespace std;

template<class DT>
void ReadForm(ifstream& datafile, DT* data, const int& readform)
{
	switch (readform)
	{
	case 1:
	{
		datafile >> data->person.name;
		datafile >> data->person.ID;
		datafile >> data->person.age;
		datafile >> data->person.gender;
		datafile >> data->person.phone;
		datafile >> data->person.income;
		datafile >> data->person.address;
	}; break;
	}
}

union pf
{
	void(*pdata)(struct data&);
	void(*pperson)(struct persondata&);
	void(*pscore)(struct scoredata&);
};
//template<class T>
//void(*)(T&) (int n)
//{
//	switch (n)
//	{
//	case 1:funcp.pdata = Print_s_data; break;
//	case 2:funcp.pperson = Print_s_person; break;
//	case 3:break;
//	default:break;
//	}
//}
//template<class T>
//void PrintForm(T& data, void (*fp)(T&))
//{
//	fp(data);
//	
//}

template<class DT>
void Print_s_data(DT& data)
{
	cout << data.person.name << " ";
	cout << data.person.ID << " ";
	cout << data.person.age << " ";
	cout << data.person.gender << " ";
	cout << data.person.phone << " ";
	cout << data.person.income << " ";
	cout << data.person.address << endl;
}

template<class DT>
void Print_s_person(DT& data)
{
	cout << data.name << " ";
	cout << data.ID << " ";
	cout << data.age << " ";
	cout << data.gender << " ";
	cout << data.phone << " ";
	cout << data.income << " ";
	cout << data.address << endl;
}

template<class DT>
void Print_s_record(DT& data)
{
	cout << data.name << " ";
	cout << data.ID << " ";
	cout << data.date << " ";
	cout << data.category << " ";
	cout << data.time << " ";
	cout << data.score_add << " ";
	cout << data.score_remain << " ";
	cout << data.score_sub << " ";
	cout << data.score_sub_t << " ";
	cout << data.mark << endl;
}


#endif // !DATAPRINTER_H

