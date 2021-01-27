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

//��ȡ���ݲ���ȡ���� 
Set readCsv(char *str){
	fstream file(str,ios::in);
	Set set;
	if(!file.is_open()){
		cout<<setw(6)<<"file error"<<endl;
		return set;
	}
	char *p;
	string q;
	getline(file,q);//�Ȱѱ����й��˵� 
	while(getline(file,q)){
		p = strtok((char *)q.c_str(),",");
		Record rec;
		for(int i=1;p;i++,p=strtok(NULL,","))
			rec.push_back(getAttr(i,p));
		set.push_back(rec);
	}
	return set;
}

//����ĳһ������������غ������� 
void conditionalEnproty(Set set,const int catas,const int catasNum,const int C,double &HDA,double &HAD){
	vector<double> Di(catasNum);//��������Ե�����������Di��
	vector<vector<double> > Dik;//��������ĳ���������Ե�����������Dik��
	for(int i=0;i<catasNum;i++){
		vector<double> t(C);
		Dik.push_back(t);
	}
	for(int i=0;i<set.size();i++){//ɨ��һ�����ݣ�ͳ�Ƹ���Ԫ�� 
		Di[set[i][catas]-1]++;//Di
		Dik[set[i][catas]-1][set[i][set[i].size()-1]-1]++;//Dik
	}
	HDA=HAD=0;//����������HDA��������HAD 
	for(int i=0;i<catasNum;i++){
		double sum=0;
		for(int k=0;k<C;k++)
			if(Dik[i][k]!=0)
				sum+=Dik[i][k]/Di[i]*log2(Dik[i][k]/Di[i]);
		HDA+=sum*Di[i]/set.size();//��ʽ5.8
		HAD+=Di[i]/set.size()*log2(Di[i]/set.size()); //����5.3 
	}
	HDA=-HDA;
	HAD=-HAD;
}

//���������������Ϣ����� 
void mutualInfoRatio(Set set,const int catas,const int *catasNum,const int C){
	//ɨ��һ��set,�õ�������
	vector<double> Ck(C);
	for(int i=0;i<set.size();i++)
		Ck[set[i][set[i].size()-1]-1]++;
	double HD=0,HDA=0,HAD=0;
	for(int i=0;i<C;i++)
		HD+=Ck[i]/set.size()*log2(Ck[i]/set.size());//��ʽ5.7 
	HD=-HD;
	//������������
	for(int i=0;i<catas;i++){
		conditionalEnproty(set,i,catasNum[i],C,HDA,HAD);
		double ratio = (HD-HDA)/HAD;//��ʽ5.9 
		cout<<"��"<<i+1<<"�����Ե���Ϣ�����Ϊ��"<<setprecision(3)<<ratio<<endl;
		cout<<"HD"<<HD<<"HDA"<<HDA<<"HAD"<<HAD<<endl;
	}
}

int main(){
	Set s = readCsv((char *)"��5.1.csv");
	int ca[5] = {15,3,2,2,3};
	mutualInfoRatio(s,5,ca,2);
	return 0;
}

