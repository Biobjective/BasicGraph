#pragma once
#ifndef GRAPH_H_
#define GRAPH_H_
const int INF  = 65536;
#include"list_graph.h"
#include<iostream>
#include<vector>
#include<map>
#include<string>
#include<fstream>
#include<queue>
#include<unordered_map>
using namespace std;

//#ifdef _cplusplus
//extern "C" {
//#endif
//	//矩形类
////class rec {
////protected:
////	int w, h;
////};
////class Rectangle :public rec {
////private:
////
////	static int nTotalNumber;
////	static int nTotalArea;
////public:
////	Rectangle(int w_, int h_);
////	~Rectangle();
////	static void PrintTotal();
////	int computw();
////};
////Rectangle::Rectangle(int w_, int h_) {
////	w = w_;
////	h = h_;
////	nTotalNumber++;
////	nTotalArea += w * h;
////}
////Rectangle::~Rectangle() {
////	nTotalNumber--;
////	nTotalArea -= w * h;
////}
////void Rectangle::PrintTotal() {
////	cout << nTotalNumber << "," << nTotalArea << endl;
////}
////int Rectangle::nTotalNumber = 0;
////int Rectangle::nTotalArea = 0;
////int Rectangle::computw() {
////	return w;
////}
//
//
//#ifdef _cplusplus
//}
//#endif

/*图类-----基类*/
template<typename T, typename E>
class Graph
{
protected:
	int  maxVertexes;      //分配内存时的最大的顶点数
	int  Vertex_num;		//顶点数目
	int  Edge_num;			//边数目
	bool Weight = true;			//是否有权
	bool Directed = false;			//是否有向
	bool Vertex_value = true;		//节点是否有值
public:
	//static const E maxWeight = INF;
	explicit Graph(int size = 20);
	Graph(const Graph&);//copy
	virtual ~Graph() {}
	Graph& operator=(const Graph& rhs);
	int getnumVertexes()
	{
		return Vertex_num;
	}
	int getnumEdges()
	{
		return Edge_num;
	}
};

template<typename T, typename E>
Graph<T, E>::Graph(int size) :maxVertexes(size) {}

template<typename T,typename E>
Graph<T, E>::Graph(const Graph<T,E>& g) {
	maxVertexes = g.maxVertexes;
	Vertex_num = g.Vertex_num;
	Edge_num = g.Edge_num;
	Weight = g.Weight;
	Directed = g.Directed;
	Vertex_value = g.Vertex_value;
}

template<typename T, typename E>
Graph<T,E>& Graph<T, E>::operator=(const Graph& rhs) {
	cout << "Graph operator= called" << endl;
	if (this == &rhs) return *this;
	maxVertexes = rhs.maxVertexes;
	Vertex_num = rhs.Vertex_num;
	Edge_num = rhs.Edge_num;
	Weight = rhs.Weight;
	Directed = rhs.Directed;
	Vertex_value = rhs.Vertex_value;
	return *this;
}

/*节点类*/
template<typename T, typename E>//T节点 E边权重
class Node
{
public:
	int serial;         //顶点编号
	T value;			//节点可能值，如不存在则默认与编号相同
	bool marked;       //顶点访问标志位：true表示已经被访问
	int block = 0;
	listpoint<T, E>* next;//与该顶点相邻的顶点列表

