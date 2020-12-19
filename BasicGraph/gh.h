/******************************�ʺ϶���൫�Ǳ����ٵ��������Ϊ���ڽӾ����˷ѿռ䣬����ѡ�����ڽ�����**************************************/
#ifndef ADGLISTGRAPH_H
#define ADGLISTGRAPH_H
#include<iostream>
#include<string>
#include<fstream>
#include<queue>
#include<stack>
#include"graph.h"
using namespace std;
template<typename T, typename E>    //�߱��㣬����ڽӶ�����±꣬Ȩֵ������һ�߱����ָ��
struct EdgeNode
{
	int adjvex;
	E  weight;
	EdgeNode<T, E>* next;

	EdgeNode() {}
	EdgeNode(int num, E cost) :adjvex(num), weight(cost), next(NULL) {}
	bool operator != (EdgeNode<T, E>& R)const
	{
		return (adjvex != R.adjvex) ? true : false;
	}
};

template<typename T, typename E >  //������㣬��Ŷ���ͱ������ͷָ��
struct VertexNode
{
	int in;    //�˴�����ÿ��������ȵ���Ϣ
	T data;
	EdgeNode<T, E>* firstedge;    //�������ͷָ��  
};

template <typename T, typename E>
class AdjlistGraph :public Graph<T, E>
{
public:
	AdjlistGraph(int size = 20);
	~AdjlistGraph();
	AdjlistGraph(const AdjlistGraph&);
	void display();
	void display1();
	T getvertex(int v);
	E getweight(int v1, int v2);
	int FirstAdjVex(int v);
	int NextAdjVex(int v, int w);
	int getVertexPos(T vertex);
	bool InsertVertex(const T& vertex);
	bool InsertEdge(const T vertex1, const T vertex2, E weight);
	bool removeEdge(const T vertex1, const T vertex2);
	bool removeVertex(const T vertex); //������
	void CreateGraph();
	void InputfileGraph(string filename);
	void DFSTraverse();
	void DFS(int i, vector<bool>& visited);
	void visit(int i);
	void BFSTraverse();
	int& getin(int i);
private:
	VertexNode<T, E>* VertexTable; //�����  
};

template<typename T, typename E>
AdjlistGraph<T, E>::AdjlistGraph(const AdjlistGraph<T, E>& rhs)    //�������캯��,����ʱ������г�Ա������Ҫ����������Ϊ��ȿ������������ڽӱ����������ǹ���ָ�롣��
{
	maxVertexes = rhs.maxVertexes;
	numVertexes = rhs.numVertexes;
	numEdges = rhs.numEdges;
	Weight = rhs.Weight;
	Directed = rhs.Directed;
	VertexNode<T, E>* p = new VertexNode<T, E>[numVertexes];
	for (int i = 0; i < numVertexes; i++)
	{
		p[i].in = rhs.VertexTable[i].in;
		p[i].data = rhs.VertexTable[i].data;
		p[i].firstedge = NULL;
		EdgeNode<T, E>* p2 = rhs.VertexTable[i].firstedge;
		if (p2 != NULL)
		{
			EdgeNode<T, E>* ptr1 = new EdgeNode<T, E>();
			p[i].firstedge = ptr1;
			ptr1->adjvex = p2->adjvex;
			ptr1->weight = p2->weight;
			ptr1->next = NULL;
			while (p2->next != NULL)
			{
				ptr1->next = new EdgeNode<T, E>();
				ptr1 = ptr1->next;
				p2 = p2->next;
				ptr1->adjvex = p2->adjvex;
				ptr1->weight = p2->weight;
				ptr1->next = NULL;
			}
		}
	}
	VertexTable = p;
}
template<typename T, typename E>
int& AdjlistGraph<T, E>::getin(int i)  //���ĳ������i�����
{
	return VertexTable[i].in;
}

