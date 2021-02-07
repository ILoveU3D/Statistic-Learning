# -*- coding:utf-8 -*-
"""
主要实现了SMO算法和对SMO算法结果进行绘制
参考：
1. 所有公式代号来自于：支持向量机通俗导论（理解SVM的三层境界）https://blog.csdn.net/v_july_v/article/details/7624837
2. 代码来自：支持向量机—SVM原理代码实现 https://www.cnblogs.com/further-further-further/p/9596898.html
                karpathy/svmjs https://github.com/karpathy/svmjs/blob/master/lib/svm.js
3. SMO原理参考自：SMO优化算法（Sequential minimal optimization）https://www.cnblogs.com/jerrylead/archive/2011/03/18/1988419.html
                                                                https://blog.csdn.net/wang_zuel/article/details/98987415
注释即其他部分代码作者：Quinn.Y.Wang
"""
import numpy as np

# SVM结构体，描述了支持向量机中的一些参量
# x:输入样本（n维）
# y:输入样本标签（±1）
# C:惩罚因子
# toler:容忍度
# length:输入样本数量
# alpha:拉格朗日乘子，每个样本点对应一个乘子
# b:超平面截距
# kernel:核函数
# K:核矩阵，存储每对样本核的内积
# ErrorCache:误差缓存，存储每个乘子优化阶段的误差
# ErrorIndex:误差标签，存储每个乘子优化阶段是否被更新（±1）
# kernelFunction:核函数，可以自己继承重载，目前只提供线形核与高斯核
class SupportVecterMachine:
    def __init__(self, x, y, C, toler, kernel):
        self.x = np.mat(x)
        self.y = np.mat(y).T
        self.C = C
        self.toler = toler
        self.length, self.dimension = np.shape(self.x)
        self.alpha = np.mat(np.zeros([self.length,1]))
        self.b = 0
        self.K = np.mat(np.zeros([self.length,self.length]))
        self.ErrorCache = np.mat(np.zeros(self.length)).T
        self.ErrorIndex = np.array(np.zeros(self.length)).T

        # 初始化核矩阵
        if type(kernel) is str:
            self.kernel = kernel
        elif type(kernel) is tuple:
            self.kernel = kernel[0]
            self.sigma = kernel[1]
        else:
            self.kernel = "undefined"

        for index in range(self.length):
            self.K[:,index] = self.kernelFunction(self.x, self.x[index,:])

    def kernelFunction(self, x, row_x):
        length = np.shape(x)[0]
        K = np.mat(np.zeros([length, 1]))
        if self.kernel is "linear":
            # 公式 2.2.21 线形核
            K = x * row_x.T
        elif self.kernel is "guassian":
            # 公式 2.2.20 高斯核
            for index in range(length):
                K[index] = (x[index,:] - row_x) * (x[index,:] - row_x).T
            K = np.exp(K / (-2 * self.sigma ** 2))
        else:
            raise NameError("kernel is invalid or not overrided")
        return K

# 计算误差
# 公式 3.5.21，3.5.22
def getError(svm, k):
    uk = float(np.multiply(svm.alpha, svm.y).T * svm.K[:,k] + svm.b)
    return uk - float(svm.y[k])

