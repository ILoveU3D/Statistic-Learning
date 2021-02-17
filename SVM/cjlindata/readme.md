### 有关libsvm的训练函数用法，解释如下：
```
"""
  svm_train(y, x [, options]) -> model | ACC | MSE

	y: a list/tuple/ndarray of l true labels (type must be int/double).

	x: 1. a list/tuple of l training instances. Feature vector of
	      each training instance is a list/tuple or dictionary.

	   2. an l * n numpy ndarray or scipy spmatrix (n: number of features).

	svm_train(prob [, options]) -> model | ACC | MSE
	svm_train(prob, param) -> model | ACC| MSE

	Train an SVM model from data (y, x) or an svm_problem prob using
	'options' or an svm_parameter param.
	If '-v' is specified in 'options' (i.e., cross validation)
	either accuracy (ACC) or mean-squared error (MSE) is returned.
	options:
	    -s svm_type : set type of SVM (default 0)
	        0 -- C-SVC		(multi-class classification)
	        1 -- nu-SVC		(multi-class classification)
	        2 -- one-class SVM
	        3 -- epsilon-SVR	(regression)
	        4 -- nu-SVR		(regression)
	    -t kernel_type : set type of kernel function (default 2)
	        0 -- linear: u'*v
	        1 -- polynomial: (gamma*u'*v + coef0)^degree
	        2 -- radial basis function: exp(-gamma*|u-v|^2)
	        3 -- sigmoid: tanh(gamma*u'*v + coef0)
	        4 -- precomputed kernel (kernel values in training_set_file)
	    -d degree : set degree in kernel function (default 3)
	    -g gamma : set gamma in kernel function (default 1/num_features)
	    -r coef0 : set coef0 in kernel function (default 0)
	    -c cost : set the parameter C of C-SVC, epsilon-SVR, and nu-SVR (default 1)
	    -n nu : set the parameter nu of nu-SVC, one-class SVM, and nu-SVR (default 0.5)
	    -p epsilon : set the epsilon in loss function of epsilon-SVR (default 0.1)
	    -m cachesize : set cache memory size in MB (default 100)
	    -e epsilon : set tolerance of termination criterion (default 0.001)
	    -h shrinking : whether to use the shrinking heuristics, 0 or 1 (default 1)
	    -b probability_estimates : whether to train a SVC or SVR model for probability estimates, 0 or 1 (default 0)
	    -wi weight : set the parameter C of class i to weight*C, for C-SVC (default 1)
	    -v n: n-fold cross validation mode
	    -q : quiet mode (no outputs) 
	"""
  ```
  ### 逐行翻译过来即
  ```
  调用svm_train函数，第一个参数为标签，第二个参数为输入数据，后面可以跟一些模型参数以供交叉验证。返回三个参数，第一个是训练好的模型，第二个是训练集准确率，第三个是误差
  y的数据结构可以是python的list，tuple，或者numpy数组，一维向量，1表示正样本
  x的数据结构可以是list，tuple，dict，或者numpy矩阵，格式为序号：特征属性值，当然这个格式读取方式可以在源码中自行修改
  下面是一个参数的调节：（一起来看libsvm有哪些功能,所有的功能一起夹在一个字符串list里面）
  '-v'：返回训练集准确率和误差
  '-s 0'：软间隔[0,∞]多分类SVM（CSVM）
  '-s 1'：软间隔[0,1]多分类SVM（nuSVM）
  '-s 2'：单分类SVM（只有一类训练样本，输出是/不是）
  '-s 3'：回归问题支持向量机
  '-s 4'：回归问题支持向量机2
  '-t 0'：线性核
  '-t 1'：多项式核
  '-t 2'：径向基函数（高斯核）
  '-t 3'：sigmoid核
  '-t 4'：自定义核
  '-d'：多项式核的次方
  '-g'：高斯核的方差
  '-r'：sigmoid核的偏置
  '-c'：CSVM中的平滑因子
  '-n'：nuSVM中的平滑因子
  '-p'：回归SVM中的epsilon
  '-m'：内存使用
  '-e'：目标误差（容忍度）
  '-h'：启发式搜索
  '-b'：概率估计
  '-wi'：回归SVM中的权重
  '-v n'：n次交叉验证
  '-q'：无返回训练
  ```
  
  ### 有关libsvm的训练函数用法，解释如下：
  ```
  """
	svm_predict(y, x, m [, options]) -> (p_labels, p_acc, p_vals)

	y: a list/tuple/ndarray of l true labels (type must be int/double).
	   It is used for calculating the accuracy. Use [] if true labels are
	   unavailable.

	x: 1. a list/tuple of l training instances. Feature vector of
	      each training instance is a list/tuple or dictionary.

	   2. an l * n numpy ndarray or scipy spmatrix (n: number of features).

	Predict data (y, x) with the SVM model m.
	options:
	    -b probability_estimates: whether to predict probability estimates,
	        0 or 1 (default 0); for one-class SVM only 0 is supported.
	    -q : quiet mode (no outputs).

	The return tuple contains
	p_labels: a list of predicted labels
	p_acc: a tuple including  accuracy (for classification), mean-squared
	       error, and squared correlation coefficient (for regression).
	p_vals: a list of decision values or probability estimates (if '-b 1'
	        is specified). If k is the number of classes, for decision values,
	        each element includes results of predicting k(k-1)/2 binary-class
	        SVMs. For probabilities, each element contains k values indicating
	        the probability that the testing instance is in each class.
	        Note that the order of classes here is the same as 'model.label'
	        field in the model structure.
	"""
  ```
  ### 逐行翻译过来即
  ```
  调用svm_predict函数，第一个参数为y标签，用来验证准确率，如果纯预测，那就以[]代替，第二个参数是x，和训练集的格式相同，第三个参数训练返回的模型，第四个参数是各项设置参数
  下面是参数调节（同样是写在一个list里面）
  '-b':是否预测为各项概率
  '-q':是否有返回值
  返回值是一个tuple，包括了三项，分别是预测的标签，准确率（或回归误差），概率值
  ```