template <typename T, typename E>
AdjlistGraph<T, E>::AdjlistGraph(int size)
{
	maxVertexes = size;
	numVertexes = 0;
	numEdges = 0;
	VertexTable = new VertexNode<T, E>[maxVertexes];
	if (VertexTable == NULL)
	{
		cerr << "�洢�����" << endl;
		exit(1);
	}
	for (int i = 0; i < maxVertexes; i++)  //��ʼ���ڽӱ�
	{
		VertexTable[i].firstedge = NULL;
		VertexTable[i].in = 0;  //��ʼ�������Ϣ
	}
}
template<typename T, typename E>
AdjlistGraph<T, E>::~AdjlistGraph()
{
	for (int i = 0; i < numVertexes; i++)
	{
		EdgeNode<T, E>* p = VertexTable[i].firstedge;
		while (p != NULL)
		{
			VertexTable[i].firstedge = p->next;
			delete p;
			p = VertexTable[i].firstedge;
		}
	}
	delete[]VertexTable;
}
template<typename T, typename E>
void AdjlistGraph<T, E>::CreateGraph()
{
	int n, m, i, j;
	T vertex, vertex1, vertex2;
	E weight;
	cout << "�����붥�����ͱ�����";
	cin >> n >> m;
	cout << "���������붥�㣺";
	for (int i = 0; i != n; i++)
	{
		cin >> vertex;
		this->InsertVertex(vertex);
	}
	cout << "�������Ȩͼ����1����������2��";
	cin >> i;
	cout << "���������ͼ����1����������2��";
	cin >> j;
	if (i == 1)
		Weight = true;
	else
		Weight = false;
	if (j == 1)
		Directed = true;
	else
		Directed = false;
	if (Weight)                      //��Ϊ��Ȩͼ
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
		if (Weight)       //��Ȩ
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
bool AdjlistGraph<T, E>::InsertVertex(const T& vertex)
{
	int i = getVertexPos(vertex);
	if (i != -1)
	{
		cerr << "�����Ѿ����ڣ����ʧ�ܣ�" << endl;
		return false;
	}
	if (numVertexes == maxVertexes)
		return false;          //ͼ����������ʧ��  
	VertexTable[numVertexes].data = vertex;
	numVertexes++;
	return true;
}
template<typename T, typename E>                //�˴�ʹ�õ���ͷ�巨
bool AdjlistGraph<T, E>::InsertEdge(const T vertex1, const T vertex2, E weight)
{
	int i = getVertexPos(vertex1);
	int j = getVertexPos(vertex2);
	if (i != -1 && j != -1)
	{
		EdgeNode<T, E>* p = VertexTable[i].firstedge;   //�ȼ��ñ��Ƿ��Ѿ�����
		while (p != NULL && p->adjvex != j)
			p = p->next;
		if (p != NULL)//�ҵ��ñ�  
		{
			cout << "�ñ��Ѿ����ڣ�����ʧ�ܣ�" << endl;
			return false;
		}
		if (!Directed)        //����ͼ,�ڽӱ�Ҳ����ԳƵ�
		{
			EdgeNode<T, E>* e1;
			e1 = new EdgeNode< T, E >(j, weight);
			e1->next = VertexTable[i].firstedge;
			VertexTable[i].firstedge = e1;

			EdgeNode<T, E>* e2;
			e2 = new EdgeNode< T, E >(i, weight);
			e2->next = VertexTable[j].firstedge;
			VertexTable[j].firstedge = e2;
		}
		else                 //����ͼ
		{
			EdgeNode<T, E>* e1;
			e1 = new EdgeNode< T, E >(j, weight);
			e1->next = VertexTable[i].firstedge;
			VertexTable[i].firstedge = e1;
			VertexTable[j].in++;     //�����ʱ�������ȣ����򲻴���
		}
		numEdges++;
		return true;
	}
	return false;
}
template<typename T, typename E>
void AdjlistGraph<T, E>::display()
{
	string s1, s2;
	if (Directed) s1 = "����";
	else         s1 = "����";
	if (Weight)   s2 = "��Ȩ";
	else         s2 = "��Ȩ";
	cout << "��ͼ��" << numVertexes << "�������" << s1 << s2 << "ͼ������ֱ��ǣ�" << endl;
	for (int i = 0; i != numVertexes; i++)
		cout << VertexTable[i].data << "  ";
	cout << endl;
	cout << "�ڽӱ�Ϊ��" << endl;
	for (int i = 0; i < numVertexes; i++)
	{
		if (!Directed)
			cout << "�����±�Ϊ" << i << " " << VertexTable[i].data << " ->";
		else
			cout << "�����±�Ϊ" << i << " �����Ϊ��" << VertexTable[i].in << "  " << VertexTable[i].data << " ->";
		EdgeNode<T, E>* p = VertexTable[i].firstedge;
		while (p != NULL)
		{
			cout << "(" << p->adjvex << ", " << p->weight << ")";
			p = p->next;
			if (p != NULL)
				cout << "->";
		}
		cout << endl;
	}
}
template<typename T, typename E>
void AdjlistGraph<T, E>::display1()
{
	T vertex1, vertex2;
	E cost;
	cout << "��ͼ������Ϊ��" << numVertexes << "����Ϊ" << numEdges << "����ıߵ���ϢΪ��" << endl;
	if (!Directed)//�������
	{
		for (int i = 0; i < numVertexes; i++)
		{
			for (int j = i + 1; j < numVertexes; j++)
			{
				cost = this->getweight(i, j);
				if (cost != 0)  //�ߴ��ڵ������
				{
					vertex1 = this->getvertex(i);
					vertex2 = this->getvertex(j);
					if (Weight)
						cout << "(" << vertex1 << "," << vertex2 << "," << cost << ")" << endl;
					else
						cout << "(" << vertex1 << "," << vertex2 << ")" << endl;
				}
			}
		}
	}
	else         //�������
	{
		for (int i = 0; i < numVertexes; i++)
		{
			for (int j = 0; j < numVertexes; j++)
			{
				cost = this->getweight(i, j);
				if (cost != 0)  //�ߴ��ڵ������
				{
					vertex1 = this->getvertex(i);
					vertex2 = this->getvertex(j);
					if (Weight)
						cout << "(" << vertex1 << "->" << vertex2 << "," << cost << ")" << endl;
					else
						cout << "(" << vertex1 << "->" << vertex2 << ")" << endl;
				}
			}
		}
	}
}

template <typename T, typename E>
E AdjlistGraph<T, E>::getweight(int v1, int v2) //���رߣ�v1��v2����Ȩ�أ��߲������򷵻�0  
{
	if (v1 != -1 && v2 != -1)
	{
		EdgeNode<T, E>* p = VertexTable[v1].firstedge;
		while (p != NULL && p->adjvex != v2)
			p = p->next;
		if (p != NULL)
			return p->weight;
	}
	return 0;
}
template<typename T, typename E>
T AdjlistGraph<T, E>::getvertex(int v)
{
	return (v >= 0 && v < numVertexes) ? VertexTable[v].data : 0;
}
template<class T, class E>
int AdjlistGraph<T, E>::FirstAdjVex(int v) //���v�ĵ�һ���ڽӶ����λ�� 
{
	if (v != -1)
	{
		EdgeNode<T, E>* p = VertexTable[v].firstedge;
		if (p != NULL)
			return p->adjvex;
	}
	return -1;
}
template<typename T, typename E>
int AdjlistGraph<T, E>::NextAdjVex(int v, int w) //���v���ڽӶ���w����һ���ڽӶ���  
{
	if (v != -1)
	{
		EdgeNode<T, E>* p = VertexTable[v].firstedge;
		while (p != NULL && p->adjvex != w)       //Ѱ���ڽӶ���w  
			p = p->next;
		if (p != NULL && p->next != NULL) //�ҵ�w�Ҵ�����һ���ڽӶ���  
			return p->next->adjvex;
	}
	return -1;
}
template<typename T, typename E>
int AdjlistGraph<T, E>::getVertexPos(T vertex)
{
	for (int i = 0; i < numVertexes; i++)
	{
		if (VertexTable[i].data == vertex)
			return i;
	}
	return -1;
}

template<typename T, typename E>
bool AdjlistGraph<T, E>::removeEdge(const T vertex1, const T vertex2)
{
	int i = getVertexPos(vertex1);
	int j = getVertexPos(vertex2);
	if (i != -1 && j != -1)
	{
		if (!Directed)  //�������
		{
			EdgeNode<T, E>* p = VertexTable[i].firstedge, * q = NULL, * s = p;
			while (p != NULL && p->adjvex != j)   //���ҵ��ñ�  
			{
				q = p;
				p = p->next;
			}
			if (p != NULL)  //�ҵ��붥��vertex1������һ����vertex2�ı�
			{
				if (p == s)//��һ���ڵ���ҵ�  
					VertexTable[i].firstedge = p->next;
				else
					q->next = p->next;
				delete p;
			}
			else
				return false;
			EdgeNode<T, E>* p1 = VertexTable[j].firstedge, * q1 = NULL, * s1 = p1;
			while (p1 != NULL && p1->adjvex != i)   //���ҵ��ñ�  
			{
				q1 = p1;
				p1 = p1->next;
			}
			if (p1 != NULL)  //�ҵ��붥��vertex2������һ����vertex1�ı�
			{
				if (p1 == s)//��һ���ڵ���ҵ�  
					VertexTable[j].firstedge = p1->next;
				else
					q1->next = p1->next;
				delete p1;
			}
		}
		else  //�������
		{
			EdgeNode<T, E>* p = VertexTable[i].firstedge, * q = NULL, * s = p;
			while (p != NULL && p->adjvex != j)   //���ҵ��ñ�  
			{
				q = p;
				p = p->next;
			}
			if (p != NULL)  //�ҵ��붥��vertex1������һ����vertex2�ı�
			{
				if (p == s)//��һ���ڵ���ҵ�  
					VertexTable[i].firstedge = p->next;
				else
					q->next = p->next;
				VertexTable[j].in--;
				delete p;
			}
			else           //û���ҵ��ñߣ�����false
				return false;
		}
		numEdges--;
		return true;
	}
	return false;
}
template<typename T, typename E>
bool AdjlistGraph<T, E>::removeVertex(const T vertex)   //�����ƣ����������ڽӱ���Ҫ�����˸�������ĳ��ȣ������ʺ�����ͼ�����ɾ�������������ͼ��Ҫ������������ָ��˶�������(Ҳ�������)
{
	int v = getVertexPos(vertex);
	if (v == -1 || v >= numVertexes || numVertexes == 1)
		return false;      //ֻʣһ�����㣬��ɾ��
	EdgeNode<T, E>* p, * s, * t;
	int  k;
	while (VertexTable[v].firstedge != NULL)     //ɾ���ö��㣬�Լ���֮�ڽӵĶ����еļ�¼ ������ֻ�����˴˶���ָ���������������û������������ָ��˶��������� 
	{
		p = VertexTable[v].firstedge;
		k = p->adjvex;
		s = VertexTable[k].firstedge;
		t = NULL;
		while (s != NULL && s->adjvex != v) //�ڶԳƵ���ڽӱ�������v��ɾ���� ,û���ҵ�sΪNULL 
		{
			t = s;
			s = s->next;
		}
		if (s != NULL)
		{
			if (t == NULL) //��һ���ڽӶ������v  
				VertexTable[k].firstedge = s->next;
			else
				t->next = s->next;
			delete s;
		}
		VertexTable[v].firstedge = p->next;
		delete p;
		numEdges--;
	}
	numVertexes--;
	VertexTable[v].data = VertexTable[numVertexes].data;     //�ö���������һ���������Ҫɾ���Ķ��� 
	p = VertexTable[v].firstedge = VertexTable[numVertexes].firstedge;
	while (p != NULL)      //����֮��Ҫ�޸ı߱������һ�����������ӵ��±�adjvex(��������¿��ԣ��������Ļ�����������)
	{
		s = VertexTable[p->adjvex].firstedge;
		while (s != NULL)
		{
			if (s->adjvex == numVertexes)
			{
				s->adjvex = v;
				break;
			}
			else
				s = s->next;
		}
		p = p->next;
	}
	return true;
}

template<typename T, typename E>
void AdjlistGraph<T, E>::InputfileGraph(string filename)
{
	int n, m, i, j;
	T vertex, vertex1, vertex2;
	ifstream fin(filename);
	E weight;
	//cout << "�����붥�����ͱ�����";
	fin >> n >> m;
	//cout << "���������붥�㣺";
	for (int i = 0; i != n; i++)
	{
		fin >> vertex;
		this->InsertVertex(vertex);
	}
	//cout << "�������Ȩͼ����1����������2��";
	fin >> i;
	//cout << "���������ͼ����1����������2��";
	fin >> j;
	if (i == 1)
		Weight = true;
	else
		Weight = false;
	if (j == 1)
		Directed = true;
	else
		Directed = false;
	for (int i = 0; i < m; i++)
	{
		if (Weight)       //��Ȩ
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
void AdjlistGraph<T, E>::DFSTraverse()   //������ȱ���,�ݹ鷽ʽ����
{
	vector<bool> visited(numVertexes, false);
	//for (int i = 0; i < numVertexes; i++)
	//visited[i] = false;    //�ȳ�ʼ�����ж���Ϊδ����״̬
	for (int j = 0; j < numVertexes; j++)
		if (!visited[j])
			DFS(j, visited);
}
template<typename T, typename E>
void AdjlistGraph<T, E>::DFS(int i, vector<bool>& visited)
{
	visited[i] = true;
	visit(i);
	EdgeNode<T, E>* p = VertexTable[i].firstedge;
	while (p != NULL)
	{
		if (!visited[p->adjvex])
			DFS(p->adjvex, visited);
		p = p->next;
	}
}
template<typename T, typename E>
void AdjlistGraph<T, E>::visit(int i)
{
	cout << VertexTable[i].data << "  ";
}
template<typename T, typename E>
void AdjlistGraph<T, E>::BFSTraverse()   //������ȱ���,���ö��У�������������
{
	vector<bool> visited(numVertexes, false);
	//for (int i = 0; i < numVertexes; i++)
	//visited[i] = false;    //�ȳ�ʼ�����ж���Ϊδ����״̬
	queue<int> q;  //�洢�������
	for (int i = 0; i < numVertexes; i++)
	{
		if (!visited[i])
		{
			q.push(i);
			visited[i] = true;
			visit(i);
			while (!q.empty())
			{
				int j = q.front();
				q.pop();
				for (int k = FirstAdjVex(j); k >= 0; k = NextAdjVex(j, k))
					if (!visited[k])
					{
						q.push(k);
						visited[k] = true;
						visit(k);
					}
			}
		}
	}
}
#endif ADGLISTGRAPH_H



