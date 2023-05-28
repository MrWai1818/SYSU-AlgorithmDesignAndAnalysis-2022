#ifndef UTILS_H
#define UTILS_H
#include <iostream>
#include <vector>
#include <cstring>

using namespace std;

int n = 0, m = 0;   //输入矩阵的行列数
vector<vector<char>> input; //输入矩阵
vector<pair<int, int>> W;   //仓库点的信息，pair用于记录其在矩阵的位置
vector<pair<int, int>> G;   //货物点的信息，pair用于记录其在矩阵的位置

//获取输入矩阵
void GetInput(int n, int m){ 
    input.resize(n);
    for (int i = 0; i < n; i++) //输入矩阵
    {   
        input[i].resize(m);
        for (int j = 0; j < m; j++)
        {
            cin >> input[i][j];
        } 
    }
}
//求两个节点的曼哈顿距离
int distance(pair<int, int> x, pair<int, int> y)
{
	return abs(x.first - y.first) + abs(x.second - y.second);
}
//提取输入矩阵的信息
void ExtractMatrix(){
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {   //将货物和仓库的位置信息存入数组
            if (input[i][j] == 'W') W.push_back(make_pair(i, j));
            else if (input[i][j] == 'G') G.push_back(make_pair(i, j));
        }
	}
}
//清空
void CleanUp(){
    for (int i = 0; i < input.size(); i++)
    {
        input[i].clear();
    }
    input.clear();
    G.clear();
    W.clear();
    n = m = 0;
}
#endif