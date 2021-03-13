#include"Graph.h"
#include"path.h"
#include"Block_Graph.h"
#include"acs.h"

UndiGraph<int, float> Graph(71);
BlockGraph<myType, float> g(310);
long long timestamp = 0;
const int m = 100;//ants number
const float beta = 2.0;//heuristic information weight 
int G = 50;//迭代次数
const float rho = 0.3;//pheromone violation factor 
const float xik = 0.1;//
const float pher_start = 0.025;
const float v_start = 100;
const int w = 3;//number of ants allowed to release pheromone 
const int cap = 20;//capacity of each road
int traffic_time = 1; // 用以计算速度的时间
const float heur_strat = 0;

const int target = 48;//设定初始固定源节点  48//521
vector<myType> source(m, target);//每只蚂蚁的源节点  521
vector<myType> safe_site;//安全的节点  或 目的节点集合
vector<myType> sites;//全部节点的集合
vector<bool> ant_dest(m, false);//是否到达终点标识
vector<vector<myType> > ant_solution(m);
vector<unordered_map<myType, float> > flow_mtx;//flow hash function
vector<pair<myType, myType> > curPosition;//每只蚂蚁目前在当前路段的位置
vector<pair<myType, myType> > curOD;//用于存储当前段的OD，将一个完整的路径划分为很多段，每一段维护一个od对，跨区的时候就是跨区的od对；
vector<myType> block_dest;//所有蚂蚁的目的block，目的是不需要多次getBlockid
vector<double> velMatrix;//用于存储当前车辆的速度

vector<vector<pair<myType, float> > > heur_mtx;//heuristic information 
vector<vector<pair<myType, float> > > trans_mtx;//Transition matrix
vector<vector<pair<myType, float> > > pher_mtx;//Pheromone matrix

set<pair<int, int> > cur_global_route;
vector<vector<pair<myType, pair<int, int> > > > density_mtx;//density matrix  与flow 同时实现 最外层为节点编号
vector<float> fit_mtx(m, 0);//适应度矩阵
pair<float, vector<myType>> history_best;
vector<vector<myType> > transmit_p;// 传送矩阵
vector<vector<myType> > short_p;//short path


int main() {
	BlockGraph<int, int> TMP(10, 1, 3);
	TMP.InputfileGraph("G.txt");
	auto borderres = TMP.getBorderBlock(1, 4);
	auto idres = TMP.getBlockid(4);
	
	TMP.Display();
	TMP.subDisplay();
	vector<int> vv;
	Graph.InputfileGraph("Dj.txt");//C:/Users/shijian/Desktop/     
	int target = 48;
	auto k = Graph.getedgematrix();
	int num = Graph.getnumVertexes();
	vector<int> d(num);
	vector<int> p(num, -1);
	dijkstra(num, 32, p, d, k);
	//for (int i = 0; i < num; ++i) {
	//	vector<int> p(num, -1);
	//	dijkstra(num, i, p, d, k);
	//	auto unreach = find(p.begin(), p.end(), -1);
	//	if (unreach == p.end()) cout << i << ":success\n";
	//}
	auto res = shortpath_construct(32, num, p);
	
	//G.Display();
	//G.BFSTraverse(target);
	vector<int> v;
	v = Graph.getAllvertex();
	for (auto s:v) {
		vector<int> ans;
		ans = Graph.pathTo(target, s);/// 需要修改
		for (auto t:ans) {
			cout << "->" << t;
		}
		cout << endl;
	}

	for (int cnts = 0; G; cnts++) {
		//freopen_s(&stream2, "CON", "w", stdout);//输出重定向回 控制台
		routeConstricution();
		updateGlobalPher();
		updateTrans();
		float sum = meanCompute();
		//cout << '\a' << endl;
		//freopen_s(&stream2, "res2.txt", "w", stdout);//输出重定向
		cout << sum << endl;
		//cout << cnts<<":"<<meanCompute() << endl;
	}

	//vector<int> v = G.Adj(2);
	return 0;
}