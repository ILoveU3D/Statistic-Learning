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
