#include"path.h"
#include"Graph.h"
#include<algorithm>
#define INFI 65536;

//这里是利用邻接矩阵找的最短路径和最小生成树
//num指顶点的数目，v指出发的顶点，g表示邻接矩阵，p矩阵为到某顶点的前驱，d为v到各个顶点的最小权值
//时间复杂度为O(n^2)
void dijkstra(int num, int v, vector<int>& p, vector<int>& d, const vector<vector<int>>& g)
{
	int min, k;
	vector<bool> final(num, false);
	for (int i = 0; i < num; i++)
		d[i] = g[v][i];
	d[v] = 0;   //表示v到v路径长度为0
	//———————>**********  此时还不能将v加入最小生成树中 不能=true  **********<——————\\
	//final[v] = true;   //表示v到v路径不需要求  
	
	for (int i = 0; i < num; ++i)
	{
		min = INFI;
		//找到本次要加入最小生成树的节点
		for (int j = 0; j < num; ++j)
		{
			if (!final[j] && d[j] < min)
			{
				k = j;
				min = d[j];
			}

		}
		final[k] = true;   //找到v最近的顶点k
		for (int w = 0; w < num; w++)//v距离k最近，如果v到k的距离加k到w的距离小于v到w的距离，则调整d使得为最短的距离
		{
			if (!final[w] && (min + g[k][w]) <= d[w])
			{
				d[w] = min + g[k][w];
				p[w] = k;     //p矩阵为到某顶点的前驱
			}
		}
	}
}

//v为源节点，p为到某顶点的前驱
vector<vector<int> > shortpath_construct(int v, int num, vector<int>& p) {
	vector<vector<int> > res;
	for (int i = 0; i < num; ++i) {
		vector<int> tmp;
		int cur = i;
		while (cur != -1) {//-1表示到达了v
			tmp.push_back(cur);
			cur = p[cur];
		}
		reverse(tmp.begin(),tmp.end()); //不需要翻转 插入时即为倒叙
		res.push_back(tmp);
		tmp.clear();
	}
	return res;
}