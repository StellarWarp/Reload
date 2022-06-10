/*
Index_s
用于外存索引的使用
*/

#pragma once
#include"Struct.h"
#include"Support.h"
#include"File.h"
#include"Define.h"//M
template<class DT>//关键字的类型
class Index_s
{
	//参数
	static const int min_key = (M % 2) ? M / 2 + 1 : M / 2;//m/2向上取整
	static const int max_key = M - 1;
	static const int min_i = min_key - 1;
	static const int max_i = max_key - 1;
	int H = 0;//树高

	typedef unsigned long Pointer;
	//结构
	struct LayerData//子叶节点
	{
		DT key[max_key] = {};
		Pointer DataPointer[max_key] = {};
		Pointer next = {};
		int num = 0;
	};
	struct LayerN
	{
		DT key[max_key] = {};
		Pointer DataPointer[max_key] = {};//用于辨识
		Pointer NodePointer[M] = {};
		int num = 0;
	};
	struct Pather
	{
		LayerN Node;
		Pointer NP;
		int i;
	};
	Stack<Pather> Path;
	Pointer Master;//根节点指针
	//文件
	fmanage<LayerData> fLayerData;
	fmanage<LayerN> fLayerN;
	LayerData& readDNode(Pointer DataPointer)
	{
		LayerData Node;
		fLayerData.ReadFile_i(DataPointer, Node);
		return Node;
	}
	LayerN& readNNode(Pointer DataPointer)
	{
		LayerN Node;
		fLayerN.ReadFile_i(DataPointer, Node);
		return Node;
	}

