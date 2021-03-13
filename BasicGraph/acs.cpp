#include"acs.h"

//�ܶȼ��� num��ʾ��Ҫ���ӵ���Ⱥ����  numΪ-1ʱ ��ʾ�����߳�����·��
void density(myType a, myType b, int num = 1) {
	//vector<int>::iterator it;
	//it = find(sites.begin(), sites.end(), a);
	//if (it == sites.end()) {
	//	cerr << "density ����ʧ��" << endl;
	//	return;
	//}
	int pos = get_pos(a);
	for (auto& s : density_mtx[pos]) {
		if (s.first == b) {
			s.second.second += num;
			return;
		}
	}
	cerr << "density error" << endl;
}

bool isDensityOk(myType a, myType b) {
	int pos = get_pos(a);
	for (auto& s : density_mtx[pos]) {
		if (s.first == b) {
			if (s.second.second >= s.second.first)
				return false;
			else return true;
		}
	}
	cerr << "isDensityOk error:" << a << "," << b << endl;
	return false;
}

void updateHeur(myType a, myType b) {
	int pos = get_pos(a);
	for (int i = 0; i < heur_mtx.at(pos).size(); ++i) {
		if (heur_mtx.at(pos).at(i).first == b) {
			if (density_mtx.at(pos).at(i).second.second == 0)
				heur_mtx.at(pos).at(i).second = 1 / g.getweight(a, b);
			else
				heur_mtx.at(pos).at(i).second = 1 / (g.getweight(a, b) * density_mtx.at(pos).at(i).second.second);
			return;
		}
	}
	cerr << "updateHeur error : " << a << b << endl;
}