	Node();
	Node(int identifier, T number = 0, int _block = 0);
	Node(const Node&);
	Node& operator=(const Node&);
	//~Node();
};
template<typename T, typename E>
Node<T, E>::Node() {
	serial = -1;
	marked = false;
	next = nullptr;
}
template<typename T, typename E>
Node<T, E>::Node(int identifier, T number,int _block)
{
	serial = identifier;
	value = number;
	block = _block;
	marked = false;
	next = nullptr;
}
template<typename T,typename E>
Node<T, E>::Node(const Node& rhs) {
	serial = rhs.serial;
	value = rhs.value;
	marked = rhs.marked;
	block = rhs.block;
	listpoint<T, E>* tmprhs = rhs.next;
	if (tmprhs != nullptr) {
		listpoint<T, E>* head = new listpoint<T, E>();
		next = head;
		head->data = tmprhs->data;
		head->value = tmprhs->value;
		head->next = nullptr;
		while (tmprhs->next != nullptr) {
			tmprhs = tmprhs->next;
			head->next = new listpoint<T, E>();
			head = head->next;
			head->data = tmprhs->data;
			head->value = tmprhs->value;
			head->next = nullptr;
		}
	}

}
template<typename T, typename E>
Node<T, E>& Node<T, E>::operator=(const Node& nd) {
	//cout << "Node operator= called" << endl;
	if (this == &nd) return *this;
	serial = nd.serial;
	value = nd.value;
	marked = nd.marked;
	block = nd.block;
	listpoint<T, E>* tmp = next;
	while (tmp != nullptr) {
		listpoint<T, E>* cur = tmp;
		tmp = tmp->next;
		delete cur;
	}
	listpoint<T, E>* tmprhs = nd.next;
	if (tmprhs != nullptr) {
		listpoint<T, E>* head = new listpoint<T, E>();
		next = head;
		head->data = tmprhs->data;
		head->value = tmprhs->value;
		head->next = nullptr;
		while (tmprhs->next != nullptr) {
			tmprhs = tmprhs->next;
			head->next = new listpoint<T, E>();
			head = head->next;
			head->data = tmprhs->data;
			head->value = tmprhs->value;
			head->next = nullptr;
		}
	}
	return *this;
}

//边类
template<typename T, typename E>
class Edge
{
	int weight;
	Node<T, E>* from;
	Node<T, E>* to;
public:
	Edge();
	Edge(int _weight, Node<T, E>* _from, Node<T, E>* _to)
	{
		weight = _weight;
		from = _from;
		to = _to;
	}
};

//无向图类  假设T可为int string 或 float等   E为值
template<typename T, typename E>
class UndiGraph :public Graph<T, E>
{
protected:
	map<int, Node<T, E>*> vertex_table;		//顶点序号和顶点对象映射  邻接表  只有每个链表头结点   注意是int序号的映射 不是T
	unordered_map<T, vector<int> >  edgeAll;
	unordered_map<T, vector<bool> >  markedAll;//用于计算 hasPathTo
public:
	map<int, vector<pair<T,T> > > border_map;//用于存储 该图通往其他图的 边界   第一个int为block id ,vector存储该图通向下一个图的所有边
public:
	explicit UndiGraph(int size = 20);
	virtual ~UndiGraph();
	UndiGraph(const UndiGraph&);//拷贝构造函数，深拷贝，
	UndiGraph& operator=(const UndiGraph& rhs);
	virtual void Display();
	virtual void InputfileGraph(string filename);

	void CreateGraph();
	bool InsertVertex(T vertex);
	bool InsertVertex(T vertex,int _block);
	int getVertexPos(T vertex);//获取节点编号
	bool InsertEdge(const T vertex1, const T vertex2, E weight);
	bool ExistEdge(T v1, T v2);
	E getweight(T v1, T v2);
	vector<T> Adj(T vertex);//获取相邻节点
	void visit(T i);
	vector<T> getAllvertex();
	void BFSTraverse(T s);
	bool hasPathTo(T s, T d);//两个节点间是否连通
	vector<int> pathTo(T s,T d);
	vector<vector<T>> getedgematrix();//将邻接矩阵存在vector<vector<E>>中
	
	int subGetBlock(T vertex);
};

