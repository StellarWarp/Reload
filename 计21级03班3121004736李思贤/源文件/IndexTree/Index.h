/*
Index
用于内存索引的使用
*/


#ifndef INDEX_H
#define INDEX_H
#include"Struct.h"
#include"Support.h"
#include"File.h"
#include"Define.h"//M
#include"VisualTree.h"
#include<string>
using namespace std;

template<class DT>//关键字的类型
class Index
{

	//参数
	static const int min_key = (M % 2) ? M / 2 + 1 : M / 2;//m/2向上取整
	static const int max_key = M - 1;
	static const int min_i = min_key - 1;
	static const int max_i = max_key - 1;
	int H = 0;//树高

	//结构
	union Pointer;//声明
	struct LayerData//子叶节点
	{
		DT key[max_key] = {};
		unsigned long DataPointer[max_key] = {};
		struct LayerData* next = NULL;
		int num = 0;
	};
	struct LayerN
	{
		DT key[max_key] = {};
		unsigned long DataPointer[max_key] = {};//用于辨识
		Pointer NodePointer[M] = {};
		int num = 0;
	};
	Stack<LayerN*> Path;
	Stack<int> Pathi;
	union Pointer//节点间指针
	{
		LayerData* Dp;
		LayerN* Np = NULL;

		Pointer()
		{
			;
		}
		Pointer(LayerData* P)
		{
			Dp = P;
		}
		Pointer(LayerN* P)
		{
			Np = P;
		}
	};
	Pointer* Master;//根节点指针

	//LayerData的操作函数
	//查找数据
	void Find(LayerData& Node, DT value, Line<unsigned long>& DataAddress)
	{
		int i = DilitarySearch_down(Node.key, 0, Node.num - 1, value);
		if (value == Node.key[i])
		{
			DataAddress.IN(Node.DataPointer[i]);
			//同值查找
			i++;
			if (i < Node.num && value == Node.key[i])
			{
				while (i < Node.num && value == Node.key[i])
				{
					DataAddress.IN(Node.DataPointer[i]);
					i++;
				}
				if (i == Node.num && Node.next)
				{

					Find(*(Node.next), value, DataAddress);
				}
			}
			else if (Node.next && i == Node.num)
			{
				Find(*(Node.next), value, DataAddress);
			}
		}
	}
	void RangeFind(LayerData& Node, DT low_value, DT top_value, Line<unsigned long>& DataAddress)
	{
		int i = DilitarySearch_down(Node.key, 0, Node.num - 1, low_value);
		LayerData* CNode = &Node;
		DT Ckey = CNode->key[i];
		while (Ckey <= top_value)
		{
			if (i < CNode->num - 1)//未到边界
			{
				DataAddress.IN(CNode->DataPointer[i]);
				i++;
				Ckey = CNode->key[i];
			}
			else
			{
				CNode = CNode->next;
				if (!CNode) break;
				i = 0;
				Ckey = CNode->key[i];
			}
		}
	}
	//统计数据
	void Satistic(LayerData& Node, DT X[], DT low_key, DT top_key, int Y[])
	{
		int i = DilitarySearch_down(Node.key, 0, Node.num - 1, low_key);
		LayerData* CNode = &Node;
		int x = 0;
		int y = 0;
		DT Ckey = CNode->key[i];
		DT Cvalue = Ckey;
		while (Ckey <= top_key)
		{
			if (i < CNode->num - 1)//未到边界
			{
				if (Ckey > Cvalue)
				{
					Y[x] = y;
					X[x] = Cvalue;
					x++;
					y = 0;
					Cvalue = Ckey;
				}
				y++;
				i++;
				Ckey = CNode->key[i];
			}
			else
			{
				if (Ckey > Cvalue)
				{
					Y[x] = y;
					X[x] = Cvalue;
					x++;
					y = 0;
					Cvalue = Ckey;
				}
				y++;
				CNode = CNode->next;
				if (!CNode) break;
				i = 0;
				Ckey = CNode->key[i];
			}
		}
		Y[x] = y;
		X[x] = Cvalue;
	}
	void Satistic_range(LayerData& Node, DT X[], DT low_key, DT top_key, DT gap, int Y[])
	{
		int i = DilitarySearch_down(Node.key, 0, Node.num - 1, low_key);
		LayerData* CNode = &Node;
		int x = 0;
		int y = 0;
		DT Ckey = CNode->key[i];
		DT StepValue = low_key;
		while (Ckey <= top_key)
		{
			if (i < CNode->num - 1)//未到边界
			{
				if (Ckey > StepValue + gap)
				{
					Y[x] = y;
					X[x] = StepValue;
					StepValue += gap;
					x++;
					y = 0;
				}
				y++;
				i++;
				Ckey = CNode->key[i];
			}
			else
			{
				if (Ckey > StepValue + gap)
				{
					Y[x] = y;
					X[x] = StepValue;
					StepValue += gap;
					x++;
					y = 0;
				}
				y++;
				CNode = CNode->next;
				if (!CNode) break;
				i = 0;
				Ckey = CNode->key[i];
			}
		}
		Y[x] = y;
		X[x] = StepValue;
	}

