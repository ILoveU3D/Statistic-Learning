#include<fstream>
#include<iostream>
#include<vector>
#include<string.h>
#include<iomanip>
#include<cmath>
#include"5.1.1.h"
using namespace std;
typedef vector<int> Record;
typedef vector<vector<int> > Set;

//读取数据并提取属性 
Set readCsv(char *str){
	fstream file(str,ios::in);
	Set set;
	if(!file.is_open()){
		cout<<setw(6)<<"file error"<<endl;
		return set;
	}
	char *p;
	string q;
	getline(file,q);//先把标题行过滤掉 
	while(getline(file,q)){
		p = strtok((char *)q.c_str(),",");
		Record rec;
		for(int i=1;p;i++,p=strtok(NULL,","))
			rec.push_back(getAttr(i,p));
		set.push_back(rec);
	}
	return set;
}

//计算某一个种类的条件熵和特征熵 
void conditionalEnproty(Set set,const int catas,const int catasNum,const int C,double &HDA,double &HAD){
	vector<double> Di(catasNum);//计算各属性的特征个数（Di）
	vector<vector<double> > Dik;//计算属于某种类别各属性的特征个数（Dik）
	for(int i=0;i<catasNum;i++){
		vector<double> t(C);
		Dik.push_back(t);
	}
	for(int i=0;i<set.size();i++){//扫描一轮数据，统计各个元素 
		Di[set[i][catas]-1]++;//Di
		Dik[set[i][catas]-1][set[i][set[i].size()-1]-1]++;//Dik
	}
	HDA=HAD=0;//计算条件熵HDA和特征熵HAD 
	for(int i=0;i<catasNum;i++){
		double sum=0;
		for(int k=0;k<C;k++)
			if(Dik[i][k]!=0)
				sum+=Dik[i][k]/Di[i]*log2(Dik[i][k]/Di[i]);
		HDA+=sum*Di[i]/set.size();//公式5.8
		HAD+=Di[i]/set.size()*log2(Di[i]/set.size()); //定义5.3 
	}
	HDA=-HDA;
	HAD=-HAD;
}

//计算所有种类的信息增益比 
void mutualInfoRatio(Set set,const int catas,const int *catasNum,const int C){
	//扫描一遍set,得到经验熵
	vector<double> Ck(C);
	for(int i=0;i<set.size();i++)
		Ck[set[i][set[i].size()-1]-1]++;
	double HD=0,HDA=0,HAD=0;
	for(int i=0;i<C;i++)
		HD+=Ck[i]/set.size()*log2(Ck[i]/set.size());//公式5.7 
	HD=-HD;
	//计算所有种类
	for(int i=0;i<catas;i++){
		conditionalEnproty(set,i,catasNum[i],C,HDA,HAD);
		double ratio = (HD-HDA)/HAD;//公式5.9 
		cout<<"第"<<i+1<<"种属性的信息增益比为："<<setprecision(3)<<ratio<<endl;
		cout<<"HD"<<HD<<"HDA"<<HDA<<"HAD"<<HAD<<endl;
	}
}

int main(){
	Set s = readCsv((char *)"表5.1.csv");
	int ca[5] = {15,3,2,2,3};
	mutualInfoRatio(s,5,ca,2);
	return 0;
}

