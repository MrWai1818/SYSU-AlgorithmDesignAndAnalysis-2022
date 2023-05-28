#include <iostream>
#include <vector>
#include <fstream>
#include <queue>
#include <algorithm>
#include <string.h>

using namespace std;
//读入文件
vector<vector<int>> file_input(){
	ifstream input("input.txt");	//开文件
	if(!input.is_open())
		std::cerr << "cannot open the file";	//排错
	vector<vector<int>> t;		//读取m，n存入二维数组，因为输入可能有多组
	while (!input.eof())		//读完文件
	{
		vector<int> temp = {0,0};
		input >> temp[0] >> temp[1];	//读入转化为int并存入vector
		t.push_back(temp);
	}
	input.close();		//读完
	return t;
}
//写结果
void file_output(int r, vector<vector<int>> &result){
	ofstream output("output.txt", ios::app);		//开文件，每有一个结果就写一次
	if(!output.is_open())
		std::cerr << "cannot open the file";
	//写入哨兵数量和哨兵分布矩阵
	output << "The number of guards : " << r <<endl;	
	for (int i = 0; i < result.size(); i++)
	{
		for (int j = 0; j < result[i].size(); j++)
		{
			output<<result[i][j]<<" ";
		}
		output<<endl;
	}
	output<<"------------"<<endl;
	output.close();		//写完
	return;
}

struct Node		//以每一个陈列室为节点
{
	vector<vector<int>> place_of_guards;	//所有陈列室哨兵的分布
	vector<vector<int>> room_in_sight;		//所有陈列室已经被监视的房间
	int i = 0, j = 0;					//当前陈列室坐标
	int numbers_of_guards = 0, numbers_of_sights = 0;	//当前已有哨兵数(代价)，当前已监视房间数
};

struct cmp 		//重载优先队列的比较函数
{
    bool operator() (Node x, Node y)
    {
        return x.numbers_of_guards > y.numbers_of_guards; 	//哨兵数目(代价少)的节点优先
    }
};

// bool judge(Node x, Node y)		//重载sort()比较规则，监视房间数多的节点优先
// {
//     return x.numbers_of_sights < y.numbers_of_sights;
// }
int sfg[5][2] = {{0, 0}, {0, 1}, {0, -1}, {1, 0}, {-1, 0}};		//哨兵的监视范围
priority_queue<Node, vector<Node>, cmp> tree;		//使用优先队列存节点
// vector<Node> tree;		//使用vector存

void set_guard(Node put, int x, int y, int n, int m){	//放置哨兵
	if (x >= n || y >= m)	//边界检查，不能在范围外放哨兵
	{
		return;
	}
	Node temp;			//复制当前节点，用于生成下一个节点
	temp.i = put.i;
	temp.j = put.j;
	temp.numbers_of_sights = put.numbers_of_sights;
	//复制当前节点的哨兵排布
	temp.room_in_sight.resize(n,vector<int>(m,0));
	for (int l = 0; l < n; l++)
		temp.room_in_sight[l].assign(put.room_in_sight[l].begin(), put.room_in_sight[l].end());
	//复制当前节点已监视房间排布
	temp.place_of_guards.resize(n,vector<int>(m,0));
	for (int l = 0; l < n; l++)
		temp.place_of_guards[l].assign(put.place_of_guards[l].begin(),put.place_of_guards[l].end());

	temp.place_of_guards[x][y] = 1;		//在输入位置放哨兵
	temp.numbers_of_guards = put.numbers_of_guards + 1;		//哨兵数+1
	for (int k = 0; k < 5; k++)		//更新新放置哨兵后的监视范围
	{
		int near_x = x + sfg[k][0];
		int near_y = y + sfg[k][1];	
		if (near_x < 0 || near_y < 0 || near_x >= n || near_y >= m)		//更新范围边界检查
			continue;
		temp.room_in_sight[near_x][near_y]++;		
		if (temp.room_in_sight[near_x][near_y] == 1)	//为1代表，房间首次被监视
			temp.numbers_of_sights++;		//增加已监视房间数
	}
	
    while(temp.i < n && temp.room_in_sight[temp.i][temp.j]) //当前节点位置坐标更新，更新为下一个节点位置
	{
		temp.j++;
		if(temp.j >= m)
		{
			temp.i++;
			temp.j = 0; 
		}
	}
	// tree.push_back(temp);
	// sort(tree.begin(), tree.end(), judge);
	tree.push(temp);		//加入树中
	return ;
};

void branch_and_bound(int n, int m){		//分支界限法
	vector<vector<int>> result(n,vector<int>(m,0)); 	//结果的哨兵布局
	int numbers_of_guards = 9999;	
	Node node;			//根节点初始化
	node.place_of_guards.resize(n,vector<int>(m,0));
	node.room_in_sight.resize(n,vector<int>(m,0));		
	// tree.push_back(node);
	tree.push(node);		//根节点加入

	while (!tree.empty())	//最小损耗优先遍历节点
	{
		// Node now = tree.front();
		Node now = tree.top();	
		// tree.erase(tree.begin());
		tree.pop();
		if (now.numbers_of_guards > numbers_of_guards) continue;	
	
		if ((now.numbers_of_sights < n*m))	//根据界扩展结点，界为已监视房间数目
		{	
			if (now.i < n)				//设置子节点一 
				set_guard(now, now.i+1, now.j, n, m);			
			if(now.room_in_sight[now.i][now.j+1] == 0)//设置子节点二
				set_guard(now, now.i, now.j, n, m);
			if (now.j + 2 < m)	//边界检查
			{
				if(now.j < m && (now.room_in_sight[now.i][now.j+1] == 0 || now.room_in_sight[now.i][now.j+2] == 0))//设置子节点三
					set_guard(now, now.i, now.j+1, n, m);
			}
		}else{	//每个房间都至少有一个哨兵监视
			if (now.numbers_of_guards < numbers_of_guards)		//选最小哨兵数的节点作为结果
			{
				numbers_of_guards = now.numbers_of_guards;
				for (int l = 0; l < n; l++)
					result[l].assign(now.place_of_guards[l].begin(), now.place_of_guards[l].end());
			}
		}
	}	
	if (n == 1 && m == 1)
	{
		numbers_of_guards = 1;
		result[0][0] = 1;
	}
	
	cout<<"The number of guards : "<<numbers_of_guards<<endl;	//屏幕打印  
	for (int r = 0; r < n; r++)
	{
		for (int q = 0; q < m; q++)
		{
			cout<<result[r][q]<<" ";
		}
		cout<<endl;
	}	
	cout<<"------------"<<endl;
	file_output(numbers_of_guards, result);		//写入输出文件
	return;
}

int main(){
	vector<vector<int>> input = file_input();	//读入输入 
	for (int i = 0; i < input.size(); i++)
	{	
		int n = input[i][0], m = input[i][1];
		branch_and_bound(n, m);
	}
	return 0;
}