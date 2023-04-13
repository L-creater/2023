## numpy.dot() 

两个数组的点积。具体来说，

- 如果*a*和*b*都是一维数组，则它是向量的内积（没有复共轭）。
- 如果*a*和*b*都是二维数组，则为矩阵乘法，但优先使用[`matmul`](https://numpy.org/doc/stable/reference/generated/numpy.matmul.html#numpy.matmul)or 。`a @ b`
- 如果*a*或*b*是 0-D（标量），则它等效于 [`multiply`](https://numpy.org/doc/stable/reference/generated/numpy.multiply.html#numpy.multiply)并使用or是首选。`numpy.multiply(a, b)``a * b`
- 如果*a*是 ND 数组而*b*是一维数组，则它是*a*和*b*的最后一个轴上的和积。
- 如果*a*是 ND 数组，*b*是 MD 数组（其中），则它是*a*`M>=2`的最后一个轴与*b*的倒数第二个轴的和 积：

例：

```
np.dot(3, 4)
12
```

两个参数都不是复共轭的：

```
>>> np.dot([2j, 3j], [2j, 3j])
(-13+0j)
```

对于二维数组，它是矩阵乘积：

```
>>> a = [[1, 0], [0, 1]]
>>> b = [[4, 1], [2, 2]]
>>> np.dot(a, b)
array([[4, 1],
       [2, 2]])
>>> a = np.arange(3*4*5*6).reshape((3,4,5,6))
>>> b = np.arange(3*4*5*6)[::-1].reshape((5,4,6,3))
>>> np.dot(a, b)[2,3,2,1,2,2]
499128
>>> sum(a[2,3,2,:] * b[1,2,:,2])
499128
```

## numpy.arange()

## numpy.reshape()

1. 在不更改其数据的情况下为数组赋予新形状。
2. 形状变化是基于[数组元素](https://so.csdn.net/so/search?q=数组元素&spm=1001.2101.3001.7020)不能改变的，变成的新形状中所包含的元素个数必须符合原来元素个数。如果数组元素发生变化的时候，就会报错：
3. reshape函数生成的新数组和原始数组公用一个内存，也就是说，不管是改变新数组还是原始数组的元素，另一个数组也会随之改变：

numpy.reshape(a, newshape, order='C')[source]
参数：
	a : 要重塑的数组。
	newshape : int 或 int元组
		//新形状应与原始形状兼容。如果是整数，则结果将是该长度的一维数组。

​		一个形状维度可以是 -1。在这种情况下，该值是根据数组的长度和剩余维度推断出来的。

​	order : {'C', 'F', 'A'}, 可选
​    使用此索引顺序读取a的元素，并使用此索引顺序将元素放入重塑的数组中。'C' 表示使用类似 C 的索引顺序读取/写入元素，最后一个轴索引变化最快，回到第一个轴索引变化最慢。'F' 表示使用类似 Fortran 的索引顺序读取/写入元素，第一个索引变化最快，最后一个索引变化最慢。请注意，“C”和“F”选项不考虑底层数组的内存布局，仅参考索引顺序。'A' 表示如果a在内存中是 Fortran连续的，则以类似Fortran 的索引顺序读取/写入元素，否则以类似 C 的顺序读取/写入元素。

## y = y.astype(np.int)

astype() 方法转换 NumPy 数组的类型。 astype() 方法通过参数指定期望的类型,这个例子中是 np.int 型

## np.dim()--获得数组维数

## np.shape()-- 获得数组形状

## np.dot() 

---乘积也称为点积）。

1. np.dot()接收两个NumPy数组作为参 数，并返回数组的乘积。
2. 这里要注意的是，np.dot(A, B)和np.dot(B, A)的 值可能不一样。
3. 和一般的运算（+或*等）不同，矩阵的乘积运算中，操作数（A、 B）的顺序不同，结果也会不同。

## np.random.choice()

  ---从指定的数字中随机选择想要的数字

- np.random.choice(60000, 10) 会从 0 到 59999 之间随机选择 10 个数字

## np.arange(bach_size)

---生成一个从 0 到 batch_size-1 的数组

比如当 batch_size 为 5时， np.arange(batch_size) 会生成一个 NumPy 数组 [0,1, 2, 3, 4] 。



## np.zeros_like(x)

np.zeros_like(x) 会生成一个形状和 x 相同、所有元素都为 0 的数组。

## np.random.rand(10,1,28,28)  

随机生成数据

- rand函数根据给定维度生成[0,1)之间的数据，包含0，不包含1
- dn表格每个维度
- 返回值为指定维度的array

```
numpy.random.randn(d0,d1,…,dn)
```

- randn函数返回一个或一组样本，具有标准正态分布。
- dn表格每个维度
- 返回值为指定维度的array

## np.argmax(x)

取出数组中最大值元素的索引

参数 axis=1 。这指定了在 100 × 10 的数组中，沿着第 1 维方向（以第 1 维为轴）找到值最大的元素的索引（第 0 维对应第 1 个维度） 

```python
>>> x = np.array([[0.1, 0.8, 0.1], [0.3, 0.1, 0.6],
...
[0.2, 0.5, 0.3], [0.8, 0.1, 0.1]])
>>> y = np.argmax(x, axis=1)
>>> print(y)
[1 2 1 0]
```



## range(start,end)

会生成一个由 start 到 end-1 之间的整数构成的列表。

像 range(start,end, step) 这样指定 3 个整数，则生成的列表中的下一个元素会增加 step 指定的值。

```python
>>> list( range(0, 10) )
[0, 1, 2, 3, 4, 5, 6, 7, 8, 9]78
>>> list( range(0, 10, 3) )
[0, 3, 6, 9]
```

