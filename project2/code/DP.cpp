#include <iostream>
#include "stdlib.h"
#include <vector>
#include <string>
#include <algorithm>

using namespace std;
vector<string> t;	//标记，存最短路径
int shortest_route_dp(vector<vector<int>> put)
{
	if(put.size()==0||put[0].size()==0)return -1;		
	int row_len = put.size();
	int col_len = put[0].size();
	vector<int> col(col_len);
	vector<vector<int>>dp(row_len,col);		//备忘录，用于存矩阵中起点到各点的最小距离，基于F(i,j)=min{F(i-1,j),F(i,j-1)}+F(i,j)更新
	dp[0][0] = put[0][0];
	for(int i = 1; i < col_len; ++i) {		//第一行和第一列的F(i,j)，最短为直线前进
		dp[0][i] = dp[0][i-1] + put[0][i];	//可以简化为F(i,j)=F(i-1,j)+F(i,j)
    	dp[i][0] = dp[i-1][0] + put[i][0];	//和F(i,j)=F(i,j-1)+F(i,j)
	}
	for(int i = 1; i < row_len; ++i) {		//对于剩下的点进行判断
		for(int j = 1; j < col_len; ++j) {
			if (dp[i-1][j] < dp[i][j-1]) {	//F(i,j)=min{F(i-1,j),F(i,j-1)}+F(i,j)，因为要输出路径，所以使用if而不是min{}
				dp[i][j] = dp[i-1][j] + put[i][j];
				//拼接字符串用于输出
				string temp = "["+to_string(i-1)+",";	
				temp += to_string(j)+"]";
				//如果不在标志vector中，则加入，作为最短路径节点
				vector<string>::iterator ti=find(t.begin(),t.end(),temp);
				if (ti==t.end()){
					t.push_back(temp);	
				}
			}else{
				dp[i][j] = dp[i][j-1] + put[i][j];
				//拼接字符串用于输出
				string temp = "["+to_string(i)+",";	
				temp += to_string(j-1)+"]";
				//如果不在标志vector中，则加入，作为最短路径节点
				vector<string>::iterator ti=find(t.begin(),t.end(),temp);
				if (ti==t.end()){
					t.push_back(temp);	
				}
			}	
		}
	}
	return dp[row_len-1][col_len-1];
}
int main()
{
	vector<vector <int>> input_matrix={{1,3,5,9},{8,1,3,4},{5,0,6,1},{8,8,4,0}};	//输入矩阵
	// vector<vector <int>> input_matrix={{1,1,4,2},{5,1,3,2},{3,2,6,1},{4,4,4,0}};	//输入矩阵
	cout <<"length of the shortest route : "<<shortest_route_dp(input_matrix)<< endl;	
	cout <<"the shortest route : [0,0]->";	
	for (int i = 0; i <t.size()-1 ; i++){
		cout<<t[i]<<"->";
	}
	cout<<"["<<input_matrix[0].size()-1<<","<<input_matrix.size()-1<<"]"<<endl;
	return 0;
}