void updateTrans(bool tag) {
	if (tag == 0) {//tag==0 ��ʾΪ����
		vector<pair<myType, float> > tmp;
		pair<myType, float> p;
		for (auto s : pher_mtx) {
			//ͳ��ÿһ����Ϣ���ܺ� �Ϸ��� ����
			float sum = 0;
			for (auto j : s) {
				sum += j.second;
			}
			for (auto j : s) {
				p.first = j.first;
				p.second = j.second / sum;
				tmp.push_back(p);
			}
			trans_mtx.push_back(tmp);
			tmp.clear();
		}
	}
	else {//���ǵ�һ�� ����pushback����
		//int cnt = 0;
		//for (auto &s : pher_mtx) {
		//	//ͳ��ÿһ����Ϣ���ܺ� �Ϸ��� ����
		//	float sum = 0;
		//	for (auto j : s) {
		//		sum += j.second;
		//	}
		//	for (int i = 0; i < s.size(); ++i) {
		//		trans_mtx[cnt][i].second = s[i].second / sum;
		//	}
		//	cnt++;
		//}
		for (auto s : sites) {
			updateNodeTrans(s);
		}
	}
}
void updateNodeTrans(myType a) {
	int pos = get_pos(a);
	float sum = 0;
	int tag;
	for (int i = 0; i < trans_mtx.at(pos).size(); ++i) {
		sum += pher_mtx.at(pos).at(i).second * pow(heur_mtx.at(pos).at(i).second, beta);
	}
	for (int j = 0; j < trans_mtx.at(pos).size(); ++j) {
		trans_mtx.at(pos).at(j).second =
			pher_mtx.at(pos).at(j).second * pow(heur_mtx.at(pos).at(j).second, beta) / sum;
	}
	return;
}
//��ʼ��
void init() {
	int k, site;
	cout << "Please input safe sites'num" << endl;
	cin >> k;
	while (k--) {
		cin >> site;
		safe_site.push_back(site);
	}
	//��ȡȫ���ڵ� 
	sites = g.getAllvertex();

	history_best.first = -1;
	//��ʼ����Ϣ�ؾ���
	vector<myType> v;
	pair<myType, float> p;
	vector<pair<myType, float> > tmp;
	vector<pair<myType, pair<int, int> > > den_tmp;
	pair<myType, pair<int, int> > den_p_tmp;
	pair<int, int> den_p_p_tmp;

	for (auto s : sites) {
		v = g.Adj(s);
		for (auto j : v) {
			p.first = j;
			p.second = pher_start;//��Ϣ�س�ʼֵ pher_start
			tmp.push_back(p);
			//�ܶȳ�ʼ��

			den_p_p_tmp.first = cap;
			den_p_p_tmp.second = 0;
			den_p_tmp.first = j;//���
			den_p_tmp.second = den_p_p_tmp;
			den_tmp.push_back(den_p_tmp);
		}
		pher_mtx.push_back(tmp);
		density_mtx.push_back(den_tmp);
		den_tmp.clear();
		tmp.clear();
		v.clear();
	}
	//flow_mtx = pher_mtx;//��ʼ ������� flow
	updateTrans(0);
	//��ʼ���ܶȾ���

	return;

}
void new_init() {
	int site;
	//��ȡȫ���ڵ� 
	sites = g.getAllvertex();
	float choice;
	//Ŀ�Ľڵ㼯�� safe_site
	for (int i = 0; i < m; ++i) {
		do {
			choice = rand_n(sites.size());
		} while (sites[choice] == 48);// 521
		safe_site.push_back(sites[choice]);
	}
	cout << "safe_sites finish with size:" << safe_site.size() << endl;
	// ���ѡ��Դ�ڵ�

	//for (int i = 0; i < m; ++i) {
	//	do {
	//		choice = rand_n(sites.size());
	//	} while (sites[choice] == safe_site[i]);// 521
	//	//safe_site.push_back(sites[choice]);
	//	source[i] = sites[choice];
	//}
	history_best.first = -1;
	//��ʼ����Ϣ�ؾ���
	vector<myType> v;
	pair<myType, float> p;
	vector<pair<myType, float> > tmp;
	vector<pair<myType, float> > tmp_heur;
	vector<pair<myType, pair<int, int> > > den_tmp;
	pair<myType, pair<int, int> > den_p_tmp;
	pair<int, int> den_p_p_tmp;
	float weight_road;
	for (auto s : sites) {
		v = g.Adj(s);
		for (auto j : v) {
			p.first = j;
			p.second = pher_start;//��Ϣ�س�ʼֵ pher_start
			tmp.push_back(p);
			//����ʽ��ʼ��
			weight_road = g.getweight(s, j);
			p.second = 1 / weight_road;
			tmp_heur.push_back(p);
			//�ܶȳ�ʼ��
			den_p_p_tmp.first = cap;
			den_p_p_tmp.second = 0;
			den_p_tmp.first = j;//���
			den_p_tmp.second = den_p_p_tmp;
			den_tmp.push_back(den_p_tmp);
		}
		heur_mtx.push_back(tmp_heur);
		pher_mtx.push_back(tmp);
		density_mtx.push_back(den_tmp);
		tmp_heur.clear();
		den_tmp.clear();
		tmp.clear();
		v.clear();
	}
	//flow_mtx = pher_mtx;//��ʼ ������� flow
	updateTrans(0);
	//��ʼ���ܶȾ���

	return;
}

//��Ӧ�ȼ���  ֻ����·������
vector<float> fitCompute() {
	vector<float> fittness;
	float sum;
	for (int i = 0; i < m; ++i) {
		float sum = 0.0;
		for (vector<int>::iterator it = ant_solution[i].begin(); it != ant_solution[i].end() - 1; ++it) {
			sum += g.getweight(*it, *(it + 1));
		}
		fittness.push_back(sum);
	}
	return fittness;
}

//����till now best solution
void updateHistoryBeSou(float fittness, int serial) {
	if (history_best.first != -1) {
		if (history_best.first > fittness) {
			history_best.first = fittness;
			history_best.second = ant_solution[serial];
		}
	}
	else {
		history_best.first = fittness;
		history_best.second = ant_solution[serial];
	}
}