	//子叶节点操作
	bool isfull(LayerData& Node)
	{
		if (Node.num == max_key) return 1;
		else return 0;
	}
	bool islow(LayerData& Node)
	{
		if (Node.num < min_key) return 1;
		else return 0;
	}
	bool ismore(LayerData& Node)
	{
		if (Node.num > min_key) return 1;
		else return 0;
	}
	bool Insert(LayerData& Node, unsigned long DP, DT value)
	{
		int in = DilitarySearch_SecondOrder_down(Node.key, Node.DataPointer, 0, Node.num - 1, value, DP);
		if (Node.key[in] == value && Node.DataPointer[in] < DP)
		{
			in++;
		}
		InsertArray(Node.key, in, max_i, value);
		InsertArray(Node.DataPointer, in, max_i, DP);
		Node.num++;
		if (in == Node.num - 1) return 1;
		else return 0;

	}
	Pointer Divide(LayerData& Node, unsigned long DP, DT value)
	{
		LayerData* NewNode = new LayerData;
		int in = DilitarySearch_SecondOrder_down(Node.key, Node.DataPointer, 0, max_i, value, DP);
		if (Node.key[in] == value && Node.DataPointer[in] < DP)
		{
			in++;
		}
		Node.num = M / 2;
		NewNode->num = min_key;
		if (in <= min_i)
		{
			MoveArray(Node.key, min_i, max_i, NewNode->key);//将原节点多出的移动到新的节点
			MoveArray(Node.DataPointer, min_i, max_i, NewNode->DataPointer);
			SetArray(Node.key, min_i, max_i, DT());
			SetArray(Node.DataPointer, min_i, max_i, 0UL);
			InsertArray(Node.key, in, min_i, value);
			InsertArray(Node.DataPointer, in, min_i, DP);

		}
		else
		{
			MoveArray(Node.key, min_key, max_i, NewNode->key);
			MoveArray(Node.DataPointer, min_key, max_i, NewNode->DataPointer);
			SetArray(Node.key, min_key, max_i, DT());
			SetArray(Node.DataPointer, min_key, max_i, 0UL);
			InsertArray(NewNode->key, in - min_key, min_i, value);
			InsertArray(NewNode->DataPointer, in - min_key, min_i, DP);
		}
		NewNode->next = Node.next;
		Node.next = NewNode;
		return Pointer(NewNode);
	}
	int Delete(LayerData& Node, DT value, unsigned long DP)
	{
		Node.num--;
		int del = DilitarySearch_SecondOrder_down(Node.key, Node.DataPointer, 0, Node.num, value, DP);
		DeleteArray(Node.key, del, Node.num, (DT)0);
		DeleteArray(Node.DataPointer, del, Node.num, (unsigned long)0);
		return del;
	}
	void Shift(LayerData& Node1, int a1, int b1, LayerData& Node2, int a2)
	{
		MoveArray(Node1.key, a1, b1, Node2.key, a2);
		MoveArray(Node1.DataPointer, a1, b1, Node2.DataPointer, a2);
		int num = b1 - a1 + 1;
		Node1.num -= num;
		Node2.num += num;
	}

