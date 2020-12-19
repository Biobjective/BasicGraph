/**Ŀǰ����migrate֮��ÿ��blockСͼֻ��洢��block�о��еĽڵ�������ıߣ�
*/
#pragma once
#ifndef BLOCK_GRAPH_H_
#define BLOCK_GRAPH_H_
#include<set>
#include"Graph.h"
#include <windows.h>
static  int  trunkPreWeight = 10;
//����0��N-1֮��������
inline float rand_n(int N) {
	//srand(time(NULL));//������������ӣ�ʹÿ�β�����������в�ͬ	
	LARGE_INTEGER seed;
	QueryPerformanceFrequency(&seed);
	QueryPerformanceCounter(&seed);
	srand(seed.QuadPart);
	return rand() % N;
}
template<typename T, typename E>//T�ڵ��ţ�Eֵ/Ȩ��
class BlockGraph :public UndiGraph<T,E> {
	int block;//block num ��1��ʼ
	int row;// block����
	int col;//��
	vector<vector<int> > bMatrix;//������ƽ��ͼ
	unordered_map<int, vector<T> > rec; //vector �洢��Ӧblock�Ľڵ��š���������int �洢�������
	unordered_map<int, vector<T> > borderPoints;   //�����洢ÿ�����ı߽��
	map<int, UndiGraph<T, E> > subBlockMap;//�����洢�ָ���Сͼ 
	map<pair<int, int>, vector<int> >  trunkLine;//�������ߣ�Ŀǰ����עϸ��·����pair<int, int>Ϊ��ʼ����Ŀ����  vector<int>Ŀǰ�洢  Ӧ��ÿ��block��Ϊһ��pair
	
public:
	BlockGraph();//�޲ι���
	explicit BlockGraph(int size, int _row = 4, int _col =4);//�вι���   **����**
	virtual ~BlockGraph();//����
	virtual void Display();//���ӡ
	virtual void InputfileGraph(string filename);//
	//int getNext(int cur_block,int destination);//��ȡ��ǰ·����һ�ڵ�
	vector<T> getBlockPoint(int block);//��ȡÿ��block��ȫ����
	bool sepBlock();//separate into block ����rec���зָ�
	vector<T> getBorderPoint(int block);//��ȡ��block�����ӵı߽�ڵ�  ����ֵΪ�ڵ㼯��
	vector<int> getBorderBlock(int current, int destination);//��ȡ����ѡ��ı߽�ֵ  ����ǰ�ڵ����һ�ڵ�
	int getBlockid(T vertex);
	void subDisplay();
	bool migrate(UndiGraph<T, E>& G, T vertex,int);//����ͼ�ָ�

	vector<int> getDiffBlockTrunkWeight(int, int);// ��ȡ����block֮��ĸ���Ȩ��   
	void genTrunkLine(int, int);//����TrunkLine��ֵ  Ŀǰ���������  ÿ������֮������2��3��·��  
};


template<typename T, typename E>
BlockGraph<T, E>::BlockGraph() {

	cout << "Please input row and column example as(4 6):  ";
	int _col, _row;
	cin >> _row >> _col;
	this->row = _row;
	this->col = _col;
	bMatrix.resize(_row, vector<int>(_col, 1));
	int last = 0;
	for (auto& s : bMatrix) {
		for (auto& k : s) {
			k = ++last;
		}
	}

}

template<typename T, typename E>
BlockGraph<T, E>::BlockGraph(int size, int _row, int _col) :UndiGraph<T,E>(size),block(_row*_col),row(_row),col(_col)
{
	this->bMatrix.resize(_row, vector<int>(_col, 1));
	int last = 0;
	for (auto& s : bMatrix) {
		for (auto& k : s) {
			k = ++last;
		}
	}
}

template<typename T,typename E>
BlockGraph<T, E>::~BlockGraph() {}

template<typename T, typename E>
void BlockGraph<T, E>::Display()
{
	UndiGraph<T, E>::Display();
	ostream_iterator<int> it(std::cout, " ");
	cout << "��ͼһ����" << block << "��block" << endl;
	for (int i = 1; i <= block;++i) {
		cout << "block id" << i << ":  ";
		copy(rec[i].begin(), rec[i].end(), it);
		cout <<'\n' << endl;
	}
	return;
}

//
template<typename T, typename E>
bool BlockGraph<T, E>::sepBlock() {
	for (int i = 1; i < block; ++i) {
		UndiGraph<T, E> tmp;
		for (auto s : rec[i]) {
			if(this->migrate(tmp, s, i)) continue;
			else {
				cout << "BlockGraph<T, E>::sepBlock() error at migrate " << '\a' << endl;
				system("pause");
				return false;
			}
		}
		subBlockMap[i] = tmp;
		//subBlockMap.insert({ i, tmp });//�����Ӧ�Ľ�����ͼ
	}
	return true;
}

template<typename T, typename E>
void BlockGraph<T, E>::InputfileGraph(string filename) {
	int n, m, i, j, q;
	T vertex, vertex1, vertex2;
	ifstream fin(filename);
	E weight;
	fin >> n >> m;//�������ͱ�����
	fin >> i;
	fin >> j;//�ڵ��Ƿ���ֵ
	fin >> q;
	this->Weight = (i == 1) ? true : false;
	this->Vertex_value = (j == 1) ? true : false;
	this->Directed = (q == 1) ? true : false;

	T curr;
	int block;
	while (n--) {
		fin >> curr >> block;
		rec[block].emplace_back(curr);
		this->InsertVertex(curr, block);
	}
	for (int i = 0; i < m; i++)
	{
		if (this->Weight)       //��Ȩ
			fin >> vertex1 >> vertex2 >> weight;
		else              //��Ȩ
		{
			fin >> vertex1 >> vertex2;
			weight = 1;
		}
		this->InsertEdge(vertex1, vertex2, weight);
	}
	cout << "InputfileGraph success" << '\a'<<endl;
	return;
}

