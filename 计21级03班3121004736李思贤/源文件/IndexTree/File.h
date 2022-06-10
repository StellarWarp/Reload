#ifndef FILE_H
#define FILE_H
#include<fstream>
#include<filesystem>
#include"Struct.h"
#include"Support.h"
#include"DataPrinter.h"
using namespace std;
void test();

unsigned long getinfo();

template<class DT>
DT* GetTextData(string filepath, unsigned long index, const int readform)
{
	ifstream datafile;
	datafile.open(filepath);
	datafile.seekg(index);
	DT* data = new DT;
	ReadForm(datafile, data, readform);
	return data;
}

//基础文件读写管理
template<class DT>
class fmanage
{
	int datalen = sizeof(DT);
	Stack<unsigned long> holestack;
	int holecount = 0;
	int datanum = 0;

	//获取储存的孔
	void OpenStackFile(string filepath)
	{
		//读入孔
		holefile.open(filepath, ios::binary | ios::in | ios::out);
		if (!holefile.is_open())
		{
			//cout << "Lack of file manage file " << filepath << "\nwill build after modify" << endl;
		}
		else
		{
			holefile.seekg(0, ios::end);
			if (holefile.tellg())
			{
				holefile.seekg(0);
				while (!holefile.eof())
				{
					unsigned long p;
					holefile.read((char*)&p, sizeof(unsigned long));
					holestack.IN(p);
					holecount++;
				}
			}
			holefile.clear();
		}
	}
	//保存孔栈数据
	void SaveStack()
	{
		//储存孔
		holefile.seekp(0);
		while (holestack.Nempty())
		{
			unsigned long p = holestack.OUT();
			holefile.write((char*)&p, sizeof(unsigned long));
		}
	}
	//获取数据量
	void GetDataNum()
	{
		datanum = 0;
		DT temp;
		file.seekg(0);
		while (!file.eof())
		{
			file.read((char*)&temp, datalen);
			datanum++;
		}
		datanum--;
		file.clear();
	}
public:

	fstream file;
	fstream holefile;
	//打开系列文件
	void OpenFile(string filepath, string fileholepath, bool Rewitre = false)
	{
		OpenStackFile(fileholepath);
		if (Rewitre)file.open(filepath, ios::binary | ios::in | ios::out | ios::trunc);
		else file.open(filepath, ios::binary | ios::in | ios::out);
		if (!file.is_open())
		{
			cout << "Fail to open " << filepath << endl;
			exit(-1);
		}
	}
	//关闭系列文件
	void CloseFile()
	{
		SaveStack();
		holefile.close();
		file.close();
	}
	~fmanage()
	{
		CloseFile();
	}

	//删除
	void FileDelete(unsigned long DataPointer)
	{
		holestack.IN(DataPointer);
		holecount++;
	}

	void FileDelete_i(unsigned long i)
	{
		holestack.IN(i*datalen);
		holecount++;
	}
	//增加
	unsigned long FileAppends(DT data)
	{
		if (holestack.Nempty())
		{
			unsigned long DataPointer = holestack.OUT();
			file.seekp(DataPointer);
			file.write((char*)&data, datalen);
			return DataPointer / datalen;
		}
		else
		{
			file.seekp(0, ios::end);
			file.write((char*)&data, datalen);
			return ((unsigned long)file.tellp() / datalen - 1);
		}
	}
	//修改
	void FileModify(DT data, unsigned long i)
	{
		file.seekp(i * datalen);
		file.write((char*)&data, datalen);
	}
	//读取
	void ReadFile(unsigned long DataPointer, DT& data)
	{
		file.seekg(DataPointer);
		file.read((char*)&data, datalen);
	}
	//用标号读取
	void ReadFile_i(unsigned long i, DT& data)
	{
		file.seekg(i * datalen);
		file.read((char*)&data, datalen);
	}
	//数据遍历//返回动态列队//文件指针//考虑抛弃
	Line<unsigned long>& FileTraversal()
	{

		unsigned long* holelist;
		if (holecount == 0)
		{
			holelist = NULL;
		}
		else
		{
			holelist = holestack.Read(holecount);
			quicksort(holelist, 0, holecount - 1);
		}

		Line<unsigned long>* DataAddress = new Line<unsigned long>;
		int n = 0;//数据编号
		int h = 0;//孔编号
		unsigned long DataPointer = n * datalen;

		GetDataNum();//需要改进

		if (holelist)
		{
			while (n < datanum)
			{
				DataPointer = n * datalen;
				if (DataPointer != holelist[h])
				{
					(*DataAddress).IN(DataPointer);
				}
				else h++;
				n++;
			}
		}
		else
		{
			while (n < datanum)
			{
				(*DataAddress).IN(n * datalen);
				n++;
			}
		}

		holestack.ReleasesReadLog();

		return *DataAddress;
	}
	//数据遍历//返回动态列队//标号
	Line<unsigned long>& FileTraversal_i()
	{

		unsigned long* holelist;
		if (holecount == 0)
		{
			holelist = NULL;
		}
		else
		{
			holelist = holestack.Read(holecount);
			quicksort(holelist, 0, holecount - 1);
		}

		Line<unsigned long>* DataAddress = new Line<unsigned long>;
		unsigned long n = 0;//数据编号
		unsigned long h = 0;//孔编号
		GetDataNum();//需要改进

		if (holelist)
		{
			while (n < datanum)
			{
				if (n != holelist[h] / datalen)
				{
					(*DataAddress).IN(n);
				}
				else h++;
				n++;
			}
		}
		else
		{
			while (n < datanum)
			{
				(*DataAddress).IN(n);
				n++;
			}
		}

		holestack.ReleasesReadLog();

		return *DataAddress;
	}
	//未完成//printfunction未写完
	void PrintFile(Line<unsigned long>& DataAddress, void (*printfunction)(DT&))
	{
		DT data;
		while (DataAddress.Nempty())
		{
			unsigned long DataPointer = DataAddress.OUT() * datalen;
			ReadFile(DataPointer, data);
			printfunction(data);
		}
	}
};

