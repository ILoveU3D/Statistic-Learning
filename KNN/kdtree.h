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
