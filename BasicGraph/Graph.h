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
//	//������
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

/*ͼ��-----����*/
template<typename T, typename E>
class Graph
{
protected:
	int  maxVertexes;      //�����ڴ�ʱ�����Ķ�����
	int  Vertex_num;		//������Ŀ
	int  Edge_num;			//����Ŀ
	bool Weight = true;			//�Ƿ���Ȩ
	bool Directed = false;			//�Ƿ�����
	bool Vertex_value = true;		//�ڵ��Ƿ���ֵ
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

/*�ڵ���*/
template<typename T, typename E>//T�ڵ� E��Ȩ��
class Node
{
public:
	int serial;         //������
	T value;			//�ڵ����ֵ���粻������Ĭ��������ͬ
	bool marked;       //������ʱ�־λ��true��ʾ�Ѿ�������
	int block = 0;
	listpoint<T, E>* next;//��ö������ڵĶ����б�

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

//����
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

//����ͼ��  ����T��Ϊint string �� float��   EΪֵ
template<typename T, typename E>
class UndiGraph :public Graph<T, E>
{
protected:
	map<int, Node<T, E>*> vertex_table;		//������źͶ������ӳ��  �ڽӱ�  ֻ��ÿ������ͷ���   ע����int��ŵ�ӳ�� ����T
	unordered_map<T, vector<int> >  edgeAll;
	unordered_map<T, vector<bool> >  markedAll;//���ڼ��� hasPathTo
public:
	map<int, vector<pair<T,T> > > border_map;//���ڴ洢 ��ͼͨ������ͼ�� �߽�   ��һ��intΪblock id ,vector�洢��ͼͨ����һ��ͼ�����б�
public:
	explicit UndiGraph(int size = 20);
	virtual ~UndiGraph();
	UndiGraph(const UndiGraph&);//�������캯���������
	UndiGraph& operator=(const UndiGraph& rhs);
	virtual void Display();
	virtual void InputfileGraph(string filename);

	void CreateGraph();
	bool InsertVertex(T vertex);
	bool InsertVertex(T vertex,int _block);
	int getVertexPos(T vertex);//��ȡ�ڵ���
	bool InsertEdge(const T vertex1, const T vertex2, E weight);
	bool ExistEdge(T v1, T v2);
	E getweight(T v1, T v2);
	vector<T> Adj(T vertex);//��ȡ���ڽڵ�
	void visit(T i);
	vector<T> getAllvertex();
	void BFSTraverse(T s);
	bool hasPathTo(T s, T d);//�����ڵ���Ƿ���ͨ
	vector<int> pathTo(T s,T d);
	vector<vector<T>> getedgematrix();//���ڽӾ������vector<vector<E>>��
	
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
		vertex_table[i] = nullptr;//��ʼ��list
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
		*tmpNode = *(it->second);//������   ָ��
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