template<typename T, typename E>
UndiGraph<T, E>::UndiGraph(int size) {
	//Graph<T, E>::Graph(size);
	this->maxVertexes = size;
	this->Vertex_num = 0;
	this->Edge_num = 0;
	map<int, Node<T, E>*> vertex_table;
	for (int i = 0; i < this->maxVertexes; ++i) {
		vertex_table[i] = nullptr;//初始化list
	}
}
template<typename T, typename E>
UndiGraph<T, E>::~UndiGraph()
{
	cout << "destructor called" << endl;
	Node<T, E>* p;
	for (int i = 0; i < this->Vertex_num; ++i) {
		p = vertex_table[i];
		listpoint<T, E>* head = p->next;
		while (head != nullptr) {
			listpoint<T, E>* tmp = head;
			head = head->next;
			delete tmp;
		}
		delete p;
	}
}
template<typename T,typename E>
UndiGraph<T, E>::UndiGraph(const UndiGraph& g) {
	//Node<T, E>* tmpNode = new Node<T, E>[g.Vertex_num];
	cout << "copy called" << endl;
	map<int, Node<T, E>*> tmpTable;
	//for (int i = 0; i < g.Vertex_num; ++i) {
		//Node<T, E>* tmpNode = new Node<T, E>();
		//tmpNode = g.vertex_table[i];
	int i = 0; 
	for (auto it = g.vertex_table.begin(); it != g.vertex_table.end(); it++) {
		Node<T, E>* tmpNode = new Node<T, E>();
		tmpNode = it->second;
		tmpTable[i++] = tmpNode;
	}
	//}
	vertex_table = tmpTable;
}
template<typename T, typename E>
UndiGraph<T, E>& UndiGraph<T, E>::operator=(const UndiGraph& rhs) {
	cout << "UndiGraph operator = called" << endl;
	Graph<T, E>::operator=(rhs);
	if (this == &rhs) return *this;
	//map<int, Node<T, E>*> tmpTable;
	int i = 0;
	for (auto it = rhs.vertex_table.begin(); it != rhs.vertex_table.end(); ++it) {
		Node<T, E>* tmpNode = new Node<T, E>();
		*tmpNode = *(it->second);//出问题   指针
		vertex_table[i++] = tmpNode;
	}
	//vertex_table = tmpTable;
	return *this;
}
template<typename T, typename E>
void UndiGraph<T, E>::CreateGraph() {
	int n, m, i, j, q;
	T vertex, vertex1, vertex2;
	E weight;

	cout << "如果边具有权值输入1，否则输入2：";
	cin >> i;
	cout << "如果节点具有权值则输入1，否则输入2：";
	cin >> j;
	cout << "如果有向图输入1，否则输入2：";
	cin >> q;
	this->Weight = (i == 1) ? true : false;
	this->Vertex_value = (j == 1) ? true : false;
	this->Directed = (q == 1) ? true : false;
	cout << "请输入顶点数和边数：";
	cin >> n >> m;
	if (!this->Vertex_value) {

		for (int i = 0; i != n; i++)
		{
			this->InsertVertex(this->Vertex_num);

		}
		cout << "无权值节点创建完毕";
	}
	else {
		cout << "请依次输入节点权值：";
		for (int i = 0; i != n; i++)
		{
			cin >> vertex;
			this->InsertVertex(vertex);
		}
	}
	if (this->Weight)                      //当为带权图
	{
		cout << "如果有方向则按头尾方式输入,如果无方向输入边的两个顶点" << endl;
		cout << "请以这样的方式输入边(顶点(头)，顶点(尾)，权值)：vertex1 vertex2 weight:";
	}
	else                           //当为无权图
	{
		cout << "如果有方向则按头尾方式输入,如果无方向输入边的两个顶点" << endl;
		cout << "请以这样的方式输入边(顶点(头),顶点(尾))：vertex1 vertex2:";
	}
	for (int i = 0; i < m; i++)
	{
		if (this->Weight)       //有权
			cin >> vertex1 >> vertex2 >> weight;
		else              //无权
		{
			cin >> vertex1 >> vertex2;
			weight = 1;
		}
		InsertEdge(vertex1, vertex2, weight);
	}
}

template<typename T, typename E>
bool UndiGraph<T, E>::InsertVertex(T vertex) {
	if (getVertexPos(vertex) != -1) {
		cerr << "顶点已经存在，添加失败！" << endl;
		return false;
	}
	else if (this->Vertex_num == this->maxVertexes) {
		cerr << "图已满，添加失败！" << endl;
		return false;
	}
	else {
		Node<T, E>* p = new Node<T, E>(this->Vertex_num, vertex);
		vertex_table[this->Vertex_num] = p;
		this->Vertex_num++;
		return true;
	}
}

template<typename T, typename E>
bool UndiGraph<T, E>::InsertVertex(T vertex,int _block) {
	if (getVertexPos(vertex) != -1) {
		cerr << "顶点已经存在，添加失败！" << endl;
		return false;
	}
	else if (this->Vertex_num == this->maxVertexes) {
		cerr << "图已满，添加失败！" << endl;
		return false;
	}
	else {
		Node<T, E>* p = new Node<T, E>(this->Vertex_num, vertex);
		vertex_table[this->Vertex_num] = p;
		this->Vertex_num++;
		p->block = _block;
		return true;
	}

}


template<typename T, typename E>
int UndiGraph<T, E>::getVertexPos(T vertex) {
	for (int i = 0; i < this->Vertex_num; ++i) {
		if (vertex_table[i]->value == vertex) {
			return i;
		}
	}
	return -1;
}