//����ÿ��flow  flow_mtx
/******** flow �� density ����exchange **********/
void flowCompute() {
	// ���� flow_mtx
	flow_mtx.clear();
	flow_mtx.resize(sites.size());
	unordered_map<myType, float> tmp;
	for (auto s : ant_solution) {
		for (int j = 0; j < s.size() - 1; ++j) {
			if (flow_mtx[get_pos(s[j])].count(s[j + 1]) == 0)
				flow_mtx[get_pos(s[j])][s[j + 1]] = 1;
			else
				flow_mtx[get_pos(s[j])][s[j + 1]] += 1;
		}
	}


}

void updateGlobalPher() {
	vector<float> v{ fit_mtx };
	sort(v.begin(), v.end());
	//����flow ÿ��ֻ����һ��
	cur_global_route.clear();
	flowCompute();
	int dis;
	vector<float>::iterator it;
	vector<myType>::iterator it_t;
	const float weight_pher = 5 * pow(10, 3);
	for (int i = 0; i < w; ++i) {// �ҵ�fittness ǰ���Ľ�
		it = find(fit_mtx.begin(), fit_mtx.end(), v[i]);
		dis = distance(fit_mtx.begin(), it);// ������i�Ľ� ��fit_mtx�еı��
		if (i == 0) {
			updateHistoryBeSou(v[i], dis);//���� ����Ϊֹ ��õ� solution,ֻ���뱾�����Ž�Ƚ�
		}
		for (int j = 0; j < ant_solution[dis].size() - 1; ++j) {
			if (cur_global_route.count({ ant_solution[dis].at(j),ant_solution[dis].at(j + 1) }))
				continue;
			it_t = find(sites.begin(), sites.end(), ant_solution[dis].at(j));
			int dist = distance(sites.begin(), it_t);//�ҵ��ڵ���
			for (auto& s : pher_mtx[dist]) {
				if (s.first == ant_solution[dis].at(j + 1)) {// Ҫ����·���� Ŀ�ĵؽڵ�
					cur_global_route.insert({ ant_solution[dis].at(j),ant_solution[dis].at(j + 1) });
					s.second = s.second * (1 - rho) + rho * ((w - 1 - i) * flow_mtx[dist].at(s.first) / (m * fit_mtx[dis] * weight_pher));
					break;
				}
			}
		}
	}
	// ��ʷ���Ÿ���
	for (int j = 0; j < history_best.second.size() - 1; ++j) {
		it_t = find(sites.begin(), sites.end(), history_best.second[j]);
		dis = distance(sites.begin(), it_t);//
		for (auto& s : pher_mtx[dis]) {
			if (s.first == history_best.second.at(j + 1)) { // Ҫ����·���� Ŀ�ĵؽڵ�
				s.second = s.second * (1 - rho) + rho * (w * flow_mtx[dis].at(s.first) / (m * history_best.first));
				return;
			}
		}
	}
}

void updateLocalPher(myType a, myType b) {
	//vector<myType>::iterator it;
	//it = find(sites.begin(), sites.end(), a);
	int pos = get_pos(a);
	for (auto& s : pher_mtx[pos]) {
		if (s.first == b) {
			s.second = s.second * (1 - xik) + xik * pher_start;// ����Ϣ�ؽ��е���
			return;
		}
	}
	cerr << "updateLocalPher error" << endl;
}

//��a->b ���ٶ� T ʱ��
float velocityCompute(myType a, myType b) {
	//vector<myType>::iterator it;
	//it = find(sites.begin(), sites.end(), a);
	//if (it == sites.end()) {
	//	cerr << "velocity ����ʧ��" << endl;
	//	return -1;
	//}
	int pos = get_pos(a);
	for (auto s : density_mtx[pos]) {
		if (s.first == b) {
			float weight = g.getweight(a, b);
			return v_start * exp(1 / static_cast<float>(s.second.second) * weight * (float)0.05 * (float)traffic_time);//���㹫ʽ ����ʹ��static_castת�����߲�ת��
		}
	}
	cerr << "velocityCompute error" << endl;
	return -1;
}

