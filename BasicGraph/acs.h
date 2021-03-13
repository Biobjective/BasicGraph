#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include <time.h>
#include<unordered_map>
#include <windows.h>
#include<algorithm>
#include<numeric>
#include<sstream>
#include <queue>
#include<unordered_set>
#include<set>
#include"path.h"
#include "Graph.h"
#include"Block_Graph.h"

#define myType int
#define eps   1e-6//float使用的测试精度

/********using extern to declaration ***********/
extern long long timestamp;//时间戳********

extern const int m;//ants number
extern const float beta;//heuristic information weight 
extern int G;//迭代次数
extern const float rho;//pheromone violation factor 
extern const float xik;//
extern const float pher_start;
extern const float v_start;
extern const int w;//number of ants allowed to release pheromone 
extern const int cap;//capacity of each road
extern int traffic_time; // 用以计算速度的时间
extern const float heur_strat;

extern const int target;//设定初始固定源节点  48//521

extern vector<pair<myType, myType> > curPosition;//每只蚂蚁目前在当前路段的位置
extern vector<double> velMatrix;//用于存储当前车辆的速度
extern vector<pair<myType, myType> > curOD;//用于存储当前段的OD，将一个完整的路径划分为很多段，每一段维护一个od对，跨区的时候就是跨区的od对；
extern vector<myType> block_dest;//所有蚂蚁的目的block，目的是不需要多次getBlockid


extern vector<myType> source;//每只蚂蚁的源节点  521
extern vector<myType> safe_site;//安全的节点  或 目的节点集合
extern vector<myType> sites;//全部节点的集合
extern vector<bool> ant_dest;//是否到达终点标识
extern vector<vector<myType> > ant_solution;
extern vector<unordered_map<myType, float> > flow_mtx;//flow hash function

extern vector<vector<pair<myType, float> > > heur_mtx;//heuristic information 
extern vector<vector<pair<myType, float> > > trans_mtx;//Transition matrix
extern vector<vector<pair<myType, float> > > pher_mtx;//Pheromone matrix

extern set<pair<int, int> > cur_global_route;
extern vector<vector<pair<myType, pair<int, int> > > > density_mtx;//density matrix  与flow 同时实现 最外层为节点编号
extern vector<float> fit_mtx;//适应度矩阵
extern pair<float, vector<myType>> history_best;
extern vector<vector<myType> > transmit_p;// 传送矩阵
extern vector<vector<myType> > short_p;//short path

extern BlockGraph<myType, float> g;
/********end of Data declaration ***********/

void routeConstricution();
void new_init();
vector<float> fitCompute();
float velocityCompute(myType a, myType b);
void updateLocalPher(myType a, myType b);
void updateGlobalPher();
//float meanCompute();
void readFile(string direction);
//float rand_n(int N);
//int get_pos(myType t);
void updateTrans(bool tag = true);
void updateNodeTrans(myType a);
bool graphPreprocess(int v, int num, vector<vector<myType> >& p, vector<vector<myType> >& res);
pair<myType, myType> selectNextRoute(int before_pos, int current_pos);
pair<myType, myType> selctTrunkLine(int b1, int b2, int vertex);
myType getNextWaySerial(myType orgn, myType dstn, int antId, myType cur, myType pre);
void UpdateCurOd(int antid, myType cur_vertex, myType des_vertex);

inline float meanCompute() {
	float sum = accumulate(fit_mtx.begin(), fit_mtx.end(), 0.0);
	sum /= (float)m;
	return sum;
}
//当前节点是否safe
inline bool isSafe(int p) {
	vector<int>::iterator it = find(safe_site.begin(), safe_site.end(), p);
	if (it != safe_site.end()) return true;
	else return false;
}
//num表示蚂蚁id  p表示当前站点
inline bool isEnd(int p, int num) {
	if (p == safe_site[num]) return true;
	else return false;
}

//返回0-1之间的随机数
inline float rand_p() {
	int N = 999;//设置随机数精度
	LARGE_INTEGER seed;
	QueryPerformanceFrequency(&seed);
	QueryPerformanceCounter(&seed);
	srand(seed.QuadPart);
	//return rand();
	return rand() % (N + 1) / (float)(N + 1);
}

inline int get_pos(myType t) {
	int dis = -1;
	vector<myType>::iterator it;
	it = find(sites.begin(), sites.end(), t);
	dis = distance(sites.begin(), it);
	if (dis == sites.size())
		cerr << "get_pos wrong" << endl;
	return dis;
}