//索引文件+记录文件读写管理
template<class DT, class IT>
class RelevantInfoFile
{
	fmanage<DT> fileindex_m;
	fstream& file = file_m.file;
	fstream& fileindex = fileindex_m.file;
	int datalen = sizeof(DT);
	int indexlen = sizeof(IT);
public:
	fmanage<DT> file_m;
	void OpenFile(string datapath, string dataholepath, string indexpath, string indexholepath)
	{
		file_m.OpenFile(datapath, dataholepath);
		fileindex_m.OpenFile(indexpath, indexholepath);
	}
	void CloseFile()
	{
		file_m.CloseFile();
		fileindex_m.CloseFile();
	}
	void RFileDelete(unsigned long DataPointer, unsigned long IndexPointer)
	{
		file_m.FileDelete(DataPointer);
		file.seekg(DataPointer);
		DT data;
		//删除点
		file.read((char*)&data, datalen);
		unsigned long pdatap = data.previous;
		unsigned long ndatap = data.next;
		if (pdatap != -1)//删除点前
		{
			file.seekg(pdatap);
			file.read((char*)&data, datalen);
			data.next = ndatap;
			file.seekp(pdatap);
			file.write((char*)&data, datalen);
		}
		if (ndatap != -1)//删除点后
		{
			file.seekg(ndatap);
			file.read((char*)&data, datalen);
			data.previous = pdatap;
			file.seekp(ndatap);
			file.write((char*)&data, datalen);
		}
		//读取索引
		IT index;
		fileindex.seekg(IndexPointer);
		file.read((char*)&index, indexlen);
		//索引重定向
		if (index.record == DataPointer)
		{
			index.record = pdatap;
			fileindex.seekp(IndexPointer);
			fileindex.write((char*)&index, indexlen);
		}
	}
	//末增（不可中间插入）
	void RFileAppend(DT data, unsigned long IndexPointer)
	{
		//读取当前节点
		IT index;
		fileindex.seekg(IndexPointer);
		file.read((char*)&index, indexlen);
		//设置插入点指针
		data.previous = index.record;
		data.next = -1;
		//插入节点并设置索引
		index.record = file_m.FileAppend(data);
		fileindex.seekp(IndexPointer);
		fileindex.write((char*)&index, indexlen);
	}

	void ReadDataFile(unsigned long DataPointer, DT& data)
	{
		file_m.ReadFile(DataPointer, data);
	}
	//通过序列数读取最晚数据
	void ReadLatestRecord(int n, DT& data)
	{
		//获取指针
		IT index;
		fileindex.seekg(n * indexlen);
		fileindex.read((char*)&index, indexlen);
		//读取数据
		file.seekg(index.record);
		file.read((char*)&data, datalen);
	}
	//打印数据
	void PrintData(void (*printfunction)(DT&))
	{
		file_m.PrintFile(file_m.FileTraversal(), printfunction);
	}

	Line<unsigned long>& RecordTraversal()
	{
		return file_m.FileTraversal();
	}
};


#endif // !FILE_H
