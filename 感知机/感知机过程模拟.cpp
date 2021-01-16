#include<iostream>
#include<iomanip>
#include<fstream>
#include<string.h>
#include<vector>
#include<stdlib.h>
#include<stdio.h>
using namespace std;

int d = 2; //数据维度 
vector<vector<double> > x;//输入数据
vector<double> y;//标签 

void data_load(string f){ //数据读取 
	fstream file(f.c_str(),ios::in);
	if(!file.is_open()){
		cout<<"文件打开失败"<<endl;
	}
	string temp;
	char *p;
	while(getline(file,temp)){
		p = strtok((char *)temp.c_str()," ");
		vector<double> xv;
		while(p!=NULL){
			xv.push_back(atof(p));
			p=strtok(NULL," ");
		}
		y.push_back(xv[xv.size()-1]);
		xv.pop_back();
		x.push_back(xv);
	}
}


//模拟实现P39 算法2.1

//定义感知机模型
class proceptorns{
public:
	vector<double> w;//权重 
	double b;//偏置 
	double ita;
	
	proceptorns(int size){
		for(int i=0;i<size;i++){
			w.push_back(0);
		}
		b=0;
		ita = 0.9;
	}
	
	void train(vector<vector<double> > x,vector<double> y){
		//遍历x，选择不符合的x更新
		for(int i=0;i<x.size();i++){
			double sum=0;
			for(int j=0;j<x[i].size();j++){ //计算w*x+b 
				sum+= w[j]*x[i][j];
			}
			sum+=b;
			if(y[i]*sum<=0){ //不符合 
				for(int j=0;j<x[i].size();j++){ //更新w 
					w[j] = w[j] + ita * y[i] * x[i][j];
				}
				b = b + ita * y[i];//更新b 
				cout<<"发现"<<i<<"不符合"<<endl;
				i=-1;
			}else{
				cout<<"第"<<i<<"组符合"<<endl; 
			}
			show();
		}
	}
	
	void show(){ //显示结果 
		for(int i=0;i<w.size();i++){
			if(i!=0) cout<<" + ";
			cout<<setprecision(3)<<w[i]<<"*x("<<i+1<<")";
		}
		cout<<" + "<<setprecision(3)<<b<<endl;
	}
	
}; 

int main(){
	data_load("data.txt");
	proceptorns p(d);
	p.train(x,y);
	return 0;
}
