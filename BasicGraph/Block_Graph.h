/**目前是在migrate之后，每个block小图只会存储该block中具有的节点相关联的边，
*/
#pragma once
#ifndef BLOCK_GRAPH_H_
#define BLOCK_GRAPH_H_
#include<set>
#include"Graph.h"
#include <windows.h>

//假定为图中全部权重的均值
static  int  trunkPreWeight = 10;

//生成0到N-1之间的随机数
static int rand_n(int N) {
	//srand(time(NULL));//设置随机数种子，使每次产生的随机序列不同	
	LARGE_INTEGER seed;
	QueryPerformanceFrequency(&seed);
	QueryPerformanceCounter(&seed);
	srand(seed.QuadPart);
	return rand() % N;
}

template<typename T, typename E>//T节点编号，E值/权重
class BlockGraph :public UndiGraph<T,E> {
	int block;//block num 从1开始
	int row;// block的行
	int col;//列
	vector<vector<int> > bMatrix;//街区的平面图
	unordered_map<int, vector<T> > rec; //vector 存储对应block的节点编号—————int 存储街区编号
	unordered_map<int, vector<T> > borderPoints;   //用来存储每个区的边界点
	map<int, UndiGraph<T, E> > subBlockMap;//用来存储分割后的小图 
	//map<pair<int, int>, vector<int> >  trunkLine;//跨区干线，目前不关注细分路径，pair<int, int>为起始区到目标区  vector<int>目前存储  应将每组block作为一个pair
	map<pair<int, int>, vector<pair<T,E> > >  trunkLine;//干线更新版本,vector<pair<int,int> >存储每一组trunkline,weight可以通过getWeight获取


public:
	BlockGraph();//无参构造
	explicit BlockGraph(int size, int _row = 4, int _col =4);//有参构造   **常用**
	virtual ~BlockGraph();//析构
	virtual void Display();//虚打印
	virtual void InputfileGraph(string filename);//
	//int getNext(int cur_block,int destination);//获取当前路径下一节点
	vector<T> getBlockPoint(int block);//获取每个block的全部点
	bool sepBlock();//separate into block 根据rec进行分割
	vector<T> getBorderPoint(int block);//获取与block相连接的边界节点  返回值为节点集合
	vector<int> getBorderBlock(int current, int destination);//获取可以选择的边界值  即当前节点的下一节点
	int getBlockid(T vertex);
	void subDisplay();
	bool migrate(UndiGraph<T, E>& G, T vertex,int);//将大图分割

	//vector<int> getDiffBlockTrunkWeight(int, int);// 获取两个block之间的干线权重   
	//void genTrunkLine(int, int);//生成TrunkLine的值  目前是随机生成  每两个区之间生成2或3条路径  
	bool handleTrunkLine(ifstream&);
	pair<T,E> getBlockTrunkLine(int, int, T);
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
	cout << "此图一共有" << block << "个block" << endl;
	for (int i = 1; i <= block;++i) {
		cout << "block id" << i << ":  ";
		copy(rec[i].begin(), rec[i].end(), it);
		cout <<'\n' << endl;
	}
	return;
}

//分区的同时负责生成trunkline
template<typename T, typename E>
bool BlockGraph<T, E>::sepBlock() {
	for (int i = 1; i <= block; ++i) {
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
		//subBlockMap.insert({ i, tmp });//插入对应的街区子图
	}
	for (auto it = this->vertex_table.begin(); it != this->vertex_table.end(); it++) {
		Node<T, E>& tmpNode = *(it->second);
		T cur_node = tmpNode.value;
		int block_id = getBlockid(cur_node);
		auto* cur = tmpNode.next;
		while (cur) {
			T tmp_data = cur->data;
			int tmp_id = getBlockid(tmp_data);
			//如果该边的 起始节点和终点不在同一个block
			if (tmp_id != block_id) {
				if (this->subBlockMap[block_id].border_map.count(tmp_id) != 0) {
					this->subBlockMap[block_id].border_map[tmp_id].emplace_back(pair<T, T>{ cur_node ,tmp_data });
				}
				else {
					vector<pair<T, T> > tmp_vec;
					tmp_vec.emplace_back(pair<T, T>{ cur_node ,tmp_data });
					this->subBlockMap[block_id].border_map[tmp_id] = tmp_vec;
				}
			}
			cur = cur->next;
		}
	}

	vector<int> tmpBlock;
	//用一个vector存储目前的所有block
	for (int i = 1; i <= block; ++i) tmpBlock.emplace_back(i);
	//每次把vector中第一个元素删除，防止生成trunkline时候有重复的 如：{1，2}和{2，1}
	while (!tmpBlock.empty()) {
		int current = tmpBlock[0];
		tmpBlock.erase(tmpBlock.begin());
		for (int s : tmpBlock) {
			//genTrunkLine(current, s);
		}
	}
	return true;
}