//��nyc bicycle dataset ��ȡ���� ����ͼ
void readFile(string direction) {
	const int sites = 600;
	map<pair<int, int>, int > p;
	map<pair<int, int>, int> site_map;//��ϣ�� �����
	ifstream fin(direction);
	string line;
	vector<vector<int> > Intact(sites, vector<int>(sites, 0));//sites  ��ʼ��
	const int ReadLine = 10000;
	for (int i = 0; i < ReadLine; ++i) {
		getline(fin, line);//����һ������
		//cout << "ԭʼ�ַ�����" << line << endl;
		istringstream sin(line);
		vector<int> res;
		string tmp;
		int t;
		for (int i = 0; i < 3; ++i) //���ַ�����sin�е��ַ����뵽field�ַ����У��Զ���Ϊ�ָ���
		{
			getline(sin, tmp, ',');//��ȡ����
			t = stoi(tmp);
			res.push_back(t);
		}
		if (site_map.count({ res[1],res[2] }) == 0) site_map[{res[1], res[2]}] = 1; //����
		else site_map.at({ res[1],res[2] })++;
		if (res[1] <= sites && res[2] < sites) {
			Intact[res[1]][res[2]] = res[0];
		}

		//cout << "����֮����ַ�����" << res[0] << ',' << res[1] << ',' << res[2] << endl;
		res.clear();
	}
	int cnt = 0, points = 0;
	int tmp;
	struct tmp1 //��д�º���
	{
		bool operator() (vector<int> a, vector<int> b)
		{
			return a[1] < b[1]; //�󶥶�
		}
	};
	priority_queue<vector<int>, vector<vector<int> >, tmp1>  min4;//�󶥶� �洢�ĸ���ǰ��Сֵ
	FILE* stream1;
	freopen_s(&stream1, "sites.txt", "w", stdout);//����ض���
	cout << endl;
	for (int i = 0; i < sites; ++i) {
		bool tag = false;
		for (int j = i + 1; j < sites; ++j) {
			int number = 0;
			if (Intact[i][j] || Intact[j][i]) {
				if (!tag) {
					points++, tag = !tag;
				}
				if (site_map.count({ i,j }) != 0) number += site_map[{i, j}];
				if (site_map.count({ j,i }) != 0) number += site_map[{j, i}];
				if (number == 0) cerr << "line72 number is 0" << endl;
				tmp = Intact[i][j] + Intact[j][i] / number;
				if (min4.size() < 3) min4.push({ j,tmp });//if (min4.size() < 2) min4.push({ j,tmp });
				else if (min4.top()[1] > tmp) {
					min4.pop();
					min4.push({ j,tmp });
				}
			}// end if
		}// end j 
		while (!min4.empty())
		{
			cnt++;
			cout << i << " " << min4.top()[0] << " " << (float)min4.top()[1] / (float)50 << endl;
			min4.pop();
		}
	}// end i
	freopen_s(&stream1, "CON", "w", stdout);//����ض���� ����̨
	ofstream fout("sites.txt", ios::out || ios::app);
	if (!fout.is_open()) cerr << "Can't open file" << endl;
	long location = 0;
	fout.seekp(location);
	//cout << cnt << endl << points << endl;
	fout << points << " " << cnt << " " << 1 << " " << 1 << " " << 2;
	fout.close();

	cout << "write finish" << "\a" << endl;
	return;
}

