#include<string.h>
#include<stdlib.h>
using namespace std;
int getAttr(int index,char *str){
	switch(index%7){
		case 1://ID
			return atoi(str);
		case 2://年龄
			return !strcmp(str,"青年")?1:!strcmp(str,"中年")?2:3;
		case 3://工作
			return strcmp(str,"否")?1:2;
		case 4://有房
			return strcmp(str,"否")?1:2;
		case 5://信贷
			return !strcmp(str,"一般")?1:!strcmp(str,"好")?2:3;
		case 6://类别
			return strcmp(str,"否")?1:2;
	}
}