	//LayerN的操作函数
	bool isfull(LayerN& Node)
	{
		if (Node.num == max_key) return 1;
		else return 0;
	}
	bool islow(LayerN& Node)
	{
		if (Node.num < min_key) return 1;
		else return 0;
	}
	bool ismore(LayerN& Node)
	{
		if (Node.num > min_key) return 1;
		else return 0;
	}
	LayerN* newLayer(Pointer P1, DT value1, unsigned long DP1, Pointer P2, DT value2, unsigned long DP2)
	{
		LayerN* NewNode = new LayerN;
		NewNode->key[0] = value1;
		NewNode->key[1] = value2;
		NewNode->NodePointer[0] = P1;
		NewNode->NodePointer[1] = P2;
		NewNode->DataPointer[0] = DP1;
		NewNode->DataPointer[1] = DP2;
		NewNode->num = 2;
		return NewNode;
	}
	//返回是否最大值
	bool Insert(LayerN& Node, Pointer P, DT value, unsigned long DP)
	{
		int in = DilitarySearch_SecondOrder_down(Node.key, Node.DataPointer, 0, Node.num - 1, value, DP);
		if (Node.key[in] == value && Node.DataPointer[in] < DP)
		{
			in++;
		}
		InsertArray(Node.key, in, max_i, value);
		InsertArray(Node.NodePointer, in, max_i, P);
		InsertArray(Node.DataPointer, in, max_i, DP);
		Node.num++;
		if (in == Node.num - 1) return 1;
		else return 0;
	}
	//更新并回溯
	bool update(LayerN* Node, int i, DT value, unsigned long DP)
	{
		if (i == Node->num - 1)
		{
			Node->key[i] = value;
			Node->DataPointer[i] = DP;
			if (Path.Nempty())
			{
				update(Path.OUT(), Pathi.OUT(), value, DP);
			}
			return 1;
		}
		else
		{
			Node->DataPointer[i] = DP;
			Node->key[i] = value;
			return 0;
		}
	}
	//不回溯
	void update0(LayerN* Node, int i, DT value, unsigned long DP)
	{
		Node->key[i] = value;
		Node->DataPointer[i] = DP;
	}
	//分裂并增加
	Pointer Divide(LayerN& Node, Pointer P, DT value, unsigned long DP)
	{
		LayerN* NewNode = new LayerN;
		int in = DilitarySearch_SecondOrder_down(Node.key, Node.DataPointer, 0, max_i, value, DP);
		if (Node.key[in] == value && Node.DataPointer[in] < DP)
		{
			in++;
		}
		Node.num = M / 2;
		NewNode->num = min_key;
		if (in <= min_i)
		{
			MoveArray(Node.key, min_i, max_i, NewNode->key);//将原节点多出的移动到新的节点
			MoveArray(Node.NodePointer, min_i, max_i, NewNode->NodePointer);
			MoveArray(Node.DataPointer, min_i, max_i, NewNode->DataPointer);
			SetArray(Node.key, min_i, max_i, DT());
			SetArray(Node.NodePointer, min_i, max_i, Pointer());
			SetArray(Node.DataPointer, min_i, max_i, 0UL);
			InsertArray(Node.key, in, min_i, value);
			InsertArray(Node.NodePointer, in, min_i, P);
			InsertArray(Node.DataPointer, in, min_i, DP);
		}
		else
		{
			MoveArray(Node.key, min_key, max_i, NewNode->key);
			MoveArray(Node.NodePointer, min_key, max_i, NewNode->NodePointer);
			MoveArray(Node.DataPointer, min_key, max_i, NewNode->DataPointer);
			SetArray(Node.key, min_key, max_i, DT());
			SetArray(Node.NodePointer, min_key, max_i, Pointer());
			SetArray(Node.DataPointer, min_key, max_i, 0UL);
			InsertArray(NewNode->key, in - min_key, min_i, value);
			InsertArray(NewNode->NodePointer, in - min_key, min_i, P);
			InsertArray(NewNode->DataPointer, in - min_key, min_i, DP);
		}
		return Pointer(NewNode);
	}
	//删除并返回删除index
	int Delete(LayerN& Node, DT value, unsigned long DP)
	{
		Node.num--;
		int del = DilitarySearch_SecondOrder_down(Node.key, Node.DataPointer, 0, Node.num, value, DP);
		DeleteArray(Node.key, del, Node.num, (DT)0);
		Pointer Empty;
		DeleteArray(Node.NodePointer, del, Node.num, Empty);
		DeleteArray(Node.DataPointer, del, Node.num, (unsigned long)0);
		return del;
	}
	//删除index
	void Delete(LayerN& Node, int i)
	{
		Node.num--;
		int del = i;
		DeleteArray(Node.key, del, Node.num, (DT)0);
		Pointer Empty;
		DeleteArray(Node.NodePointer, del, Node.num, Empty);
		DeleteArray(Node.DataPointer, del, Node.num, (unsigned long)0);
	}
	//将1移到2
	void Shift(LayerN& Node1, int a1, int b1, LayerN& Node2, int a2)
	{
		MoveArray(Node1.key, a1, b1, Node2.key, a2);
		MoveArray(Node1.NodePointer, a1, b1, Node2.NodePointer, a2);
		MoveArray(Node1.DataPointer, a1, b1, Node2.DataPointer, a2);
		int num = b1 - a1 + 1;
		Node1.num -= num;
		Node2.num += num;
	}