//v��ʾԴ�ڵ�,p��ʾv������������·������   
//resΪ��ά����  res[i][0]��ʾ������ʼ�ڵ� res[i][1]��ʾ�����յ�
bool graphPreprocess(int v, int num, vector<vector<myType> >& p, vector<vector<myType> >& res) {
	for (int i = 0; i < num; ++i) {
		vector<int> tmb(2, -1);
		int n = p[i].size();
		if (n > 4) {
			float tmp, tmp_t;
			do {
				tmp = rand_n(n);
			} while (tmp == 0 || tmp == n - 1);
			tmb[0] = p[i][tmp];
			int k = n - tmp - 1;
			tmp_t = rand_n(k);
			tmb[1] = p[i][tmp + tmp_t + 1];
		}
		res.push_back(tmb);
	}
	return true;
}


//���а汾  ����·��*************->
void routeConstricution() {
	if (G == 0) return;
	traffic_time = 1;
	vector<bool>::iterator it = find(ant_dest.begin(), ant_dest.end(), false);
	if (it == ant_dest.end()) return;//�Ƿ�ȫ�����յ�
	ant_solution.clear();
	ant_solution.resize(m);
	myType current_pos;//��ǰ�ڵ�
	myType before_pos;
	//vector<int> v;//�洢ÿֻ����·��
	//for (int i = 0; i < m; ++i) {//i��ʾ���ϱ��
	//	if (ant_dest[i] != true) {
	//		int current_pos = source[i];//��ǰ�ڵ�
	//		v.push_back(current_pos);
	//		while (!isSafe(current_pos)) {
	//			float tmp = rand_p();//���̶�
	//			int choice = 0;//Ҫѡ��Ľڵ�
	//			float sump = 0.0;
	//			while (sump <= tmp) {
	//				sump += trans_mtx[current_pos][choice++].second;
	//			}//����0ʱ��Ĭ��ѡ���һ�ڵ�
	//			current_pos = trans_mtx[current_pos][--choice].first;//ѡ��ڵ�
	//			v.push_back(current_pos);
	//		}
	//		ant_solution.push_back(v);//�洢ÿֻ���ϵ�·��
	//		ant_dest[i] = true;
	//		v.clear();
	//	}
	//	else {
	//		continue;
	//	}
	//}
	fit_mtx.clear();
	fit_mtx.resize(m);
	for (int j = 0; j < m; ++j) {
		current_pos = source[j];//��ǰ�ڵ�
		ant_solution[j].push_back(current_pos);
	}
	for (; find(ant_dest.begin(), ant_dest.end(), false) != ant_dest.end(); ++traffic_time) {//�Ƿ�ȫ�������յ�
		for (int i = 0; i < m; ++i) {
			if (ant_dest[i] != true) {
				current_pos = ant_solution[i].back();
				before_pos = current_pos;
				//���̶�ѡ��ڵ�
				int choice_test = 0;
				do {
					if (choice_test++ > 50) {
						cerr << choice_test << " ::choice_test erro: " << before_pos << "****" << current_pos << endl;
						current_pos = before_pos;
						break;
					}
					current_pos = before_pos;
					float tmp = rand_p();//���̶�
					static int choice = 0;//Ҫѡ��Ľڵ�
					float sump = 0.0;
					if (tmp == 0) {
						current_pos = trans_mtx[get_pos(current_pos)][0].first;
					}
					else {
						while (sump < tmp) {
							sump += trans_mtx[get_pos(current_pos)][choice++].second;
						}//����0ʱ��Ĭ��ѡ���һ�ڵ�
						current_pos = trans_mtx[get_pos(current_pos)][--choice].first;//ѡ��ڵ�
					}
				} while (current_pos == before_pos || !isDensityOk(before_pos, current_pos) || current_pos == target);

				ant_solution[i].push_back(current_pos);

				if (isEnd(current_pos, i)) {
					ant_dest[i] = true;
				}
				else {
					density(before_pos, current_pos);
				}//�����±�  ���һ�λ�δ���� ��ȫ·��֮ǰ  ��ȫ֮·������ӵ����

				if (traffic_time != 1) {
					density(ant_solution[i].at(ant_solution[i].size() - 3), before_pos, -1);//��һ���� �߳�
					updateHeur(ant_solution[i].at(ant_solution[i].size() - 3), before_pos);// ��������ʽ  ��һ�ξ�����·��
					updateNodeTrans(ant_solution[i].at(ant_solution[i].size() - 3));// �������� �ڵ�� ת�Ƹ���
				}

				float vel = velocityCompute(before_pos, current_pos);
				if (before_pos == current_pos) fit_mtx[i] += 1;
				else fit_mtx[i] += g.getweight(before_pos, current_pos) / vel;
				updateLocalPher(before_pos, current_pos);
				updateHeur(before_pos, current_pos);
				updateNodeTrans(before_pos);
				//�ж��Ƿ񵽴ﴫ�͵�
				int cur_tmp_safe = get_pos(safe_site[i]);
				if (current_pos == transmit_p[cur_tmp_safe][0]) {
					auto k = find(short_p[cur_tmp_safe].begin(), short_p[cur_tmp_safe].end(), current_pos);
					int cur_tmp_pos = *(k++ + 1);//������Ϊend��ʱ��������
					while (cur_tmp_pos != transmit_p[cur_tmp_safe][1]) {
						ant_solution[i].push_back(cur_tmp_pos);
						cur_tmp_pos = *(++k);
					}
					ant_solution[i].push_back(transmit_p[cur_tmp_safe][1]);
					//�ж��Ƿ񵽴�safe place
					if (isEnd(transmit_p[cur_tmp_safe][1], i)) {
						ant_dest[i] = true;
					}
				}
			}//end if ant_dest true
			else continue;
		}// end i
		//�������ѡ��Ľڵ�
		//for (int i = 0; i < m; ++i) {
		//	cout << ant_solution[i].back() << endl;
		//}
	}
	ant_dest.clear();
	ant_dest.resize(m);
	G--;//��������
	return;
}