template<typename T, typename E>
vector<T> BlockGraph<T, E>::getBorderPoint(int block) {

}

template<typename T,typename E>
vector<int> BlockGraph<T, E>::getBorderBlock(int cur, int des) {
	if (cur == des) {
		cerr << "already arrive at destination" << endl;
		return { -1 };
	}
	//�����des�պ�ͬ�л�ͬ�оͲ����л�������л���
	if (des % row == cur % row) {
		if (des > cur) return { cur + row };
		else return { cur - row };
	}
	if ((des - 1) / row == (cur - 1) / row) {
		if (des > cur) return { cur + 1 };
		else return { cur - 1 };
	}
	bool updown, lefrig;//true Ϊup/left;  ***ע���˱��Ϊdes��cur��ʲô��λ  ��curΪԭ���Ϊ�ĸ�����
	if ((cur - 1) / row < (des - 1) / row) updown = false;
	else updown = true;
	if (cur % row > des % row) lefrig = false;
	else lefrig = true;
	if (updown && lefrig) return { cur - row,cur - 1 };
	else if (!updown && !lefrig)return { cur + row,cur + 1 };
	else if (!updown && lefrig) return { cur + row,cur - 1 };
	else if (updown && !lefrig) return { cur - row,cur + 1 };
	else return{ -1 };
}

template<typename T,typename E>
int BlockGraph<T, E>::getBlockid(T vertex) {
	int cur = this->getVertexPos(vertex);
	if (cur == -1) cerr << "error in getBlockid when getVertexPos->" << vertex << endl;
	else {
		int block = this->subGetBlock(vertex);
		if (block == -1) cerr << "error in getBlockid when subGetBlock->" << vertex << endl;
		return block;
	}
	return -1;
}
template<typename T, typename E>
void BlockGraph<T, E>::subDisplay() {
	for (auto it = subBlockMap.begin(); it != subBlockMap.end(); ++it) {
		cout <<'\n'<< "Sub Block " << it->first << " - Display:";
		it->second.Display();
		cout << "<--------***BLOCK END***------->" << endl << endl;
	}
	return;
}

template<typename T, typename E>
bool BlockGraph<T, E>::migrate(UndiGraph<T, E>& G, T vertex,int blockId) {
	int pos = this->getVertexPos(vertex);//��ȡ�ڵ���
	listpoint<T, E>* ptr = this->vertex_table[pos]->next;//��ȡ������ͷ�ڵ�
	//vector<T> adjVertex = this->Adj(vertex);
	vector<T> adjVertex = getBlockPoint(vertex);
	set<T> setVertex(adjVertex.begin(), adjVertex.end());
	while (ptr != nullptr) {
		//����ɸѡ��ֻ���ڸ�block�ĵ��йصı߽��� migrate
		if (setVertex.count(ptr->data)!=0) {
			if (G.InsertEdge(vertex, ptr->data, ptr->value))
				ptr = ptr->next;
			else {
				cout << "UndiGraph<T, E>::migrate error at insert " << vertex << ptr->data << ptr->value << '\a' << endl;
				system("pause");
				return false;
			}
		}
		else ptr = ptr->next;
	}
	return true;
}

template<typename T, typename E>
vector<T> BlockGraph<T, E>::getBlockPoint(int block) {
	if (rec.count(block) != 0) return rec[block];
	else cerr << "UndiGraph<T, E>::getBlockPoint error at get block ID " << block << endl;
	return {};
}

template<typename T, typename E>
vector<int> BlockGraph<T, E>::getDiffBlockTrunkWeight(int b1,int b2) {
	int x = trunkLine.count({ b1,b2 });
	int y = trunkLine.count({ b2,b1 });
	//���ж�map����û������������trunk weight
	if (x == 0 && y == 0) {
		//û���򱨴�
		cerr << "error in BlockGraph<T, E>::getDiffBlockTrunkWeight while get:" << b1 << ':' << b2 << endl;
		return {};
	}
	if (x == 0) return trunkLine[{b2, b1}];
	else return trunkLine[{b1, b2}];
}

//ֻ�ܸ��ݸ���block����  ������������ظ����� ��������׷��insertTrunkLine�ӿ�
template<typename T, typename E>
void BlockGraph<T, E>::genTrunkLine(int b1, int b2) {
	int x = trunkLine.count({ b1,b2 });
	int y = trunkLine.count({ b2,b1 });
	if (x != 0 || y != 0) return;
	//�˴���ʼѡ����������trunk��������trunk
	int lines;
	int tmp = rand_n(2);//inline function
	if (tmp == 0) lines = 2;
	else lines = 3;

}
//template<typename T, typename E>
//int BlockGraph<T, E>::getNext(int cur,int des) {
//	if (cur == des) {
//		cout << "already arrive at destination" << endl;
//		return -1;
//	}
//	//�����des�պ�ͬ�л�ͬ�оͲ����л�������л���
//	if (des % row == cur % row) {
//		return des > cur ? cur + row : cur - row;
//	}
//	if (des % col == cur % col) {
//		return des > cur ? cur + col : cur - col;
//	}
//
//}

#endif 