	//函数
	LayerData* FIndLayerData(DT key, unsigned long DP)
	{
		while (Path.Nempty())
		{
			Path.OUT();
			Pathi.OUT();
		}
		Pointer p = *Master;
		int h = H;
		if (h > 1)
		{
			DT maxkey = Master->Np->key[Master->Np->num - 1];
			if (key > maxkey)
			{
				key = maxkey;
				unsigned long maxPD = Master->Np->DataPointer[Master->Np->num - 1];
				if (DP > maxPD)
				{
					DP = maxPD;
				}
			}
		}
		while (h > 1)
		{
			Path.IN(p.Np);
			int i = DilitarySearch_SecondOrder_down(p.Np->key, p.Np->DataPointer, 0, p.Np->num - 1, key, DP);//建立的时候出错//第二指标排序错误
			p = p.Np->NodePointer[i];
			Pathi.IN(i);
			h--;
		}
		return p.Dp;

	};
	void LayerNAdd(Pointer P, DT key, unsigned long DP)
	{
		LayerN& CLayer = *Path.OUT();
		Pathi.OUT();
		if (isfull(CLayer))
		{
			Pointer NLayer = Divide(CLayer, P, key, DP);
			if (Master->Np == &CLayer)
			{
				Pointer C(&CLayer);
				delete Master;
				Master = new Pointer(newLayer(C, CLayer.key[min_i], CLayer.DataPointer[min_i],
					NLayer, NLayer.Np->key[min_i], NLayer.Np->DataPointer[min_i]));
				H++;
			}
			else
			{
				update0(Path.Read0(), Pathi.Read0(), CLayer.key[min_i], CLayer.DataPointer[min_i]);
				LayerNAdd(NLayer, NLayer.Np->key[min_i], NLayer.Np->DataPointer[min_i]);
			}
		}
		else
		{
			if (Insert(CLayer, P, key, DP))
			{
				//更新key的函数
				if (Path.Nempty())
				{
					update(Path.OUT(), Pathi.OUT(), key, DP);
				}
			}
		}
	}
	void LayerDataAdd(unsigned long DataPointer, DT key)
	{
		if (H == 0)
		{
			Master = new Pointer;
			Master->Dp = new LayerData;
			Insert(*(Master->Dp), DataPointer, key);
			H++;
		}
		else
		{
			LayerData& CLayer = *FIndLayerData(key, DataPointer);
			if (isfull(CLayer))
			{
				Pointer NLayer = Divide(CLayer, DataPointer, key);
				if (H == 1)
				{
					Pointer C(&CLayer);
					delete Master;
					Master = new Pointer(newLayer(C, CLayer.key[min_i], CLayer.DataPointer[min_i],
						NLayer, NLayer.Dp->key[min_i], NLayer.Dp->DataPointer[min_i]));
					H++;
				}
				else
				{
					update0(Path.Read0(), Pathi.Read0(), CLayer.key[min_i], CLayer.DataPointer[min_i]);
					LayerNAdd(NLayer, NLayer.Dp->key[min_i], NLayer.Dp->DataPointer[min_i]);
				}
			}
			else
			{
				if (Insert(CLayer, DataPointer, key))
				{
					//更新key的函数
					if (Path.Nempty())
					{
						update(Path.OUT(), Pathi.OUT(), key, DataPointer);
					}
				}
			}

		}
	}