# SMO算法，实例化SVM对象后，将对象带入此方法中进行训练，并设置最大迭代次数
def SMO(svm, maxIter):
    # 迭代次数
    iter = 0
    # 全局遍历
    entire = True
    # 被修改的拉格朗日乘子对
    alphaChanged = 0
    # 当达到最大迭代次数，或者无乘子对再受到改变是训练完成，返回模型对象
    while iter < maxIter and (alphaChanged > 0 or entire):
        alphaChanged = 0
        # 全局遍历，遍历长度为所有样本
        if entire:
            checkedSet = range(svm.length)
        # 局部遍历，遍历长度为支持向量
        else:
            # 公式 3.5.13
            vects = np.array(np.multiply((svm.alpha > 0),(svm.alpha < svm.C)))
            checkedSet = np.nonzero(vects[:,0])[0]
        # 开始遍历
        for i in  checkedSet:
            # 选中第一个i
            Ei = getError(svm,i)
            # 公式 3.5.15，3.5.16，3.5.17 判断i是否违反了KKT条件
            if svm.y[i] * Ei < -svm.toler and svm.alpha[i] < svm.C or svm.y[i] * Ei > svm.toler and svm.alpha[i] > 0:
                j = -1
                maxDeltaE = 0
                Ej = 0
                # 取得所有被修改过（可能符合KKT条件）的乘子索引（可能包括i）
                validEIndex = np.nonzero(svm.ErrorIndex)
                if len(validEIndex) > 1:
                    for k in validEIndex:
                        if k is i:
                            continue
                        Ek = getError(svm, k)
                        # 计算i与j的ΔE并依据其找到最适合的j
                        deltaE = abs(Ei - Ek)
                        if deltaE > maxDeltaE:
                            maxDeltaE = deltaE
                            Ej = Ek
                            j = k
                # 没有乘子受到改变，随机选择一个j
                else:
                    j = i
                    while j is i:
                        j = int(np.random.uniform(0, svm.length))
                    Ej = getError(svm, j)
                print("Ej = %f" % Ej)
                print("selected J is %d" % j)
                # 深复制乘子，便于接下来的更新乘子核旧乘子做差
                alphaIOld = svm.alpha[i].copy()
                alphaJOld = svm.alpha[j].copy()
                # 公式 3.5.25，3.5.26 根据yi,yj同号或异号计算上界和下界
                if svm.y[i] != svm.y[j]:
                    L = max(0, svm.alpha[j] - svm.alpha[i])
                    H = min(svm.C, svm.C + svm.alpha[j] - svm.alpha[i])
                else:
                    L = max(0, svm.alpha[j] + svm.alpha[i] - svm.C)
                    H = min(svm.C, svm.alpha[j] + svm.alpha[i])
                if L == H:
                    print("L equals to H")
                    continue
                # 公式 3.5.23
                eta = svm.K[i,i] + svm.K[j,j] - 2 * svm.K[i,j]
                if eta <= 0:
                    print("eta lower than 0")
                    continue
                # 公式 3.5.20
                svm.alpha[j] += svm.y[j] * (Ei - Ej) / eta
                # 公式 3.5.24 修剪
                if svm.alpha[j] > H:
                    svm.alpha[j] = H
                elif svm.alpha[j] < L:
                    svm.alpha[j] = L
                # 更新j的Error缓存
                svm.ErrorCache[j] = getError(svm, j)
                svm.ErrorIndex[j] = 1
                # 如果乘子的改变量非常小，可以认为已经训练到最优解，视作不更新
                if abs(svm.alpha[j] - alphaJOld) < 10e-5:
                    print("not update, abandon it")
                    continue
                # 公式 3.5.27
                svm.alpha[i] += svm.y[i] * svm.y[j] * (alphaIOld - svm.alpha[j])
                # 更新i的Error缓存
                svm.ErrorCache[i] = getError(svm, i)
                svm.ErrorIndex[i] = 1
                # 公式 3.5.29
                bi = svm.b - Ei - svm.y[i] * (svm.alpha[i] - alphaIOld) * svm.K[i,i] - svm.y[j] * (svm.alpha[j] - alphaJOld) * svm.K[i,j]
                # 公式 3.5.30
                bj = svm.b - Ej - svm.y[i] * (svm.alpha[i] - alphaIOld) * svm.K[i,j] - svm.y[j] * (svm.alpha[j] - alphaJOld) * svm.K[j,j]
                # 公式 3.5.28
                if svm.alpha[i] > 0 and svm.alpha[i] < svm.C:
                    svm.b = bi
                elif svm.alpha[j] > 0 and svm.alpha[j] < svm.C:
                    svm.b = bj
                else:
                    svm.b = (bi + bj) / 2
                alphaChanged += 1
            print("alphaChanged = %d" % alphaChanged)
        iter += 1
        if entire:
            entire = False
        elif alphaChanged is 0:
            entire = True
        print("iter is %d" % iter)
    return svm

def predict(svm, x, answer=None):
    x = np.mat(x)
    # 公式 2.2.15
    prediction = np.multiply(svm.alpha, svm.y).T * svm.kernelFunction(svm.x, x) + svm.b
    prediction_y = int(np.sign(prediction))
    if answer is None:
        return prediction_y
    else:
        return prediction_y, answer is prediction_y

def twoDimensionFigure(svm,label1='label1', label2='label2', label1_color='red',label2_color='blue'):
    import matplotlib.pyplot as plt
    x1 = svm.x[np.nonzero(svm.y == -1)[0],:][:,0]
    y1 = svm.x[np.nonzero(svm.y == -1)[0],:][:,1]
    x1 = np.reshape(x1,len(x1)).A[0]
    y1 = np.reshape(y1,len(y1)).A[0]
    plt.scatter(x1, y1, color=label1_color, s=40, label=label1)
    x2 = svm.x[np.nonzero(svm.y == 1)[0],:][:,0]
    y2 = svm.x[np.nonzero(svm.y == 1)[0],:][:,1]
    x2 = np.reshape(x2,len(x2)).A[0]
    y2 = np.reshape(y2,len(y2)).A[0]
    plt.scatter(x2, y2, color=label2_color, s=40, label=label2) 
    plt.legend(loc = 'best')
    plt.show()
       
def areaFigure(svm,label1_color='red',label2_color='blue'):
    import matplotlib.pyplot as plt
    xl = np.linspace(-10,10,20)
    yl = np.linspace(-10,10,20)
    for i in range(len(xl)):
        for j in range(len(yl)):
            res = predict(svm,[xl[i],yl[j]])
            if res is -1:
                plt.scatter(xl[i],yl[j],color=label1_color,s=300)
            elif res is 1:
                plt.scatter(xl[i],yl[j],color=label2_color,s=300)
    plt.show()
