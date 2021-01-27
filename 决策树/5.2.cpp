#include<iostream>
#include<numeric>
#include<vector>
#include<cmath>
using namespace std;

//表5.2 
const double x[10]={1,2,3,4,5,6,7,8,9,10};
const double y[10]={4.5,4.75,4.91,5.34,5.80,7.05,7.9,8.23,8.7,9};
const int len=10;

struct Node{
	double x;
	double y;
}data[10];

vector<Node> tree;//回归树 

//生成最小回归树 
void getTree(int begin,int end){
	if(begin>end)
		return;
	if(begin==end||begin==end-1){
		tree.push_back(data[begin]);
		return;
	}
	double maxn=0;
	int max_i=0;
	for(int i=begin;i<=end;i++){
		double sum_l=0,sum_r=0,ave_l=0,ave_r=0;
		for(int j=begin;j<=i;j++)
			ave_l+=data[j].y;
		for(int j=i+1;j<=end;j++)
			ave_r+=data[j].y;
		ave_l/=i-begin+1;
		ave_r/=end-i>0;
		for(int j=begin;j<=i;j++)
			sum_l+=abs(data[j].y-ave_l);
		for(int j=i+1;j<=end;j++)
			sum_r+=abs(data[j].y-ave_r);
		if(sum_l+sum_r>maxn){
			maxn = sum_l+sum_r;
			max_i = i;
		}
	}
	tree.push_back(data[max_i]);
	getTree(begin,max_i-1);
	getTree(max_i+1,end);
}

//回归（深度搜索） 
double test(double x,int depth){
	if(x==tree[depth].x)
		return tree[depth].y;
	else if(x<tree[depth].x&&2*depth+1<tree.size())
		return test(x,2*depth+1);
	else if(x>tree[depth].x&&2*depth+2<tree.size())
		return test(x,2*depth+2);
	return tree[depth].y;
} 

int main(){
	for(int i=0;i<len;i++){
		data[i].x=x[i];
		data[i].y=y[i];
	}
	getTree(0,len-1);
	for(vector<Node>::iterator ite=tree.begin();ite!=tree.end();ite++){
		cout<<(*ite).x<<ends;
		cout<<(*ite).y<<endl;
	}
	cout<<endl;
	//test
	for(int i=-1;i<=10;i++){
		cout<<i<<":"<<test(i,0)<<endl;
	}
	return 0;
}
