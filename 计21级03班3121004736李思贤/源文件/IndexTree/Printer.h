#pragma once
#include<iostream>
#include"TypeSupport.h"
using namespace std;
template<class DT>
void PrintXY(DT X[], int Y[], int num)
{
	for (int i = 0; i < num; i++)
	{
		cout.width(7);
		cout << X[i] << " ";
		cout.width(7);
		cout << Y[i];
		cout << " | ";
		for (int j = 0; j < Y[i]/10; j++)
		{
			cout << "=";
		}
		cout << endl;
	}
}
