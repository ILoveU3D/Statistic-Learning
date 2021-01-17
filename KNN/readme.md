## K邻近算法
上面的头文件kdtree.h中定义了kd树类，并且包括了kd树的一系列操作，包括：
1. `createTree(Set x,int t)`>根据输入的集合x创建kd树
2. `showTree()` 输出树
3. `getKNearest(Set r,point x)` 得到点x的k个最邻近点
4. `test(point x)`  测试待测点x

### 测试结果
![result]
(http://a1.qpic.cn/psc?/V51BIuzF0MWGsS2w8dUj3KLBWS2TJRCm/ruAMsa53pVQWN7FLK88i5ku9o83WeHdoUVgDsWc2*gdzrpfwxLNUbAkMPcm076*YdDWm44D3hkaCY22svTXrNDCxyApmQxpRN2YqYD2duN0!/b&ek=1&kp=1&pt=0&bo=hAIhAQAAAAADF5Q!&tl=1&vuin=2360085507&tm=1610888400&sce=60-2-2&rf=viewer_4)

### kdtree.h
```C++
#include<vector>
#include<queue>
#include<algorithm>
#include<iostream>
#include<cmath>
//构造kd树的数据结构 
using namespace std;
typedef vector<vector<double> > Set;

class SortAid{
public:
	double data;
	int index;
	SortAid(double data,int index){
		this->data = data;
		this->index = index; 
	}
	inline bool operator<(const SortAid &x) const{
		return this->data < x.data;
	}
};

class KdTree{
	Set data;
	int k; //k
	
public:
	KdTree(int k){
		this->k = k;
	}
	
	bool createTree(Set x,int t){ //构造kd树 ,t初始为1（0是y标签） 
		if(x.empty())
			return true;
		Set left,right;
		vector<SortAid> temp;
		t = t%k+1;
		for(int j=0;j<x.size();j++){
			SortAid sa(x[j][t],j);
			temp.push_back(sa);
		}
		sort(temp.begin(),temp.end());
		for(int j=0;j<temp.size();j++){
			if(j<temp.size()/2)
				left.push_back(x[temp[j].index]);
			else if(j>temp.size()/2)
				right.push_back(x[temp[j].index]);
		}
		data.push_back(x[temp[temp.size()/2].index]);
		return createTree(left,t+1) && createTree(right,t+1);
	}
	
	void showTree() const{ //层次遍历输出kd树
		cout<<data.size()<<endl;
		for(int i=0;i<data.size();i++){
			cout<<"(";
			for(int j=0;j<data[i].size();j++){
				cout<<data[i][j]<<",";
			}
			cout<<")";
			if((i+2)&(i+1)?false:true)
				cout<<endl;
		}	
	}
	
	double nearest(vector<double> s,Set &result,int t,int depth) const{ //利用构造好的kd树寻找样本邻近的k个点 
		double minest=0;
		//深度搜索到某一叶子
		if(2*depth+2 >= data.size()){ //叶子节点 
			for(int i=0;i<data[depth].size();i++){
				minest+=(s[i]-data[depth][i])*(s[i]-data[depth][i]);
			}
			minest = sqrt(minest);
			//if(data[depth].size()<5) cout<<"warning!d="<<depth<<endl;
			result.push_back(data[depth]);
			return minest;
		}else{
			if(data[depth][t%k+1]<s[t%k+1]){
				if(2*depth+1 < data.size())
					minest = nearest(s,result,t+1,2*depth+1);
			}
			else{
				if(2*depth+2 < data.size())
					minest = nearest(s,result,t+1,2*depth+2);
			}
		}
		return nearest_searching(result,s,depth,minest,t);
	}
	
	double nearest_searching(Set &result,vector<double> s,int depth,double minest,int t) const{
		if(2*depth+1>=data.size()) //到达叶子节点 
			return minest;
		//逐层向上向下kd搜索
		if(abs(data[depth][t%k+1]-s[t%k+1])<minest){
			double sum=0;
			for(int i=0;i<data[depth].size();i++){
				sum+=(s[i]-data[depth][i])*(s[i]-data[depth][i]);
			}
			sum=sqrt(sum);
			if(sum<minest){
				minest = sum;
				//if(data[depth].size()<5) cout<<"warning!d="<<depth<<endl;
				result.push_back(data[depth]);
			}
			if(s[t%k+1]<=data[depth][t%k+1]){
				minest = nearest_searching(result,s,2*depth+2,minest,t+1); //搜索右子树 
			}else{
				minest = nearest_searching(result,s,2*depth+1,minest,t+1); //搜索左子树 
			}
		}
		return minest;
	}
	
	double getKNearest(Set &result,vector<double> s) const{
		double minest = nearest(s,result,1,0);
		reverse(result.begin(),result.end());
		while(result.size()>k)
			result.pop_back();
		return minest;
	}
	
	bool test(vector<double> s){
		Set r;
		getKNearest(r,s);
		int count[10]={0};
		for(int i=0;i<r.size();i++){
			count[int(r[i][0])]++;
		}
		int max_i,max_is=0;
		for(int i=0;i<10;i++){
			if(count[i]>max_is){
				max_i=i;
				max_is=count[i];
			}
		}
		cout<<"the label is "<<s[0]<<" while the predict is "<<max_i<<endl;
		return max_i == s[0];
	}
};
```

### KNNMain.cpp
```C++
#include<iostream>
#include<fstream>
#include<string.h>
#include<stdlib.h>
#include "kdtree.h"

using namespace std;

Set data_load(string f){ //读取数据 
	fstream file(f.c_str(),ios::in);
	Set x;
	if(!file.is_open()){
		cout<<"文件打开失败！"<<endl;
		return x;
	}
	string temp;
	char *p;
	while(getline(file,temp)){
		p=strtok((char *)temp.c_str()," ");
		vector<double> vx;
		while(p!=NULL){
			vx.push_back(atof(p));
			p=strtok(NULL," ");
		}
		x.push_back(vx);
	}
	return x;
}


int main(){
	int mx_k,mx_test=0;
	for(int k=1;k<50;k++){
		KdTree kt(34);
		Set train = data_load("./BS-train.txt");
		kt.createTree(train,1);
		//kt.showTree();
		double correct_train = 0;
		for(int i=0;i<train.size();i++){
			correct_train += kt.test(train[i])?1:0;
			cout<<"group "<<i<<endl;
		}
		Set test = data_load("./BS-test.txt");
		double correct_test = 0;
		for(int i=0;i<test.size();i++){
			correct_test += kt.test(test[i])?1:0;
			cout<<"group "<<i<<endl;
		}
		cout<<"train set correct rate: "<<correct_train/train.size()*100<<"%"<<endl;
		cout<<"test set correct rate: "<<correct_test/test.size()*100<<"%"<<endl;
		
		if(correct_test>mx_test){
			mx_k=k;
			mx_test=correct_test;
		}
		cout<<"epoch:"<<k<<endl;
	}
	cout<<"best k:"<<mx_k<<endl;
	return 0;
}
```