//���а汾����·��
void routeConstructSerial() {
	if (G == 0) return;
	ant_solution.clear();
	ant_solution.resize(m);
	myType current_pos;//��ǰ�ڵ�
	myType before_pos;
	for (int i = 0; i < m; ++i) {
		while (ant_dest[i] != true) {//��ǰ�����Ƿ񵽴��յ�
			current_pos = ant_solution[i].back();
			before_pos = current_pos;
			int pre_before_pos;
			//��һ���ߵ���ʼ�ڵ㣬������һ����ΪA-B���˴�pre_before_posΪA��
			if (ant_solution[i].size() > 1) pre_before_pos = ant_solution[i][ant_solution[i].size() - 2];
			else pre_before_pos = -1;

			current_pos = getNextWaySerial(0, 0, i, before_pos, pre_before_pos);
			ant_solution[i].push_back(current_pos);

			density(before_pos, current_pos);//�����±�  ���һ�λ�δ���� ��ȫ·��֮ǰ  ��ȫ֮·������ӵ����

			float vel = velocityCompute(before_pos, current_pos);
			if (before_pos == current_pos) fit_mtx[i] += 1;//�˴�Ϊԭ��ͣ��
			else fit_mtx[i] += g.getweight(before_pos, current_pos) / vel;
			updateLocalPher(before_pos, current_pos);
			updateHeur(before_pos, current_pos);//����
			updateNodeTrans(before_pos);//��Ϊһ���ߵ����������仯������������ڱ߶���仯
			//�ж��Ƿ񵽴�safe place
			if (isEnd(current_pos, i)) {
				ant_dest[i] = true;
			}

		}//end if ant_dest true
	}// end i
	//�������ѡ��Ľڵ�
	//for (int i = 0; i < m; ++i) {
	//	cout << ant_solution[i].back() << endl;
	//}
}


//��ȡ����block֮���trunk��֮����������̶�ѡ������block
vector<int> getBlockTrunk(int b1, int b2) {
	return {};
}

