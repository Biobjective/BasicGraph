#include"path.h"
#include"Graph.h"
#include<algorithm>
#define INFI 65536;

//�����������ڽӾ����ҵ����·������С������
//numָ�������Ŀ��vָ�����Ķ��㣬g��ʾ�ڽӾ���p����Ϊ��ĳ�����ǰ����dΪv�������������СȨֵ
//ʱ�临�Ӷ�ΪO(n^2)
void dijkstra(int num, int v, vector<int>& p, vector<int>& d, const vector<vector<int>>& g)
{
	int min, k;
	vector<bool> final(num, false);
	for (int i = 0; i < num; i++)
		d[i] = g[v][i];
	d[v] = 0;   //��ʾv��v·������Ϊ0
	//��������������>**********  ��ʱ�����ܽ�v������С�������� ����=true  **********<������������\\
	//final[v] = true;   //��ʾv��v·������Ҫ��  
	
	for (int i = 0; i < num; ++i)
	{
		min = INFI;
		//�ҵ�����Ҫ������С�������Ľڵ�
		for (int j = 0; j < num; ++j)
		{
			if (!final[j] && d[j] < min)
			{
				k = j;
				min = d[j];
			}

		}
		final[k] = true;   //�ҵ�v����Ķ���k
		for (int w = 0; w < num; w++)//v����k��������v��k�ľ����k��w�ľ���С��v��w�ľ��룬�����dʹ��Ϊ��̵ľ���
		{
			if (!final[w] && (min + g[k][w]) <= d[w])
			{
				d[w] = min + g[k][w];
				p[w] = k;     //p����Ϊ��ĳ�����ǰ��
			}
		}
	}
}

//vΪԴ�ڵ㣬pΪ��ĳ�����ǰ��
vector<vector<int> > shortpath_construct(int v, int num, vector<int>& p) {
	vector<vector<int> > res;
	for (int i = 0; i < num; ++i) {
		vector<int> tmp;
		int cur = i;
		while (cur != -1) {//-1��ʾ������v
			tmp.push_back(cur);
			cur = p[cur];
		}
		reverse(tmp.begin(),tmp.end()); //����Ҫ��ת ����ʱ��Ϊ����
		res.push_back(tmp);
		tmp.clear();
	}
	return res;
}