	//LayerData的操作函数
	//未完成?
	void Find(LayerData Node, DT value, Line<Pointer>& DataAddress)
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
				if (i == Node.num && Node.next != -1)
				{

					Find(readDNode(Node.next), value, DataAddress);
				}
			}
			else if (Node.next && i == Node.num)
			{
				Find(readDNode(Node.next), value, DataAddress);
			}
		}
	}
	void RangeFind(LayerData Node, DT low_value, DT top_value, Line<Pointer>& DataAddress)
	{
		int i = DilitarySearch_down(Node.key, 0, Node.num - 1, low_value);
		LayerData CNode = Node;
		DT Ckey = CNode.key[i];
		while (Ckey <= top_value)
		{
			if (i < CNode.num - 1)//未到边界
			{
				DataAddress.IN(CNode.DataPointer[i]);
				i++;
				Ckey = CNode.key[i];
			}
			else
			{
				if (CNode.next == -1) break;
				CNode = readDNode(CNode.next);
				i = 0;
				Ckey = CNode.key[i];
			}
		}
	}
	void Satistic(LayerData& Node, DT X[], DT low_key, DT top_key, int Y[])
	{
		int i = DilitarySearch_down(Node.key, 0, Node.num - 1, low_key);
		LayerData CNode = Node;
		int x = 0;
		int y = 0;
		DT Ckey = CNode.key[i];
		DT Cvalue = Ckey;
		while (Ckey <= top_key)
		{
			if (i < CNode.num - 1)//未到边界
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
				Ckey = CNode.key[i];
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
				if (CNode.next == -1) break;
				CNode = readDNode(CNode.next);
				i = 0;
				Ckey = CNode.key[i];
			}
		}
		Y[x] = y;
		X[x] = Cvalue;
	}
	void Satistic_range(LayerData& Node, DT X[], DT low_key, DT top_key, DT gap, int Y[])
	{
		int i = DilitarySearch_down(Node.key, 0, Node.num - 1, low_key);
		LayerData CNode = Node;
		int x = 0;
		int y = 0;
		DT Ckey = CNode.key[i];
		DT StepValue = low_key;
		while (Ckey <= top_key)
		{
			if (i < CNode.num - 1)//未到边界
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
				Ckey = CNode.key[i];
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
				if (CNode.next == -1) break;
				CNode = readDNode(CNode.next);
				i = 0;
				Ckey = CNode.key[i];
			}
		}
		Y[x] = y;
		X[x] = StepValue;
	}
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
	bool Insert(LayerData& Node, Pointer NP, Pointer DP, DT value)
	{
		int in = DilitarySearch_SecondOrder_down(Node.key, Node.DataPointer, 0, Node.num - 1, value, DP);
		if (Node.key[in] == value && Node.DataPointer[in] < DP)
		{
			in++;
		}
		InsertArray(Node.key, in, max_i, value);
		InsertArray(Node.DataPointer, in, max_i, DP);
		Node.num++;
		fLayerData.FileModify(Node, NP);
		if (in == Node.num - 1) return 1;
		else return 0;

	}
	Pointer Divide(LayerData& Node, Pointer NP, Pointer DP, DT value)
	{
		LayerData NewNode;
		int in = DilitarySearch_SecondOrder_down(Node.key, Node.DataPointer, 0, max_i, value, DP);
		if (Node.key[in] == value && Node.DataPointer[in] < DP)
		{
			in++;
		}
		Node.num = M / 2;
		NewNode.num = min_key;
		if (in <= min_i)
		{
			MoveArray(Node.key, min_i, max_i, NewNode.key);//将原节点多出的移动到新的节点
			MoveArray(Node.DataPointer, min_i, max_i, NewNode.DataPointer);
			SetArray(Node.key, min_i, max_i, DT());
			SetArray(Node.DataPointer, min_i, max_i, 0UL);
			InsertArray(Node.key, in, min_i, value);
			InsertArray(Node.DataPointer, in, min_i, DP);

		}
		else
		{
			MoveArray(Node.key, min_key, max_i, NewNode.key);
			MoveArray(Node.DataPointer, min_key, max_i, NewNode.DataPointer);
			SetArray(Node.key, min_key, max_i, DT());
			SetArray(Node.DataPointer, min_key, max_i, 0UL);
			InsertArray(NewNode.key, in - min_key, min_i, value);
			InsertArray(NewNode.DataPointer, in - min_key, min_i, DP);
		}
		NewNode.next = Node.next;
		Pointer p = fLayerData.FileAppends(NewNode);
		Node.next = p;
		fLayerData.FileModify(Node, NP);
		return p;
	}
	//返回删除的索引值
	int Delete(LayerData& Node, Pointer NP, DT value, Pointer DP)
	{
		Node.num--;
		int del = DilitarySearch_SecondOrder_down(Node.key, Node.DataPointer, 0, Node.num, value, DP);
		DeleteArray(Node.key, del, Node.num, (DT)0);
		DeleteArray(Node.DataPointer, del, Node.num, (Pointer)0);
		fLayerData.FileModify(Node, NP);
		return del;
	}
	//进行数量运算
	void Shift(LayerData& Node1, Pointer NP1, int a1, int b1, LayerData& Node2, Pointer NP2, int a2)
	{
		MoveArray(Node1.key, a1, b1, Node2.key, a2);
		MoveArray(Node1.DataPointer, a1, b1, Node2.DataPointer, a2);
		int num = b1 - a1 + 1;
		Node1.num -= num;
		Node2.num += num;
		fLayerData.FileModify(Node1, NP1);
		fLayerData.FileModify(Node2, NP2);
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

	Pointer newLayer(Pointer P1, DT value1, Pointer DP1, Pointer P2, DT value2, Pointer DP2)
	{
		LayerN NewNode;
		NewNode.key[0] = value1;
		NewNode.key[1] = value2;
		NewNode.NodePointer[0] = P1;
		NewNode.NodePointer[1] = P2;
		NewNode.DataPointer[0] = DP1;
		NewNode.DataPointer[1] = DP2;
		NewNode.num = 2;
		return fLayerN.FileAppends(NewNode);;
	}
	//返回是否最大值
	bool Insert(LayerN& Node, Pointer NP, Pointer P, DT value, Pointer DP)
	{//NP节点指针，P，value，DP插入的指针、值、辅助值（数据指针）
		int in = DilitarySearch_SecondOrder_down(Node.key, Node.DataPointer, 0, Node.num - 1, value, DP);
		if (Node.key[in] == value && Node.DataPointer[in] < DP)
		{
			in++;
		}
		InsertArray(Node.key, in, max_i, value);
		InsertArray(Node.NodePointer, in, max_i, P);
		InsertArray(Node.DataPointer, in, max_i, DP);
		Node.num++;
		fLayerN.FileModify(Node, NP);
		if (in == Node.num - 1) return 1;
		else return 0;
	}
	//更新并回溯
	bool update(Pather path, DT value, Pointer DP)
	{//节点参数与访问索引、更新参数
		//解释
		LayerN& Node = path.Node;
		Pointer NP = path.NP;
		int i = path.i;
		//操作
		if (i == Node.num - 1)
		{
			Node.key[i] = value;
			Node.DataPointer[i] = DP;
			fLayerN.FileModify(Node, NP);
			if (Path.Nempty())
			{
				update(Path.OUT(), value, DP);
			}
			return 1;
		}
		else
		{
			Node.DataPointer[i] = DP;
			Node.key[i] = value;
			fLayerN.FileModify(Node, NP);
			return 0;
		}
	}
	//不回溯
	void update0(Pather path, DT value, Pointer DP)
	{
		//解释
		LayerN& Node = path.Node;
		Pointer NP = path.NP;
		int i = path.i;
		//操作
		Node.DataPointer[i] = DP;
		Node.key[i] = value;
		fLayerN.FileModify(Node, NP);
	}
	//分裂并增加
	Pointer Divide(LayerN& Node, Pointer NP, Pointer P, DT value, Pointer DP)
	{
		LayerN NewNode;
		int in = DilitarySearch_SecondOrder_down(Node.key, Node.DataPointer, 0, max_i, value, DP);
		if (Node.key[in] == value && Node.DataPointer[in] < DP)
		{
			in++;
		}
		Node.num = M / 2;
		NewNode.num = min_key;
		if (in <= min_i)
		{
			MoveArray(Node.key, min_i, max_i, NewNode.key);//将原节点多出的移动到新的节点
			MoveArray(Node.NodePointer, min_i, max_i, NewNode.NodePointer);
			MoveArray(Node.DataPointer, min_i, max_i, NewNode.DataPointer);
			SetArray(Node.key, min_i, max_i, DT());
			SetArray(Node.NodePointer, min_i, max_i, Pointer());
			SetArray(Node.DataPointer, min_i, max_i, 0UL);
			InsertArray(Node.key, in, min_i, value);
			InsertArray(Node.NodePointer, in, min_i, P);
			InsertArray(Node.DataPointer, in, min_i, DP);
		}
		else
		{
			MoveArray(Node.key, min_key, max_i, NewNode.key);
			MoveArray(Node.NodePointer, min_key, max_i, NewNode.NodePointer);
			MoveArray(Node.DataPointer, min_key, max_i, NewNode.DataPointer);
			SetArray(Node.key, min_key, max_i, DT());
			SetArray(Node.NodePointer, min_key, max_i, Pointer());
			SetArray(Node.DataPointer, min_key, max_i, 0UL);
			InsertArray(NewNode.key, in - min_key, min_i, value);
			InsertArray(NewNode.NodePointer, in - min_key, min_i, P);
			InsertArray(NewNode.DataPointer, in - min_key, min_i, DP);
		}
		fLayerN.FileModify(Node, NP);
		return fLayerN.FileAppends(NewNode);
	}
	//删除并返回删除index
	int Delete(LayerN& Node, Pointer NP, DT value, Pointer DP)
	{
		Node.num--;
		int del = DilitarySearch_SecondOrder_down(Node.key, Node.DataPointer, 0, Node.num, value, DP);
		DeleteArray(Node.key, del, Node.num, (DT)0);
		Pointer Empty = 0;
		DeleteArray(Node.NodePointer, del, Node.num, Empty);
		DeleteArray(Node.DataPointer, del, Node.num, (Pointer)0);
		fLayerN.FileModify(Node, NP);
		return del;
	}
	//删除index
	void Delete(LayerN& Node, Pointer NP, int i)
	{
		Node.num--;
		int del = i;
		DeleteArray(Node.key, del, Node.num, (DT)0);
		Pointer Empty = 0;
		DeleteArray(Node.NodePointer, del, Node.num, Empty);
		DeleteArray(Node.DataPointer, del, Node.num, (unsigned long)0);
		fLayerN.FileModify(Node, NP);
	}
	//将1移到2
	void Shift(LayerN& Node1, Pointer NP1, int a1, int b1, LayerN& Node2, Pointer NP2, int a2)
	{
		MoveArray(Node1.key, a1, b1, Node2.key, a2);
		MoveArray(Node1.NodePointer, a1, b1, Node2.NodePointer, a2);
		MoveArray(Node1.DataPointer, a1, b1, Node2.DataPointer, a2);
		int num = b1 - a1 + 1;
		Node1.num -= num;
		Node2.num += num;
		fLayerN.FileModify(Node1, NP1);
		fLayerN.FileModify(Node2, NP2);
	}

	//函数
	Pointer FIndLayerData(DT key, Pointer DP = 0)
	{
		while (Path.Nempty())
		{
			Path.OUT();
		}
		LayerN CNode = readNNode(Master);
		int h = H;
		if (h > 1)
		{
			DT maxkey = CNode.key[CNode.num - 1];
			if (key > maxkey)
			{
				key = maxkey;
				Pointer maxPD = CNode.DataPointer[CNode.num - 1];
				if (DP > maxPD)
				{
					DP = maxPD;
				}
			}
		}
		Pather path;
		path.NP = Master;
		Pointer p;
		while (h > 1)
		{
			path.Node = CNode;
			int i = DilitarySearch_SecondOrder_down(CNode.key, CNode.DataPointer, 0, CNode.num - 1, key, DP);
			p = CNode.NodePointer[i];
			if (h != 2) CNode = readNNode(p);
			path.i = i;
			Path.IN(path);
			h--;
		}
		return p;
	};
	void LayerNAdd(Pointer P, DT key, Pointer DP)
	{
		Pather path = Path.OUT();
		LayerN& CLayer = path.Node;//当前节点
		Pointer CNP = path.NP;//当前节点的指针
		if (isfull(CLayer))
		{
			Pointer NLayer = Divide(CLayer, CNP, P, key, DP);
			if (Master == CNP)
			{

				Master = newLayer(CNP, CLayer.key[min_i], CLayer.DataPointer[min_i],
					NLayer, readNNode(NLayer).key[min_i], readNNode(NLayer).DataPointer[min_i]);
				H++;
			}
			else
			{
				update0(Path.Read0(), CLayer.key[min_i], CLayer.DataPointer[min_i]);
				LayerNAdd(NLayer, readNNode(NLayer).key[min_i], readNNode(NLayer).DataPointer[min_i]);
			}
		}
		else
		{
			if (Insert(CLayer, CNP, P, key, DP))//插入的值是最大值
			{
				//更新key的函数
				if (Path.Nempty())
				{
					update(Path.OUT(), key, DP);
				}
			}
		}
	}
	void LayerDataAdd(Pointer DataPointer, DT key)
	{
		if (H == 0)
		{
			LayerData NewLayer;
			Master = fLayerData.FileAppends(NewLayer);
			Insert(NewLayer, Master, DataPointer, key);
			H++;
		}
		else
		{
			Pointer CNP = FIndLayerData(key, DataPointer);
			LayerData CLayer = readDNode(CNP);
			if (isfull(CLayer))
			{
				Pointer NLayer = Divide(CLayer, CNP, DataPointer, key);
				if (H == 1)
				{
					Master = newLayer(CNP, CLayer.key[min_i], CLayer.DataPointer[min_i],
						NLayer, readDNode(NLayer).key[min_i], readDNode(NLayer).DataPointer[min_i]);
					H++;
				}
				else
				{
					update0(Path.Read0(), CLayer.key[min_i], CLayer.DataPointer[min_i]);
					LayerNAdd(NLayer, readDNode(NLayer).key[min_i], readDNode(NLayer).DataPointer[min_i]);
				}
			}
			else
			{
				if (Insert(CLayer, CNP, DataPointer, key))
				{
					//更新key的函数
					if (Path.Nempty())
					{
						update(Path.OUT(), key, DataPointer);
					}
				}
			}

		}
	}
	//删除功能
	void LayerNDelete(int del)
	{
		Pather path = Path.OUT();
		LayerN& CLayer = path.Node;//当前节点
		Pointer CNP = path.NP;//当前节点的指针
		//当前节点最大参数//即在父节点存放的索引
		DT keymax = CLayer.key[min_i];
		Pointer DPmax = CLayer.DataPointer[min_i];
		//直接删除
		Delete(CLayer, CNP, del);
		if (Path.Nempty())
		{
			if (CLayer.num < min_key)
			{
				path = Path.Read0();
				//父节点
				Pointer FNP = path.NP;
				LayerN& FNode = path.Node;
				int Ci = path.i;
				//兄弟节点
				Pointer BNP;
				LayerN BNode;
				int Bi;
				if (Ci == 0)
				{//右
					Bi = Ci + 1;
					BNP = FNode.NodePointer[Bi];
					BNode = readNNode(BNP);
					if (ismore(BNode))//借用
					{
						int i = 0;
						Shift(BNode, BNP, i, i, CLayer, CNP, min_i);
						Shift(BNode, BNP, 1, BNode.num, BNode, BNP, 0);
						update0(Path.OUT(), CLayer.key[min_i], CLayer.DataPointer[min_i]);
					}
					else//合并//删除B
					{
						Shift(BNode, BNP, 0, min_i, CLayer, CNP, min_i);
						update0(Path.OUT(), CLayer.key[min_i], CLayer.DataPointer[min_i]);
						LayerNDelete(Bi);
						fLayerN.FileDelete(CNP);
					}
				}
				else
				{//左
					Bi = Ci - 1;
					BNP = FNode.NodePointer[Bi];
					BNode = readNNode(BNP);
					if (ismore(BNode))//借用
					{
						int i = --BNode.num;//数量减一
						fLayerN.FileModify(BNode, BNP);
						Insert(CLayer, CNP, BNode.NodePointer[i], BNode.key[i], BNode.DataPointer[i]);
						path.Node = FNode;
						path.NP = FNP;
						path.i = Bi;
						update0(path, BNode.key[i - 1], BNode.DataPointer[i - 1]);
					}
					else//合并
					{
						Shift(CLayer, CNP, 0, min_i - 1, BNode, BNP, min_i + 1);
						path.Node = FNode;
						path.NP = FNP;
						path.i = Bi;
						update0(path, BNode.key[max_i], BNode.DataPointer[max_i]);
						LayerNDelete(Ci);
						fLayerN.FileDelete(CNP);
					}
				}
			}
			else if (del == CLayer.num)
			{
				update(Path.OUT(), CLayer.key[CLayer.num - 1], CLayer.DataPointer[CLayer.num - 1]);
			}
		}
		else if (CLayer.num == 1)
		{
			fLayerN.FileDelete(Master);
			Master = CLayer.NodePointer[0];
			H--;
		}

	}
	void LayerDataDelete(Pointer DataPointer, DT key)
	{
		Pointer CNP = FIndLayerData(key, DataPointer);
		LayerData CLayer = readDNode(CNP);
		DT keymax = CLayer.key[min_i];
		DT DPmax = CLayer.DataPointer[min_i];
		int del = Delete(CLayer, CNP, key, DataPointer);
		if (Path.Nempty())
		{
			if (CLayer.num < min_key)
			{
				Pather path = Path.Read0();
				//父节点
				Pointer FNP = path.NP;
				LayerN& FNode = path.Node;
				int Ci = path.i;
				//兄弟节点
				Pointer BNP;
				LayerData BNode;
				int Bi;
				if (Ci == 0)
				{//右
					Bi = Ci + 1;
					BNP = FNode.NodePointer[Bi];
					BNode = readDNode(BNP);
					if (ismore(BNode))//借用//最小值
					{
						int i = 0;
						Shift(BNode, BNP, i, i, CLayer, CNP, min_i);
						Shift(BNode, BNP, 1, BNode.num, BNode, BNP, 0);
						update0(Path.OUT(), CLayer.key[min_i], CLayer.DataPointer[min_i]);
					}
					else//合并
					{
						Shift(BNode, BNP, 0, min_i, CLayer, CNP, min_i);
						update0(Path.OUT(), CLayer.key[min_i], CLayer.DataPointer[min_i]);
						LayerNDelete(Bi);
						CLayer.next = BNode.next;
						fLayerData.FileModify(CLayer, CNP);
						fLayerData.FileDelete(BNP);
					}
				}
				else
				{//左
					Bi = Ci - 1;
					BNP = FNode.NodePointer[Bi];
					BNode = readDNode(BNP);
					if (ismore(BNode))//借用
					{
						int i = --BNode.num;
						fLayerData.FileModify(BNode, BNP);
						Insert(CLayer, CNP, BNode.DataPointer[i], BNode.key[i]);
						path.Node = FNode;
						path.NP = FNP;
						path.i = Bi;
						update0(path, BNode.key[i - 1], BNode.DataPointer[i - 1]);
					}
					else//合并
					{
						Shift(CLayer, CNP, 0, min_i - 1, BNode, BNP, min_i + 1);
						path.Node = FNode;
						path.NP = FNP;
						path.i = Bi;
						update0(path, BNode.key[max_i], BNode.DataPointer[max_i]);
						LayerNDelete(Ci);
						BNode.next = CLayer.next;
						fLayerData.FileModify(CLayer, CNP);
						fLayerData.FileDelete(CNP);
					}
				}
			}
			else if (del == CLayer.num)
			{
				update(Path.OUT(), CLayer.key[CLayer.num - 1], CLayer.DataPointer[CLayer.num - 1]);
			}
		}
	}

public:
	void Import(string Name)
	{
		string PathN = "Index/" + Name + "LayerN.bin";
		string PathN_h = "Index/" + Name + "LayerN_hole.bin";
		string PathD = "Index/" + Name + "LayerData.bin";
		string PathD_h = "Index/" + Name + "LayerData_hole.bin";
		string Pathinfo = "Index/" + Name + "info.bin";
		fLayerN.OpenFile(PathN, PathN_h);
		fLayerData.OpenFile(PathD, PathD_h);
		ifstream info;
		info.open(Pathinfo, ios::binary);
		info.read((char*)&Master, sizeof(Master));
		info.read((char*)&H, sizeof(H));
		info.close();
	}
	~Index_s()
	{
		fLayerN.CloseFile();
		fLayerData.CloseFile();
	}
	void append(Pointer DataPointer, DT key)
	{
		LayerDataAdd(DataPointer, key);
	}
	void data_delete(Pointer DataPointer, DT key)
	{
		LayerDataDelete(DataPointer, key);
	}
	void data_find(Line<Pointer>& DataAddress, DT key, Pointer DataPointer = 0)
	{
		Pointer CNP = FIndLayerData(key, DataPointer);
		LayerData CNode = readDNode(CNP);
		Find(CNode, key, DataAddress);
	}
	void data_rangefind(Line<Pointer>& DataAddress, DT low_key, DT top_key)
	{
		Pointer CNP = FIndLayerData(low_key, 0);
		LayerData CNode = readDNode(CNP);
		RangeFind(CNode, low_key, top_key, DataAddress);
	}
	void data_satistic(DT X[], DT low_key, DT top_key, int Y[])
	{
		Pointer CNP = FIndLayerData(low_key, 0);
		LayerData CNode = readDNode(CNP);
		Satistic(CNode, X, low_key, top_key, Y);
	}
	void data_satistic_range(DT X[], DT low_key, DT top_key, DT gap, int Y[])
	{
		Pointer CNP = FIndLayerData(low_key, 0);
		LayerData CNode = readDNode(CNP);
		Satistic_range(CNode, X, low_key, top_key, gap, Y);
	}
};
