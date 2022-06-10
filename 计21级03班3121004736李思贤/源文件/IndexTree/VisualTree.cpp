#include"VisualTree.h"
#include"控制台.h"
#include<vector>
#include<iostream>
#include"Define.h"
using namespace std;

const int lenth = M-1;
const int space = 5;
vector<int> tree;

void tree_append(int layer, const string& str)
{
	layer--;
	if (layer == tree.size())
	{
		tree.push_back(0);
	}
	int offset = pow(lenth, layer)-1 +pow(lenth,layer)* tree[layer];
	int pos[2] = { layer * space, offset };
	printc(pos, str.c_str());
	tree[layer]++;
}
void init()
{
	InitializeUI();
}
void Clear()
{
	system("cls");
	tree.clear();
}

////可视化
//void PrintLayerData(Pointer P)
//{
//	if (P.Dp)
//	{
//		for (int i = 0; i < P.Dp->num; i++)
//		{
//			tree_append(1, to_string(P.Dp->key[i]));
//		}
//		for (int i = 0; i < max_key - P.Dp->num; i++)
//		{
//			tree_append(1, " ");
//		}
//	}
//	else
//	{
//		for (int i = 0; i < max_key; i++)
//		{
//			tree_append(1, " ");
//		}
//	}
//}
//void PrintLayerN(Pointer P, int Layer)
//{
//	if (Layer > 2)
//	{
//		for (int i = 0; i < max_key; i++)
//		{
//			if (P.Np)
//			{
//				PrintLayerN(P.Np->NodePointer[i], Layer - 1);
//				if (i < P.Np->num)
//					tree_append(Layer, to_string(P.Np->key[i]));
//				else
//					tree_append(Layer, " ");
//			}
//			else
//			{
//				Pointer Empty{};
//				PrintLayerN(Empty, Layer - 1);
//				tree_append(Layer, " ");
//			}
//		}
//	}
//	else//layer == 2
//	{
//		for (int i = 0; i < max_key; i++)
//		{
//			if (P.Np)
//			{
//				PrintLayerData(P.Np->NodePointer[i]);
//				if (i < P.Np->num)
//					tree_append(Layer, to_string(P.Np->key[i]));
//				else
//					tree_append(Layer, " ");
//			}
//			else
//			{
//				Pointer Empty{};
//				PrintLayerData(Empty);
//				tree_append(Layer, " ");
//			}
//
//		}
//	}
//}
//void PrintTree()
//{
//	Clear();
//	PrintLayerN(*Master, H);
//}