template<typename T, typename E>
void BlockGraph<T, E>::InputfileGraph(string filename) {
	int n, m, i, j, q;
	T vertex, vertex1, vertex2;
	ifstream fin(filename);
	E weight;
	fin >> n >> m;//顶点数和边数量
	fin >> i;
	fin >> j;//节点是否有值
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
		if (this->Weight)       //有权
			fin >> vertex1 >> vertex2 >> weight;
		else              //无权
		{
			fin >> vertex1 >> vertex2;
			weight = 1;
		}
		this->InsertEdge(vertex1, vertex2, weight);
	}

	bool test = sepBlock();
	if (test) cout << "sepBlock() sucess" << endl;
	else cout << "sepBlock() failed" << endl;
	bool testf = handleTrunkLine(fin);
	if(testf) cout << "handleTrunkLine() sucess" << endl;
	else cout << "handleTrunkLine() failed********" << endl;
	cout << "InputfileGraph finished" << '\a'<<endl;
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
	//如果和des刚好同行或同列就不用切换到别的行或列
	if (des % row == cur % row) {
		if (des > cur) return { cur + row };
		else return { cur - row };
	}
	if ((des - 1) / row == (cur - 1) / row) {
		if (des > cur) return { cur + 1 };
		else return { cur - 1 };
	}
	bool updown, lefrig;//true 为up/left;  ***注：此标记为des在cur的什么方位  以cur为原点分为四个象限
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
	int pos = this->getVertexPos(vertex);//获取节点编号
	listpoint<T, E>* ptr = this->vertex_table[pos]->next;//获取单链表头节点
	//vector<T> adjVertex = this->Adj(vertex);
	vector<T> adjVertex = getBlockPoint(blockId);
	set<T> setVertex(adjVertex.begin(), adjVertex.end());
	while (ptr != nullptr) {
		//进行筛选，只对在该block的点有关的边进行 migrate
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

//template<typename T, typename E>
//vector<int> BlockGraph<T, E>::getDiffBlockTrunkWeight(int b1,int b2) {
//	int x = trunkLine.count({ b1,b2 });
//	int y = trunkLine.count({ b2,b1 });
//	//先判断map中有没有这两个区的trunk weight
//	if (x == 0 && y == 0) {
//		//没有则报错
//		cerr << "error in BlockGraph<T, E>::getDiffBlockTrunkWeight while get:" << b1 << ':' << b2 << endl;
//		return {};
//	}
//	if (x == 0) return trunkLine[{b2, b1}];
//	else return trunkLine[{b1, b2}];
//}

//只能根据给定block生成  如果存在则不能重复生成 ，后续可追加insertTrunkLine接口
//template<typename T, typename E>
//void BlockGraph<T, E>::genTrunkLine(int b1, int b2) {
//	//map<pair<int, int>, vector<int> >  trunkLine;//跨区干线
//	int x = trunkLine.count({ b1,b2 });
//	int y = trunkLine.count({ b2,b1 });
//	if (x != 0 || y != 0) return;
//	//此处开始选择生成两条trunk还是三条trunk
//	int lines;
//	int tmp = rand_n(2);//inline function
//	if (tmp == 0) lines = 2;
//	else lines = 3;
//	//先在trunkline中放入空vector
//	trunkLine[pair<int, int>(b1, b2)] = {};
//	//2倍trunkPreWeight
//	int weight = 2 * (rand_n(trunkPreWeight) + trunkPreWeight);
//	while (lines--) {
//		//注：此处无论是否写pair<int, int>，在insert时候都会将b1，b2强转为int型
//		//pair<int, int>(b1, b2)
//		trunkLine[{b1, b2}].emplace_back(weight);
//		weight /= 2;
//	}
//}

//根据给定的b1和b2，以及b1的边界点vertex，找到通往b2的trunkline
//返回参数first为目标点，second为权重

template<typename T, typename E>
bool BlockGraph<T, E>::handleTrunkLine(ifstream& fin) {
	int num;
	fin >> num;
	while (num--) {
		int x, y, weight;
		fin >> x >> y ;
		int bx = getBlockid(x);
		int by = getBlockid(y);
		int xc = trunkLine.count({ bx,by });
		int yc = trunkLine.count({ by,bx });
		if (yc == 0 && xc == 0) trunkLine.emplace(pair<int,int>{ bx,by }, vector<pair<T, E> >{ { x, y }});
		else {
			if (yc == 0) trunkLine[{bx, by}].emplace_back(pair<int,int>{ x,y });
			else trunkLine[{by, bx}].emplace_back(pair<int, int>{ x,y });
		}
	}
	if (!(fin >> num)) return true;
	else return false;
}
template<typename T, typename E>
pair<T, E>  BlockGraph<T, E>::getBlockTrunkLine(int b1, int b2, T vertex) {
	int x = trunkLine.count({ b1,b2 });
	int y = trunkLine.count({ b2,b1 });
	//先判断map中有没有这两个区的trunk weight
	if (x == 0 && y == 0) {
		//没有则报错
		cerr << "error in BlockGraph<T, E>::getDiffBlockTrunkWeight while get:" << b1 << ':' << b2 << endl;
		return {};
	}
	if (x == 0) {
		for (pair<T, E> s : trunkLine[{b2, b1}]) {
			if (s.first == vertex) {
				return s;
			}
		}
	}
	else if (y == 0) {
		for (pair<T, E> s : trunkLine[{b1, b2}]) {
			if (s.first == vertex) {
				return s;
			}
		}
	}
	return {};
}
//template<typename T, typename E>
//int BlockGraph<T, E>::getNext(int cur,int des) {
//	if (cur == des) {
//		cout << "already arrive at destination" << endl;
//		return -1;
//	}
//	//如果和des刚好同行或同列就不用切换到别的行或列
//	if (des % row == cur % row) {
//		return des > cur ? cur + row : cur - row;
//	}
//	if (des % col == cur % col) {
//		return des > cur ? cur + col : cur - col;
//	}
//
//}

#endif 