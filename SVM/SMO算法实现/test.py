"""
本文档展示了SMOMod中的接口用法
"""
import SMOMod as smo
import os

# 继承SupportVectorMachine类，可以重写核函数
class mySVM(smo.SupportVecterMachine):
    def __init__(self, x, y, C, toler, kernel):
        return super().__init__(x, y, C, toler, kernel)

# 读取数据的函数（根据数据的具体特征来读）
def readFile(fileName):
    data = open(fileName,'r')
    x = []
    y = []
    for line in data.readlines():
        dt = line.split(' ')
        y.append(1 if dt[0] is '1' else -1)
        x.append([float(item) for item in dt[1:len(dt)-1]])
    return x,y

# bupa-train.txt 一组非线性分类数据集
x, y = readFile("bupa-train.txt")
svm = mySVM(x,y,3,0.1,("guassian",0.8))
smo.SMO(svm,35)
train_correct = 0
for i in range(len(y)):
    _, r = smo.predict(svm,x[i],y[i])
    train_correct += 1 if r else 0
print("corrected rate of trainning data: %f%%" % (train_correct / len(y) * 100))
x, y = readFile("bupa-test.txt")
test_correct = 0
for i in range(len(y)):
    _, r = smo.predict(svm,x[i],y[i])
    test_correct += 1 if r else 0
print("corrected rate of test data: %f%%" % (test_correct / len(y) * 100))
