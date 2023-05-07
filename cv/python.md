# python

# 画图

## matplotlib.pyplot.figure 

创建新图形，或激活现有图形。

plt.figure(figsize=(8, 8))

参数：

- *figsize* --宽度、高度以英寸为单位。

## plt.subplot(nrows, ncols, index)

plt.subplot(1,2,1) ---plt.subplot(1,2,2)  对比下

nrows 与 ncols 表示要划分几行几列的子区域（nrows*nclos表示子图数量），index 的初始值为1，用来选定具体的某个子区域。

![image-20230428205846942](/home/ly/.config/Typora/typora-user-images/image-20230428205846942.png)

参数：

- 三个整数（*nrows*、*ncols*、*index*）。子图将 在具有*nrows*行和*ncols*列的网格上获取*索引*位置。 *index*从左上角的 1 开始，向右递增。*index也可以是指定子图的 (* *first* , *last* ) 索引（从 1 开始，包括*last*）的二元 组，例如，制作一个跨越图的上 2/3 的子图。`fig.add_subplot(3, 1, (1, 2))`
- 一个 3 位整数。这些数字被解释为好像分别作为三个单位数整数给出，即`fig.add_subplot(235)`与 相同。请注意，这只能在不超过 9 个子图时使用。`fig.add_subplot(2, 3, 5)`

## matplotlib.pyplot.plot

```python
plt.plot(x, y, color='green', marker='o', linestyle='dashed',
     linewidth=2, markersize=12)
plt.plot(epochs_range, acc, label='Training Accuracy')
# x, y ,和其它参数
```

## plt.[legend](https://so.csdn.net/so/search?q=legend&spm=1001.2101.3001.7020)()

函数的作用是给图像加图例。

```python
plt.legend(loc='lower right')  #下端右边  -- 下面第一张图 --Training and Validation Accuracy

plt.legend(loc='upper right')  #上端右边  -- 第二张图
```

图例是集中于地图一角或一侧的地图上各种符号和颜色所代表内容与指标的说明，有助于更好的认识地图。

![image-20230428211113282](/home/ly/.config/Typora/typora-user-images/image-20230428211113282.png)

## plt.title

```python
plt.title('Training and Validation Loss')
```

## plt.show()

# 文件操作

```python
os.listdir(path)
```

- ## 返回值

​				此方法返回一个列表，其中包含路径给定目录中条目的名称。

- **path** -- 这是需要探索的目录。

## zip() 

函数用于将可迭代的对象作为参数，将对象中对应的元素打包成一个个元组，然后返回由这些元组组成的列表。

如果各个迭代器的元素个数不一致，则返回列表长度与最短的对象相同，利用`*`号操作符，可以将元组解压为列表。



## Python 引号

Python 可以使用引号( **'** )、双引号( **"** )、三引号( **'''** 或 **"""** ) 来表示字符串，引号的开始与结束必须是相同类型的。

其中三引号可以由多行组成，编写多行文本的快捷语法，常用于文档字符串，在文件的特定地点，被当做注释。

## 变量类型

### 字符串

```python
>>> s = 'abcdef'
>>> s[1:5]
'bcde'
```



### 列表

List（列表） 是 Python 中使用最频繁的数据类型。

列表可以完成大多数集合类的数据结构实现。它支持字符，数字，字符串甚至可以包含列表（即嵌套）。

列表用 **[ ]** 标识，是 python 最通用的复合数据类型。

列表中值的切割也可以用到变量 **[头下标:尾下标]** ，就可以截取相应的列表，从左到右索引默认 0 开始，从右到左索引默认 -1 开始，下标可以为空表示取到头或尾。

![image-20230419104604958](/home/ly/.config/Typora/typora-user-images/image-20230419104604958.png)



### 元组

元组是另一个数据类型，类似于 List（列表）。

元组用 () 标识。内部元素用逗号隔开。但是元组不能二次赋值，相当于只读列表。

```python
#!/usr/bin/python
# -*- coding: UTF-8 -*-
 
tuple = ( 'runoob', 786 , 2.23, 'john', 70.2 )
tinytuple = (123, 'john')
 
print tuple               # 输出完整元组
print tuple[0]            # 输出元组的第一个元素
print tuple[1:3]          # 输出第二个至第四个（不包含）的元素 
print tuple[2:]           # 输出从第三个开始至列表末尾的所有元素
print tinytuple * 2       # 输出元组两次
print tuple + tinytuple   # 打印组合的元组
```

```python
('runoob', 786, 2.23, 'john', 70.2)
runoob
(786, 2.23)
(2.23, 'john', 70.2)
(123, 'john', 123, 'john')
('runoob', 786, 2.23, 'john', 70.2, 123, 'john')
```

### 字典

字典(dictionary)是除列表以外python之中最灵活的内置数据结构类型。列表是有序的对象集合，字典是无序的对象集合。

两者之间的区别在于：字典当中的元素是通过键来存取的，而不是通过偏移存取。

字典用"{ }"标识。字典由索引(key)和它对应的值value组成。

**值可以取任何数据类型，但键必须是不可变的，如字符串，数字或元组。**

```python
#!/usr/bin/python
# -*- coding: UTF-8 -*-
 
dict = {}
dict['one'] = "This is one"
dict[2] = "This is two"
 
tinydict = {'name': 'runoob','code':6734, 'dept': 'sales'}
 
 
print dict['one']          # 输出键为'one' 的值
print dict[2]              # 输出键为 2 的值
print tinydict             # 输出完整的字典
print tinydict.keys()      # 输出所有键
print tinydict.values()    # 输出所有值
```

```python
This is one
This is two
{'dept': 'sales', 'code': 6734, 'name': 'runoob'}
['dept', 'code', 'name']
['sales', 6734, 'runoob']
```