	//删除
	void LayerNDelete(int del)
	{
		LayerN& CLayer = *Path.OUT();
		Pathi.OUT();
		DT keymax = CLayer.key[min_i];
		unsigned long DPmax = CLayer.DataPointer[min_i];
		Delete(CLayer, del);
		if (Path.Nempty())
		{
			if (CLayer.num < min_key)
			{
				LayerN& FNode = *Path.Read0();
				int Ci = Pathi.Read0();
				LayerN* BNode;
				int Bi;
				if (Ci == 0)
				{//右 ->
					Bi = Ci + 1;
					BNode = FNode.NodePointer[Bi].Np;
					if (ismore(*BNode))//借用
					{
						int i = 0;
						Shift(*BNode, i, i, CLayer, min_i);
						Shift(*BNode, 1, BNode->num, *BNode, 0);
						update0(Path.OUT(), Pathi.OUT(), CLayer.key[min_i], CLayer.DataPointer[min_i]);
					}
					else//合并
					{
						Shift(*BNode, 0, min_i, CLayer, min_i);
						update0(&FNode, Ci, CLayer.key[max_i], CLayer.DataPointer[max_i]);
						LayerNDelete(Bi);
						delete BNode;
					}
				}
				else
				{//左 <-
					Bi = Ci - 1;
					BNode = FNode.NodePointer[Bi].Np;
					if (ismore(*BNode))//借用
					{
						int i = --BNode->num;//数量减一
						Insert(CLayer, BNode->NodePointer[i], BNode->key[i], BNode->DataPointer[i]);
						update0(&FNode, Bi, BNode->key[i - 1], BNode->DataPointer[i - 1]);
					}
					else//合并
					{
						Shift(CLayer, 0, min_i - 1, *BNode, min_i + 1);
						update0(&FNode, Bi, BNode->key[max_i], BNode->DataPointer[max_i]);
						LayerNDelete(Ci);
						delete& CLayer;
					}
				}
			}
			else if (del == CLayer.num)
			{
				update(Path.OUT(), Pathi.OUT(), CLayer.key[CLayer.num - 1], CLayer.DataPointer[CLayer.num - 1]);
			}
		}
		else if (CLayer.num == 1)
		{
			Pointer* NewMaster = new Pointer(CLayer.NodePointer[0]);
			delete Master->Np;
			delete Master;
			Master = NewMaster;
			H--;
		}

	}
	void LayerDataDelete(unsigned long DataPointer, DT key)
	{
		LayerData& CLayer = *FIndLayerData(key, DataPointer);
		DT keymax = CLayer.key[min_i];
		DT DPmax = CLayer.DataPointer[min_i];
		int del = Delete(CLayer, key, DataPointer);
		if (Path.Nempty())
		{
			if (CLayer.num < min_key)
			{
				LayerN& FNode = *Path.Read0();
				int Ci = Pathi.Read0();
				LayerData* BNode;
				int Bi;
				if (Ci == 0)
				{//右 ->
					Bi = Ci + 1;
					BNode = FNode.NodePointer[Bi].Dp;
					if (ismore(*BNode))//借用
					{
						int i = 0;
						Shift(*BNode, i, i, CLayer, min_i);
						Shift(*BNode, 1, BNode->num, *BNode, 0);
						update0(Path.OUT(), Pathi.OUT(), CLayer.key[min_i], CLayer.DataPointer[min_i]);
					}
					else//合并
					{
						Shift(*BNode, 0, min_i, CLayer, min_i);
						update0(&FNode, Ci, CLayer.key[max_i], CLayer.DataPointer[max_i]);
						LayerNDelete(Bi);
						CLayer.next = BNode->next;
						delete BNode;
					}
				}
				else
				{//左 <-
					Bi = Ci - 1;
					BNode = FNode.NodePointer[Bi].Dp;
					if (ismore(*BNode))//借用
					{
						int i = --BNode->num;
						Insert(CLayer, BNode->DataPointer[i], BNode->key[i]);
						update0(&FNode, Bi, BNode->key[i - 1], BNode->DataPointer[i - 1]);
					}
					else//合并
					{
						Shift(CLayer, 0, min_i - 1, *BNode, min_i + 1);
						update0(&FNode, Bi, BNode->key[max_i], BNode->DataPointer[max_i]);
						LayerNDelete(Ci);
						BNode->next = CLayer.next;
						delete& CLayer;
					}
				}
			}
			else if (del == CLayer.num)
			{
				update(Path.OUT(), Pathi.OUT(), CLayer.key[CLayer.num - 1], CLayer.DataPointer[CLayer.num - 1]);
			}
		}
	}

