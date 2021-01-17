#include<vector>
#include<queue>
#include<algorithm>
#include<iostream>
#include<cmath>
//����kd�������ݽṹ 
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
	
	bool createTree(Set x,int t){ //����kd�� ,t��ʼΪ1��0��y��ǩ�� 
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
	
	void showTree() const{ //��α������kd��
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
	
	double nearest(vector<double> s,Set &result,int t,int depth) const{ //���ù���õ�kd��Ѱ�������ڽ���k���� 
		double minest=0;
		//���������ĳһҶ��
		if(2*depth+2 >= data.size()){ //Ҷ�ӽڵ� 
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
		if(2*depth+1>=data.size()) //����Ҷ�ӽڵ� 
			return minest;
		//�����������kd����
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
				minest = nearest_searching(result,s,2*depth+2,minest,t+1); //���������� 
			}else{
				minest = nearest_searching(result,s,2*depth+1,minest,t+1); //���������� 
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