	cout << "����߾���Ȩֵ����1����������2��";
	cin >> i;
	cout << "����ڵ����Ȩֵ������1����������2��";
	cin >> j;
	cout << "�������ͼ����1����������2��";
	cin >> q;
	this->Weight = (i == 1) ? true : false;
	this->Vertex_value = (j == 1) ? true : false;
	this->Directed = (q == 1) ? true : false;
	cout << "�����붥�����ͱ�����";
	cin >> n >> m;
	if (!this->Vertex_value) {

		for (int i = 0; i != n; i++)
		{
			this->InsertVertex(this->Vertex_num);

		}
		cout << "��Ȩֵ�ڵ㴴�����";
	}
	else {
		cout << "����������ڵ�Ȩֵ��";
		for (int i = 0; i != n; i++)
		{
			cin >> vertex;
			this->InsertVertex(vertex);
		}
	}
	if (this->Weight)                      //��Ϊ��Ȩͼ
	{
		cout << "����з�����ͷβ��ʽ����,����޷�������ߵ���������" << endl;
		cout << "���������ķ�ʽ�����(����(ͷ)������(β)��Ȩֵ)��vertex1 vertex2 weight:";
	}
	else                           //��Ϊ��Ȩͼ
	{
		cout << "����з�����ͷβ��ʽ����,����޷�������ߵ���������" << endl;
		cout << "���������ķ�ʽ�����(����(ͷ),����(β))��vertex1 vertex2:";
	}
	for (int i = 0; i < m; i++)
	{
		if (this->Weight)       //��Ȩ
			cin >> vertex1 >> vertex2 >> weight;
		else              //��Ȩ
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
		cerr << "�����Ѿ����ڣ����ʧ�ܣ�" << endl;
		return false;
	}
	else if (this->Vertex_num == this->maxVertexes) {
		cerr << "ͼ���������ʧ�ܣ�" << endl;
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
		cerr << "�����Ѿ����ڣ����ʧ�ܣ�" << endl;
		return false;
	}
	else if (this->Vertex_num == this->maxVertexes) {
		cerr << "ͼ���������ʧ�ܣ�" << endl;
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

//����ͼ���ڵ���Ƿ��б� bool
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

template<typename T, typename E>                //�˴�ʹ�õ���ͷ�巨 �ڴ��ж��Ƿ�����ͼ
bool UndiGraph<T, E>::InsertEdge(const T vertex1, const T vertex2, E weight) {
	int i = getVertexPos(vertex1);//�ڵ�1�ı�� ���δ������ִ�в������
	int j = getVertexPos(vertex2);//�ڵ�2�ı��
	if (i == -1) InsertVertex(vertex1), i= getVertexPos(vertex1);
	if (j == -1) InsertVertex(vertex2), j = getVertexPos(vertex2);
	//if (i != -1 && j != -1) {

		if (ExistEdge(vertex1, vertex2)) {
			cout << vertex1 << "," << vertex2 << "�ñ��Ѿ����ڣ�����ʧ�ܣ�" << endl;
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
				vertex_table[j]->next = e2;//���û��Ԫ�� ��Ϊ��һ��Ԫ��
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
	cout << '\a' << "�нڵ㲻����" << endl;
	return false;
}

template<typename T, typename E>
void UndiGraph<T, E>::Display() {
	string edge_s, vertex_s;
	if (this->Weight) edge_s = "����Ȩֵ,";
	else edge_s = "����Ȩֵ,";
	if (this->Vertex_value) vertex_s = "�ڵ���Ȩֵ";
	else vertex_s = "�ڵ���Ȩֵ";

	cout << endl << endl << "��ͼ��" << this->Vertex_num << "�������:" << edge_s << vertex_s << "ͼ������ֱ��ǣ�" << endl;
	for (int i = 0; i != this->Vertex_num; i++)
		cout << vertex_table[i]->value << "  ";
	cout << endl;
	cout << "�ڽӱ�Ϊ��" << endl;
	for (int i = 0; i < this->Vertex_num; i++)
	{
		if (this->Vertex_value)
			cout << "�����±�Ϊ" << i << "::ֵΪ" << vertex_table[i]->value << " ->";
		else
			cout << "�����±�Ϊ" << i << " ->";
		listpoint<T, E>* p = this->vertex_table[i]->next;
		while (p != NULL)
		{
			cout << "(" << p->data << ", Ȩֵ��" << p->value << ")";
			p = p->next;
			if (p != NULL)
				cout << "->";
		}
		cout << endl << endl;
	}
}

//���رߣ�v1��v2����Ȩ�أ��߲������򷵻�0  ����ͼ
template <typename T, typename E>
E UndiGraph<T, E>::getweight(T v1, T v2)
{
	int V1_pos = getVertexPos(v1);
	int V2_pos = getVertexPos(v2);
	if (V1_pos == -1 || V2_pos == -1) {
		cerr << "�����ڴ˱�" << endl;
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

//���ص�ǰ�ڵ��ȫ�����ڽڵ�  ��Ȩֵ
template <typename T, typename E>
vector<T> UndiGraph<T, E>::Adj(T vertex) {
	vector<T> v;
	int pos = getVertexPos(vertex);
	if (pos == -1) {
		cerr << "�ڵ����" << endl;
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

//���ļ���ȡ���� ����ͼ
template<typename T, typename E>
void UndiGraph<T, E>::InputfileGraph(string filename)
{
	int n, m, i, j, q;
	T vertex, vertex1, vertex2;
	ifstream fin(filename);
	E weight;
	//cout << "�����붥�����ͱ�����";
	fin >> n >> m;
	//cout << "���������붥�㣺";
	//cout << "�������Ȩͼ����1����������2��";
	fin >> i;
	//cout << "���������ͼ����1����������2��";
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
		if (this->Weight)       //��Ȩ
			fin >> vertex1 >> vertex2 >> weight;
		else              //��Ȩ
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
	queue<T> q;//���д洢 ��Ҫ����Ľڵ�
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
	if (block == 0) return -1;//Ϊ0��ʱ�� ��ʾ�����ڽ���
	else return block;
}

#endif GRAPH_H_