#include"Graph.h"
#include"path.h"
#include"Block_Graph.h"

UndiGraph<int, float> G(71);
//Node<int,int>* pp() {
//	auto_ptr<Node<int, int>> tmpp(new Node<int, int>(0, 0);
//	return tmpp;
//}

int main() {
	//vector<int> tmpppp{ 1,1,1 };
	BlockGraph<int, int> TMP(10, 2, 2);
	TMP.InputfileGraph("G.txt");
	auto borderres = TMP.getBorderBlock(1, 4);
	auto idres = TMP.getBlockid(4);
	if (TMP.sepBlock()) cout << "seperate sucess" << endl;
	TMP.Display();
	TMP.subDisplay();
	vector<int> vv;
	G.InputfileGraph("Dj.txt");//C:/Users/shijian/Desktop/     
	int target = 48;
	auto k = G.getedgematrix();
	int num = G.getnumVertexes();
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
	v = G.getAllvertex();
	for (auto s:v) {
		vector<int> ans;
		ans = G.pathTo(target, s);/// ÐèÒªÐÞ¸Ä
		for (auto t:ans) {
			cout << "->" << t;
		}
		cout << endl;
	}
	//vector<int> v = G.Adj(2);
	return 0;
}