	//保存
	unsigned long SaveLayerData(fmanage<Index_LayerData<DT>>& file, Pointer P)
	{
		Index_LayerData<DT> LayerData_s;
		MoveArray(P.Dp->key, 0, max_i, LayerData_s.key, 0);
		MoveArray(P.Dp->DataPointer, 0, max_i, LayerData_s.DataPointer, 0);
		LayerData_s.num = P.Dp->num;
		unsigned long p = file.FileAppends(LayerData_s);
		if (P.Dp->next)
		{
			LayerData_s.next = p + 1;
		}
		else
		{
			LayerData_s.next = -1;
		}
		file.FileModify(LayerData_s, p);
		return p;
	}
	unsigned long SaveLayerN(fmanage<Index_LayerN<DT>>& fileN, fmanage<Index_LayerData<DT>>& fileD, Pointer P, int Layer)
	{
		if (Layer > 2)
		{
			Index_LayerN<DT> LayerN_s;
			MoveArray(P.Np->key, 0, max_i, LayerN_s.key, 0);
			MoveArray(P.Np->DataPointer, 0, max_i, LayerN_s.DataPointer, 0);
			LayerN_s.num = P.Np->num;
			for (int i = 0; i < P.Np->num; i++)
			{
				LayerN_s.NodePointer[i] = SaveLayerN(fileN, fileD, P.Np->NodePointer[i], Layer - 1);
			}
			unsigned long p = fileN.FileAppends(LayerN_s);
			return p;
		}
		else//layer == 2
		{
			Index_LayerN<DT> LayerN_s;
			MoveArray(P.Np->key, 0, max_i, LayerN_s.key, 0);
			MoveArray(P.Np->DataPointer, 0, max_i, LayerN_s.DataPointer, 0);
			LayerN_s.num = P.Np->num;
			for (int i = 0; i < P.Np->num; i++)
			{
				LayerN_s.NodePointer[i] = SaveLayerData(fileD, P.Np->NodePointer[i]);//next linking problme
			}
			unsigned long p = fileN.FileAppends(LayerN_s);
			return p;
		}
	}
	void createfile(string& path)
	{
		ofstream test;
		test.open(path);
		if (!test.is_open())
		{
			test << "!" << endl;
		}
		test.close();
	}

public:
	void append(unsigned long DataPointer, DT key)
	{
		LayerDataAdd(DataPointer, key);
	}
	void data_delete(unsigned long DataPointer, DT key)
	{
		LayerDataDelete(DataPointer, key);
	}
	void data_find(Line<unsigned long>& DataAddress, DT key, unsigned long DataPointer = -1)
	{
		LayerData* CNode = FIndLayerData(key, DataPointer);
		Find(*CNode, key, DataAddress);
	}
	void data_rangefind(Line<unsigned long>& DataAddress, DT low_key, DT top_key)
	{
		LayerData* CNode = FIndLayerData(low_key, 0);
		RangeFind(*CNode, low_key, top_key, DataAddress);
	}
	void data_satistic(DT X[], DT low_key, DT top_key, int Y[])
	{
		LayerData* CNode = FIndLayerData(low_key, 0);
		Satistic(*CNode, X, low_key, top_key, Y);
	}
	void data_satistic_range(DT X[], DT low_key, DT top_key, DT gap, int Y[])
	{
		LayerData* CNode = FIndLayerData(low_key, 0);
		Satistic_range(*CNode, X, low_key, top_key, gap, Y);
	}
	void SaveIndex(string Name)
	{
		string PathN = "Index/" + Name + "LayerN.bin";
		string PathN_h = "Index/" + Name + "LayerN_hole.bin";
		string PathD = "Index/" + Name + "LayerData.bin";
		string PathD_h = "Index/" + Name + "LayerData_hole.bin";
		string Pathinfo = "Index/" + Name + "info.bin";
		createfile(PathN);
		createfile(PathD);

		fmanage<Index_LayerData<DT>> fLayerData;
		fmanage<Index_LayerN<DT>> fLayerN;
		fLayerN.OpenFile(PathN, PathN_h, true);
		fLayerData.OpenFile(PathD, PathD_h, true);

		ofstream info;
		unsigned long master_s = SaveLayerN(fLayerN, fLayerData, *Master, H);
		info.open(Pathinfo, ios::binary);
		info.write((char*)&master_s, sizeof(master_s));
		info.write((char*)&H, sizeof(H));
		info.close();
		fLayerN.CloseFile();
		fLayerData.CloseFile();
	}

	//迭代器//用于遍历
	class iterator : public std::iterator<std::forward_iterator_tag, DT>
	{
	public:
		LayerData* ptr;
		int index;
		struct node
		{
			DT indexer;
			unsigned long DataPointer;
		};
		iterator(LayerData* p = nullptr, int index = 0)
		{
			ptr = p;
			this->index = index;
		}
		node operator*()
		{
			return node{ ptr->key[index] , ptr->DataPointer[index]};
		}
		bool operator !=(const iterator& iter)
		{
			return ptr != iter.ptr;
		}
		iterator& operator++()
		{
			index++;
			if (ptr->num <= index)
			{
				ptr = ptr->next;
				index = 0;
			}
			return *this;
		}
		iterator& operator++(int)
		{
			iterator temp = *this;
			index++;
			if (ptr->num <= index)
			{
				ptr = ptr->next;
				index = 0;
			}
			return temp;
		}
	};
	iterator begin()
	{
		Pointer p = *Master;
		int h = H;

		while (h > 1)
		{
			p = p.Np->NodePointer[0];
			h--;
		}
		return iterator(p.Dp);
	}
	iterator end()
	{
		return iterator();
	}
};

#endif // !INDEX_H


