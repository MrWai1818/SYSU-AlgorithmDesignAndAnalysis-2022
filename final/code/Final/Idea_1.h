#ifndef IDEA_1_H
#define IDEA_1_H
//思路一：转化为最小权的二部图匹配问题，再使用能引入新边的匈牙利算法，也就是KM算法求解
#include "Utils.h"

vector<pair<int, int>> Graphic[120];    //二部图,最多100个仓库，怕有意外设120
//绘制赋权完全二部图
void makeGraphic(){
    for (int i = 0; i < G.size(); i++)
    {
        for (int j = 0; j < W.size(); j++) 
        {
            int weight = distance(G[i], W[j]);  //求权  
            Graphic[i].push_back(make_pair(j, -weight)); //i与j的带权非匹配边,求最小权所以为负
        }
    }
}

bool visit_A[120], visit_B[120];    //标记寻求增广路时，A和B的点是否已经被查询
int Match[120];     //匹配结果，Match[A_i]=B_j，Match[B_j]=A_i，标识A_i与B_j已经匹配
int l_A[120], l_B[120];   //A中标号和B中标号，用最小权
int s[120]; //存到s[i]的最小权，扩大便代表松弛，从而可以找到增广交错路径

//同匈牙利算法，给A_t找匹配
bool searchMatch(int t){
    visit_A[t] = 1;     //正在查询A_t
    int temp_weight, temp_index;    //查询与A_t邻接的点的编号和权重
    for (int i = 0; i < Graphic[t].size(); i++) //查询邻接点
    {
        temp_weight = Graphic[t][i].second;
        temp_index = Graphic[t][i].first;
        if (visit_B[temp_index]) continue;  //如果该邻接点已被查询，则跳过
        int temp = l_A[t] + l_B[temp_index] - temp_weight;
        if (temp != 0)  //找不到增广交错路径
        {
            s[temp_index] = min(s[temp_index], temp);  //松弛最小权
        } else {    //temp_index可以作为增广节点
            visit_B[temp_index] = 1;
            if ((Match[temp_index] == -1) || (searchMatch(Match[temp_index]))) //若temp_index未匹配
            {
                Match[temp_index] = t;  //temp_index暂时作为匹配
                return true;
            }
        }
    }
    return false;   
}
//Kuhn-Munkras算法
int KM(){
    //初始化
    memset(Match, -1, sizeof(Match));
    memset(l_B, 0, sizeof(l_B));    
    for (int i = 0; i < G.size(); i++)
    {
        for (int j = 0; j < Graphic[i].size(); j++)
        {
            l_A[i] = max(l_A[i], Graphic[i][j].second); //A标记初始化用权值的最大值
        }
    }
    //不断更新A中点的标记，直到所有点被标记且查询，即找到完美匹配
    for (int i = 0; i < G.size(); i++)
    {
        for (int i = 0; i < sizeof(s); i++)
        {
            s[i] = 65536;   //初始化s
        }
        while (1)
        {
            memset(visit_A, 0, sizeof(visit_A));    //初始化查询数组
            memset(visit_B, 0, sizeof(visit_B));
            if (searchMatch(i)) break;  //A_i在相等子图找到了匹配,继续为下一个点找匹配 
            int temp = 65536;
            for (int j = 0; j < W.size(); j++)  //更新最小权
            {
                if (visit_B[j] == 0) temp = min(temp, s[j]);
            }
            if (temp == 65536) return -1;   //找不到完美匹配
            for (int j = 0; j < G.size(); j++)
            {   //所有已搜索过的A减少相同的量，减少权重
                //从而增加未匹配A的选择可能
                if (visit_A[j]) l_A[j] -= temp;
            }
            for (int j = 0; j < W.size(); j++)
            {
                //权重和标记内容保持一直，因为标记用的最小权
                if (visit_B[j]) l_B[j] += temp;
                else s[j] -= temp;
            }    
        }
    }
    //计算完美匹配权值之和
    int result = 0;
    for (int i = 0; i < W.size(); i++)
    {
        if (Match[i] != -1)
        {
            for (int j = 0; j < Graphic[Match[i]].size(); j++)
            {
                if (Graphic[Match[i]][j].first == i)
                    result += Graphic[Match[i]][j].second;
            }
        }
    }
    return -result;
}
//使用思路一求解
void Idea1(){
    cout << "Here is Idea_1 : input \"0 0\" to end " << endl;
    while ((cin >> n >> m) && (n > 0) && (m > 0)) { //多次输入
        if ((n == 0) && (m == 0)){
            cout << "End of Idea_1" << endl <<endl;      
            return;
        }
        GetInput(n, m);     //输入
        ExtractMatrix();    //提取输入
        if (W.size() != G.size()){  //排错
            cout << "The number of warehouse and goods is inconsistent, Idea_1 is over" <<endl;
            return;
        }    
        makeGraphic();   //带权二部图
        cout << "The minimum distance : " << KM() <<endl; 
        for (int i = 0; i < 120; i++)   //清空网络，为新的网络服务
        {
            Graphic[i].clear();
        }
        CleanUp();
    }
}
#endif