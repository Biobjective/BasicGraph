#include"Graph.h"
#include"path.h"
#include"Block_Graph.h"
#include"acs.h"

UndiGraph<int, float> Graph(71);
BlockGraph<myType, float> g(310);
long long timestamp = 0;
const int m = 100;//ants number
const float beta = 2.0;//heuristic information weight 
int G = 50;//��������
const float rho = 0.3;//pheromone violation factor 
const float xik = 0.1;//
const float pher_start = 0.025;
const float v_start = 100;
const int w = 3;//number of ants allowed to release pheromone 
const int cap = 20;//capacity of each road
int traffic_time = 1; // ���Լ����ٶȵ�ʱ��
const float heur_strat = 0;

const int target = 48;//�趨��ʼ�̶�Դ�ڵ�  48//521
vector<myType> source(m, target);//ÿֻ���ϵ�Դ�ڵ�  521
vector<myType> safe_site;//��ȫ�Ľڵ�  �� Ŀ�Ľڵ㼯��
vector<myType> sites;//ȫ���ڵ�ļ���
vector<bool> ant_dest(m, false);//�Ƿ񵽴��յ��ʶ
vector<vector<myType> > ant_solution(m);
vector<unordered_map<myType, float> > flow_mtx;//flow hash function
vector<pair<myType, myType> > curPosition;//ÿֻ����Ŀǰ�ڵ�ǰ·�ε�λ��
vector<pair<myType, myType> > curOD;//���ڴ洢��ǰ�ε�OD����һ��������·������Ϊ�ܶ�Σ�ÿһ��ά��һ��od�ԣ�������ʱ����ǿ�����od�ԣ�
vector<myType> block_dest;//�������ϵ�Ŀ��block��Ŀ���ǲ���Ҫ���getBlockid
vector<double> velMatrix;//���ڴ洢��ǰ�������ٶ�

vector<vector<pair<myType, float> > > heur_mtx;//heuristic information 
vector<vector<pair<myType, float> > > trans_mtx;//Transition matrix
vector<vector<pair<myType, float> > > pher_mtx;//Pheromone matrix

set<pair<int, int> > cur_global_route;
vector<vector<pair<myType, pair<int, int> > > > density_mtx;//density matrix  ��flow ͬʱʵ�� �����Ϊ�ڵ���
vector<float> fit_mtx(m, 0);//��Ӧ�Ⱦ���
pair<float, vector<myType>> history_best;
vector<vector<myType> > transmit_p;// ���;���
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
		ans = Graph.pathTo(target, s);/// ��Ҫ�޸�
		for (auto t:ans) {
			cout << "->" << t;
		}
		cout << endl;
	}

	for (int cnts = 0; G; cnts++) {
		//freopen_s(&stream2, "CON", "w", stdout);//����ض���� ����̨
		routeConstricution();
		updateGlobalPher();
		updateTrans();
		float sum = meanCompute();
		//cout << '\a' << endl;
		//freopen_s(&stream2, "res2.txt", "w", stdout);//����ض���
		cout << sum << endl;
		//cout << cnts<<":"<<meanCompute() << endl;
	}

	//vector<int> v = G.Adj(2);
	return 0;
}