//无向图两节点间是否有边 bool
template<typename T, typename E>
bool UndiGraph<T, E>::ExistEdge(T v1, T v2) {
	if (v1 == v2) return true;
	int i = getVertexPos(v1);
	listpoint<T, E>* ptr = vertex_table[i]->next;
	while (ptr) {
		if (!ptr->next) return false;
		if (ptr->next->data == v2) {
			return true;
		}
		ptr = ptr->next;
	}
	return false;
}

template<typename T, typename E>                //此处使用的是头插法 在此判断是否有向图
bool UndiGraph<T, E>::InsertEdge(const T vertex1, const T vertex2, E weight) {
	int i = getVertexPos(vertex1);//节点1的编号 如果未插入先执行插入操作
	int j = getVertexPos(vertex2);//节点2的编号
	if (i == -1) InsertVertex(vertex1), i= getVertexPos(vertex1);
	if (j == -1) InsertVertex(vertex2), j = getVertexPos(vertex2);
	//if (i != -1 && j != -1) {

		if (ExistEdge(vertex1, vertex2)) {
			cout << vertex1 << "," << vertex2 << "该边已经存在，插入失败！" << endl;
			return false;
		}
		if (!this->Directed) {
			listpoint<T, E>* e1;
			e1 = new listpoint<T, E>(vertex2, weight);
			if (vertex_table[i]->next) {
				e1->next = vertex_table[i]->next->next;
				vertex_table[i]->next->next = e1;
			}
			else {
				vertex_table[i]->next = e1;
			}

			listpoint<T, E>* e2;
			e2 = new listpoint<T, E>(vertex1, weight);
			if (vertex_table[j]->next) {
				e2->next = vertex_table[j]->next->next;
				vertex_table[j]->next->next = e2;
			}
			else {
				vertex_table[j]->next = e2;//如果没有元素 则为第一个元素
			}
		}
		else {
			listpoint<T, E>* e1;
			e1 = new listpoint<T, E>(vertex2, weight);
			if (vertex_table[i]->next) {
				e1->next = vertex_table[i]->next->next;
				vertex_table[i]->next->next = e1;
			}
			else {
				vertex_table[i]->next = e1;
			}
		}
		this->Edge_num++;
		return true;
	//}
	cout << '\a' << "有节点不存在" << endl;
	return false;
}

template<typename T, typename E>
void UndiGraph<T, E>::Display() {
	string edge_s, vertex_s;
	if (this->Weight) edge_s = "边有权值,";
	else edge_s = "边无权值,";
	if (this->Vertex_value) vertex_s = "节点有权值";
	else vertex_s = "节点无权值";

	cout << endl << endl << "此图是" << this->Vertex_num << "个顶点的:" << edge_s << vertex_s << "图，顶点分别是：" << endl;
	for (int i = 0; i != this->Vertex_num; i++)
		cout << vertex_table[i]->value << "  ";
	cout << endl;
	cout << "邻接表为：" << endl;
	for (int i = 0; i < this->Vertex_num; i++)
	{
		if (this->Vertex_value)
			cout << "顶点下标为" << i << "::值为" << vertex_table[i]->value << " ->";
		else
			cout << "顶点下标为" << i << " ->";
		listpoint<T, E>* p = this->vertex_table[i]->next;
		while (p != NULL)
		{
			cout << "(" << p->data << ", 权值：" << p->value << ")";
			p = p->next;
			if (p != NULL)
				cout << "->";
		}
		cout << endl << endl;
	}
}

//返回边（v1，v2）的权重，边不存在则返回0  无向图
template <typename T, typename E>
E UndiGraph<T, E>::getweight(T v1, T v2)
{
	int V1_pos = getVertexPos(v1);
	int V2_pos = getVertexPos(v2);
	if (V1_pos == -1 || V2_pos == -1) {
		cerr << "不存在此边" << endl;
		return 0;
	}
	listpoint<T, E>* ptr = vertex_table[V1_pos]->next;
	while (ptr != nullptr && ptr->data != v2) {
		ptr = ptr->next;
	}
	if (ptr != nullptr)
		return ptr->value;
	//return -1;
}

//返回当前节点的全部相邻节点  和权值
template <typename T, typename E>
vector<T> UndiGraph<T, E>::Adj(T vertex) {
	vector<T> v;
	int pos = getVertexPos(vertex);
	if (pos == -1) {
		cerr << "节点出错" << endl;
		return v;
	}
	listpoint<T, E>* ptr = vertex_table[pos]->next;
	while (ptr != nullptr) {
		v.push_back(ptr->data);
		ptr = ptr->next;
	}
	return v;
}