//�ɰ汾������ȡ��һ�����ڵ�block������ֵΪͼ��ָ�� UndiGraph<myType, float>*�����Ը������̶�ѡ��
//�����б�b1Ϊ��ǰblock��b2Ϊ��ǰ����������Ŀ��block
int getNextBlock(int b1, int b2) {
	return 0;
}

//2021.1.6 ����
//��ʱ������ʱ���
void routeConstructionByTimestamp() {
	if (G == 0) return;
	//traffic_time = 1;
	//vector<bool>::iterator it = find(ant_dest.begin(), ant_dest.end(), false);
	//if (it == ant_dest.end()) return;//�Ƿ�ȫ�����յ�
	ant_solution.clear();
	ant_solution.resize(m);
	myType current_pos;//��ǰ�ڵ�
	myType before_pos;
	//ÿһ��ѭ��timesta++��
	while (find(ant_dest.begin(), ant_dest.end(), false) != ant_dest.end()) {//�ж��Ƿ�ȫ���յ�
		timestamp++;//ʱ�������
		for (int i = 0; i < m; ++i) {
			if (ant_dest[i] == true) continue;
			current_pos = ant_solution[i].back();
			before_pos = current_pos;
			if (timestamp == 1) {//�ж��Ƿ�Ϊ�߽����

			}
		}
	}
}

//������ʱ���
myType getNextWaySerial(myType orgn, myType dstn, int antId, myType cur, myType pre) {
	//����curPosition�����Խ���߽磬�������һ��·�������������ı߽磬����п���ѡ��
	if (dstn == curOD[antId].second) {
		//����,Ŀ��trunkline��������ʼʱ�趨�������޸�
		//��ʱ������trunkline��ӵ���̶�
		int curblock_id = g.getBlockid(dstn);
		int nexblock_id = getNextBlock(curblock_id, block_dest[antId]);
		//auto curTrunk = selctTrunkLine(curblock_id, nexblock_id, dstn);

		UpdateCurOd(antId,cur,dstn);
		return ;//�˴�������
	}
	else {//����Ҫ����
		auto curRoute = selectNextRoute(pre, cur);
		//ant_solution[antId].push_back(curRoute.first);
		//density(dstn, curRoute.first);//��ǰ������  ������������
		return curRoute.first;
	}
}

//����ֵΪĿǰ�Ľڵ㣬�ڽ��մ��жϣ����id��֮ǰ��ͬ����push��route�����; ͬʱ��Ҫ����Ļ��У��������·����ʣ��ʱ��ֱ�Ӷ���
//orgnΪ��ǰ·�ε�Դ�㣬dstnΪ��ǰ·�ε��յ㣬antIdΪĿǰ���ϵ�id��timeĿǰ����ʹ��
myType getNextWayByTimestamp(myType orgn, myType dstn, int antId, int timeStap, double time = 1) {
	//����curPosition�����Խ���߽磬�������һ��·�������������ı߽磬����п���ѡ��
	int current = curPosition[antId].first + velMatrix[antId];
	if (curPosition[antId].second <= current) {//ѡ����һ��·��ʲô��Ȼ���޸�curPosition��ǰֵΪ0
		curPosition[antId].first = 0;
		if (dstn == curOD[antId].second) {
			//����,Ŀ��trunkline��������ʼʱ�趨�������޸�
			//��ʱ������trunkline��ӵ���̶�
			int curblock_id = g.getBlockid(dstn);
			int nexblock_id = getNextBlock(curblock_id, block_dest[antId]);
			auto curTrunk = selctTrunkLine(curblock_id, nexblock_id, dstn);
			//�����ĳ���������
		}
		else {//����Ҫ����
			auto curRoute = selectNextRoute(orgn, dstn);
			ant_solution[antId].push_back(curRoute.first);
			curPosition[antId].second = curRoute.second;//����ǰ·�ε�Ȩ�أ�����
			curPosition[antId].first = 0;
			/***********************/
			//�˴�����Ϊant_solution��Խ�����⣬������Ҫ�жϣ���˼��
			if (timeStap != 1) {
				density(ant_solution[antId].at(ant_solution[antId].size() - 3), dstn, -1);//��һ���� �߳�,-1��ʾ�߳�
				density(dstn, curRoute.first);//��ǰ������
				//��������ʽ��������Ƶ�ÿ��timestamp
				updateHeur(ant_solution[antId].at(ant_solution[antId].size() - 3), dstn);// ��������ʽ  ��һ�ξ�����·��
				updateNodeTrans(ant_solution[antId].at(ant_solution[antId].size() - 3));// �������� �ڵ�� ת�Ƹ���
			}
			else {
				density(dstn, curRoute.first);//��ǰ������
			}
		}
	}
	else {//���û���ߵ���ǰ·�εľ�ͷ���Ծɷ��ص�ǰ·�ε��յ�
		curPosition[antId].first = current;
		return dstn;
	}
}


