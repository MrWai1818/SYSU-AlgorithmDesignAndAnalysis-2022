#include <iostream>
#include <vector>
using namespace std;

int merge_sort(vector<int> &input,int l, int r) {
    vector<int> temp(input.size()); //用于作比较的数组
    if (l >= r)
        return 0;
    int mid = (l + r) / 2;      
    int cnt = merge_sort(input,l, mid) + merge_sort(input, mid+1 , r);  //数组不断二分为若干左数组和右数组，递归实现归并排序
    int from_l = l, from_r = mid+1;     //两个指针，分别是遍历左数组、右数组
    for (int k = l; k <= r; k ++) {             //复制待排序数组
        temp[k] = input[k];
    }
    for (int k = l; k <= r; k++) {      //每一次排序，指针一共要走的长度为 l+r-1 ,即数组长度
        if(from_l == mid +1){               //代表左数组已经遍历完
            input[k] = temp[from_r++];          //填充右边数组
        }else if (from_r==r+1 ||temp[from_l] <= temp[from_r]) {     //右数组遍历完 或 左数组数字更小(代表不要排序，可以直接用)
            input[k] = temp[from_l++];              //排序数组填上左数组值
        } else {                                //出现逆序情况，即左数组的值大于右数组，需要排序
            input[k] = temp[from_r++];          //填上右数组值
            cnt+=mid-from_l+1;              //因为from_l之前的左数组已经排序完成，说明剩下的数字都大于当前填入右数组值，所以都可构成为逆序对
        }
    }
    return cnt;
}

int main(){
	vector<int> input;		//输入数组
    cout<<"Input the arrays : ";	
    string istr;		//输入字符串
    cin >> istr; 
    for (auto &p : istr){		//格式化输入，读取数组
        if (p == ',' || p == '(' || p == ')'){
            p = ' ';
        }else{
            input.push_back(p-'0');
        }
    }
    int res = merge_sort(input,0,input.size()-1);	//得到结果
    cout<<"The number of reverse pairs :"<<res<<endl;
	return 0;
}