template<typename T, typename E>
void UndiGraph<T, E>::visit(T i) {
	int pos = getVertexPos(i);
	if (pos == -1) {
		cerr << "vertex not found" << endl;
		return;
	}
	vertex_table[pos]->marked = true;
}

//从文件读取数据 创建图
template<typename T, typename E>
void UndiGraph<T, E>::InputfileGraph(string filename)
{
	int n, m, i, j, q;
	T vertex, vertex1, vertex2;
	ifstream fin(filename);
	E weight;
	//cout << "请输入顶点数和边数：";
	fin >> n >> m;
	//cout << "请依次输入顶点：";
	//cout << "如果是有权图输入1，否则输入2：";
	fin >> i;
	//cout << "如果是有向图输入1，否则输入2：";
	fin >> j;
	fin >> q;
	this->Weight = (i == 1) ? true : false;
	this->Vertex_value = (j == 1) ? true : false;
	this->Directed = (q == 1) ? true : false;
	//if (!this->Vertex_value) {
	//	for (int i = 0; i != n; i++)
	//	{
	//		this->InsertVertex(this->Vertex_num);
	//	}
	//}
	//else {
	//	for (int i = 0; i != n; i++)
	//	{
	//		fin >> vertex;
	//		this->InsertVertex(vertex);
	//	}
	//}

	for (int i = 0; i < m; i++)
	{
		if (this->Weight)       //有权
			fin >> vertex1 >> vertex2 >> weight;
		else              //无权
		{
			fin >> vertex1 >> vertex2;
			weight = 1;
		}
		InsertEdge(vertex1, vertex2, weight);
	}
}

template<typename T, typename E>
vector<T> UndiGraph<T, E>::getAllvertex() {
	vector<T> sites;
	typename map<T, Node<T, E>*>::iterator iter;
	for (iter = vertex_table.begin(); iter != vertex_table.end(); ++iter) {
		sites.push_back(iter->second->value);
	}
	return sites;
}

template<typename T, typename E>
void UndiGraph<T, E>::BFSTraverse(T s) {
	vector<bool> visited(this->Vertex_num, false);
	int n = getVertexPos(s);
	visited[n] = true;
	vector<int> edgeTo(this->Vertex_num,-1);
	queue<T> q;//队列存储 需要处理的节点
	q.push(s);
	while (!q.empty()) {
		T tmp = q.front();
		int num = getVertexPos(tmp);
		q.pop();
		for (auto k : Adj(tmp)) {
			n = getVertexPos(k);
			if (!visited[n]) {
				edgeTo[n] = num;
				visited[n] = true;
				q.push(k);
			}
		}
	}
	edgeAll[s] = edgeTo;
	markedAll[s] = visited;
	return;
}

template<typename T, typename E>
bool UndiGraph<T, E>::hasPathTo(T s,T d) {
	if (!markedAll.count(s)) BFSTraverse(s);
	return markedAll[s][getVertexPos(d)];
}

template<typename T, typename E>
vector<int> UndiGraph<T, E>::pathTo(T s,T d) {
	if (!hasPathTo(s, d)) return {-1};
	vector<int> res;
	int num = getVertexPos(d);
	int end = getVertexPos(s);
	for (int x = num; x != end; x = edgeAll[s][x]) {
		res.push_back(x);
	}
	res.push_back(end);
	return res;
}

template<typename T,typename E>
vector<vector<T>> UndiGraph<T, E>::getedgematrix() {
	vector<vector<int>> g(this->Vertex_num,vector<int>(this->Vertex_num,INF));
	for (int i = 0; i < this->Vertex_num; i++)
	{
		g[i][i] = 0;
		listpoint<T, E>* s = vertex_table[i]->next;
		for (; s != nullptr; s = s->next)
		{
			int tmp = getVertexPos(s->data);
			g[i][tmp] = s->value;
		}
			//g[i].push_back(s->data);

	}
	return g;
}

template<typename T,typename E>
int UndiGraph<T, E>::subGetBlock(T vertex) {
	int cur = getVertexPos(vertex);
	int block = this->vertex_table[cur]->block;
	if (block == 0) return -1;//为0的时候 表示不存在街区
	else return block;
}

#endif GRAPH_H_