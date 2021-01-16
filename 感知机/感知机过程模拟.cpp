#include<iostream>
#include<iomanip>
#include<fstream>
#include<string.h>
#include<vector>
#include<stdlib.h>
#include<stdio.h>
using namespace std;

int d = 2; //����ά�� 
vector<vector<double> > x;//��������
vector<double> y;//��ǩ 

void data_load(string f){ //���ݶ�ȡ 
	fstream file(f.c_str(),ios::in);
	if(!file.is_open()){
		cout<<"�ļ���ʧ��"<<endl;
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


//ģ��ʵ��P39 �㷨2.1

//�����֪��ģ��
class proceptorns{
public:
	vector<double> w;//Ȩ�� 
	double b;//ƫ�� 
	double ita;
	
	proceptorns(int size){
		for(int i=0;i<size;i++){
			w.push_back(0);
		}
		b=0;
		ita = 0.9;
	}
	
	void train(vector<vector<double> > x,vector<double> y){
		//����x��ѡ�񲻷��ϵ�x����
		for(int i=0;i<x.size();i++){
			double sum=0;
			for(int j=0;j<x[i].size();j++){ //����w*x+b 
				sum+= w[j]*x[i][j];
			}
			sum+=b;
			if(y[i]*sum<=0){ //������ 
				for(int j=0;j<x[i].size();j++){ //����w 
					w[j] = w[j] + ita * y[i] * x[i][j];
				}
				b = b + ita * y[i];//����b 
				cout<<"����"<<i<<"������"<<endl;
				i=-1;
			}else{
				cout<<"��"<<i<<"�����"<<endl; 
			}
			show();
		}
	}
	
	void show(){ //��ʾ��� 
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
