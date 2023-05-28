#include <iostream>
#include <vector>
#include <math.h>
#include<stdio.h>
using namespace std;

int greedy(vector<int>& put,vector<int>& out, int start,int n){
	int total=0;
	int now=start;		//当前磁头位置
	for (int i = 0; i < n; i++){		//遍历所有请求位置
		int next=0,index=0;			//下一个磁头位置
		int min_delt=999999;		//距离差，为贪心，取最小的为下一个磁头位置
		for (int j = 0; j < put.size(); j++)		//遍历未处理请求位置
		{	
			if (abs(now-put[j])<=min_delt)		//求距离差
			{
				min_delt = abs(now-put[j]);		//更新距离差
				next = put[j];				//更新当前最近磁头位置
				index=j;
			}
		}
		now=next;			//更新最近磁头
		put.erase(put.begin()+index);		//除去已经处理位置
		total+= min_delt;			//磁头移动距离
		out.push_back(now);			//处理序列
	}
	return total;
}

int main(){
	vector<int> request;	 //请求队列
	vector<int> reply;			//处理队列
	int total=0;				//磁头移动总长
	int n,start;				//请求数目、磁头初始位置
	//输入
	cout<<"Input numbers of request : ";		
	cin>>n;						
	cout<<"Input initial position : ";
	cin>>start;
	cout<<"Input the sequence : ";
	for (int i = 0; i < n; i++)
	{
		int temp;
		cin>>temp;
		request.push_back(temp);
	}
	//贪心算法计算
	total = greedy(request,reply,start,n);
	cout<<"Request sequence : ";
	//输出
	for (int i = 0; i < reply.size(); i++)
	{
		cout<<reply[i]<<' ';
	}
	cout<<endl<<"Total movement : "<<total;	
	return 0;
}