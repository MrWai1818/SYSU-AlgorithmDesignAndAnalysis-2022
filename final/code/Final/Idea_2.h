#ifndef IDEA_2_H
#define IDEA_2_H
//思路二：将问题转化为最小费用最大流问题问题，并使用最小费用流的最短路径算法求解
#include "Utils.h"
#include <queue>

//将仓库和货物作为节点，绘制出流量网络
//网络的边
struct edge
{
    int des;     //边的终点
    int capacity;   //边的容量
    int dist;   //距离
    int reverse;    //与该边相反的边
    edge() : des(-1), capacity(0), dist(0), reverse(-1) {}
    edge(int dest, int cap, int dst, int rvrse) : des(dest), capacity(cap), dist(dst), reverse(rvrse) {}
};

vector<edge> Network[120];  //网络,最多100个仓库，怕有意外设120

//网络添加新的从start到des的边
void addEdge(int start, int des, int capacity, int dist){
    Network[start].push_back(edge(des, capacity, dist, Network[des].size()));//起点start加边
    Network[des].push_back(edge(start, 0, -dist, Network[start].size() - 1));//终点des加边
}
//建立网络
void getNetwork(){
    for (int i = 0; i < G.size(); i++)
    {
        addEdge(0, i + 1, 1, 0);    //网络中写入货物节点，与源点相连，原点设为0
        for (int j = 0; j < W.size(); j++)
        {
            int dist = distance(G[i], W[j]);
            addEdge(i + 1, G.size() + j + 1, 1, dist);    //网络中写入仓库节点，与货物相连
        }   
    }
    for (int i = 0; i < W.size(); i++)//仓库节点与汇点相连，汇点设为119
    {
        addEdge(G.size() + i + 1, 119, 1, 0);
    }
}

int dist[120];  // start->des的最短距离
int visit[120];  // i节点是否被访问, 0 <= i < 120
int path[120];  //最短路径经过的节点的前驱节点
int road[120];  //最短路径经过的边

//最短路径算法spfa
int spfa(int start, int des) {  
    //每次找最短路径都要初始化
    for (int i = 0; i < sizeof(dist); i++){
        dist[i] = 65536;
    }
    memset(visit, 0, sizeof(visit));
    memset(path, -1, sizeof(path));
    memset(road, -1, sizeof(road)); 
	queue<int> q;
	q.push(start);  //压入起始节点
	dist[start] = 0;
	visit[start] = 1;
	while(!q.empty()) { 
		int srch = q.front();   //正在被查询的节点
		q.pop();
		visit[srch] = 0;
		for(int i = 0; i < Network[srch].size(); i ++)  //遍历查询节点的邻接结点
        {
            edge & temp = Network[srch][i];
			if ((temp.capacity > 0) && (dist[temp.des] > dist[srch] + temp.dist)) { //dist>0代表不是反向路径，找到更段路径
				dist[temp.des] = dist[srch] + temp.dist;    //更新最短距离
				path[temp.des] = srch;  //更新当前点的前驱节点
                road[temp.des] = i;     //更新当前点到达前驱节点的路径
				if(visit[temp.des]) continue;   //如果已经到终点，结束
				visit[temp.des] = 1;    
				q.push(temp.des);
			}
		}
	}
	return dist[des] != 65536;  //返回最短路径值
}
//求出最小费用最大流
int MCMF(int s, int t) {
	int min_cost = 0, max_flow = 0;
	while(spfa(s, t)) {
		int flow = 65536;   //当前流
        //s经过最短路径，最终到达能t的流量，通过t倒着求回去
		for (int i = t; i != s; i = path[i])
        {
            flow = min(flow, Network[path[i]][road[i]].capacity);
        }
        for (int i = t; i != s; i = path[i])
        {
            min_cost += flow * Network[path[i]][road[i]].dist;  //计算最小开销
            Network[path[i]][road[i]].capacity -= flow;   //遍历辅助网络时路径容量要减去流
            Network[i][Network[path[i]][road[i]].reverse].capacity += flow;     //反向路径要加上流
        }
        max_flow += flow;   //更新最大流
	}
	return min_cost; //返回最小开销
}
//使用思路二求解
void Idea2(){
    cout << "Here is Idea_2 : input \"0 0\" to end " << endl;
    while ((cin >> n >> m) && (n > 0) && (m > 0)) { //多次输入
        if ((n == 0) && (m == 0)){
            cout << "End of Idea_2" << endl <<endl;
            return;
        }
        GetInput(n, m);     //输入
        ExtractMatrix();    //提取输入
        if (W.size() != G.size()){  //排错
            cout << "The number of warehouse and goods is inconsistent, Idea_2 is over" <<endl;
            return;
        }    
        getNetwork();   //绘制带顶点、容量的网络
        cout << "The minimum distance : " << MCMF(0, 119) <<endl;    //源点为0，汇点为119
        for (int i = 0; i < 120; i++)   //清空网络，为新的网络服务
        {
            Network[i].clear();
        }
        CleanUp();
    }
}
#endif