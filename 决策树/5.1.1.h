#include<string.h>
#include<stdlib.h>
using namespace std;
int getAttr(int index,char *str){
	switch(index%7){
		case 1://ID
			return atoi(str);
		case 2://����
			return !strcmp(str,"����")?1:!strcmp(str,"����")?2:3;
		case 3://����
			return strcmp(str,"��")?1:2;
		case 4://�з�
			return strcmp(str,"��")?1:2;
		case 5://�Ŵ�
			return !strcmp(str,"һ��")?1:!strcmp(str,"��")?2:3;
		case 6://���
			return strcmp(str,"��")?1:2;
	}
}