//��Ҫ�л�block������£��л����
//����trunkline��Ŀ�Ľڵ㣬firstΪ�ڵ㣬secondΪȨ��
//����˵����b1Ϊ��ǰblock��b2ΪĿ��block,vertexΪĿǰ����b1�ı߽�ڵ�
//Ŀǰ������Ϊ��������֮��
pair<myType, myType> selctTrunkLine(int b1, int b2, int vertex) {
	//g.getTrunkLine();
	return pair<myType, myType>(0, 0);
}


//����Ҫ�л�block������£��������
//Ŀǰ�����̶�ѡ�񷽷�
//����trunkline��Ŀ�Ľڵ㣬firstΪ�ڵ㣬secondΪȨ��
pair<myType, myType> selectNextRoute(int before_pos, int current_pos) {
	int tmp_pos = current_pos;
	//���̶�ѡ��ڵ�
	int choice_test = 0;
	do {
		if (choice_test++ > 50) {
			cerr << choice_test << " ::choice_test erro: " << before_pos << "****" << current_pos << endl;
			current_pos = before_pos;
			break;
		}
		//current_pos = before_pos;
		//need to be test*************************   choice eps tmp
		float tmp = rand_p();//���̶�
		static int choice = 0;//Ҫѡ��Ľڵ�
		float sump = 0.0;
		if (tmp < eps) {//ͨ��eps���ж�float���Ƿ�������0
			tmp_pos = trans_mtx[get_pos(current_pos)][choice].first;//����0ʱ��Ĭ��ѡ���0�ڵ�
		}
		else {
			while (sump < tmp) {
				sump += trans_mtx[get_pos(current_pos)][choice++].second;
			}
			tmp_pos = trans_mtx[get_pos(current_pos)][--choice].first;//ѡ��ڵ�
		}
	} while (tmp_pos == before_pos || !isDensityOk(current_pos, tmp_pos) || tmp_pos == target);
	int cur_weight = g.getweight(current_pos, tmp_pos);
	return pair<myType, myType>{tmp_pos, cur_weight};
}

//�˴�cur_vertex  Ϊ����֮ǰ��һ������id  �൱�ڱ�������һ�����ĳ���  des_vertexΪ�յ�block
void UpdateCurOd(int antid, myType cur_vertex, myType des_vertex) {
	int cur_id = g.getBlockid(cur_vertex);
	int des_id = g.getBlockid(des_vertex);
	if (cur_id == des_id) {
		curOD[antid] = pair<int, int>{ cur_vertex ,des_vertex };
		return;
	}
	auto res = g.getBorderBlock(cur_id, des_id);
	//�������block���յ� ��ǰ����
	for (auto s : res) {
		if (s == des_id) {
			curOD[antid] = pair<int, int>{ cur_id ,s };
			return;
		}
	}
	int size = res.size();
	//�˴�Ϊ�漴ѡ����һ���� ************* ���Ľ�
	int ret = rand_n(size);
	int nex_id = res[ret];
	curOD[antid] = pair<int, int>{ cur_id ,nex_id };
	return;
}