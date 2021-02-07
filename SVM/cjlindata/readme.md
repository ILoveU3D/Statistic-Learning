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
  '-t 3'：
  '-t 4'：
  '-d'
  '-g'
  '-r'
  '-c'
  '-n'
  '-p'
  '-m'
  '-e'
  '-h'
  '-b'
  '-wi'
  '-v'
  '-q'
  ```
