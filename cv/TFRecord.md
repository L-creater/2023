# TFRecord



## [tf.io.TFRecordWriter](https://www.tensorflow.org/api_docs/python/tf/io/TFRecordWriter)





## 1.基础数据格式

三种基础数据类型：bytes，float，int64

对应tf.train中三种类型：BytesList (字符串列表), FloatList (浮点数列表), Int64List (64位整数列表)，构造它们三个依照下面的方式传入相对应的value。

```python
tf.train.BytesList(value=[context_idxs.tostring()]
tf.train.Int64List(value=[1,2])
tf.train.FloatList(value=[0.1,0.2])
```

## 2.example 协议格式

要将我们的数据写入 .tfrecords 文件，需要将每一个样本数据封装为tf.train.Example格式，再将Example逐个写入文件。Example格式中的数据基础类型是tf.train.Feature

tf.train.Feature(): 它的参数是BytesList, FloatList, Int64List三种

```python
tf.train.Feature(bytes_list=tf.train.BytesList(value=)
tf.train.Feature(int64_list=tf.train.Int64List(value=)
tf.train.Feature(float_list=tf.train.FloatList(value=)
```



[`tf.train.Features`](https://www.tensorflow.org/api_docs/python/tf/train/Features): 它的参数是一个字典，k-v对中 v 的类型是Feature，对应每一个字段。

```python
tf.train.Features(feature={
            "k1": tf.train.Feature(bytes_list=tf.train.BytesList(value=)),
            "k2": tf.train.Feature(bytes_list=tf.train.BytesList(value=)),
            "k3": tf.train.Feature(float_list=tf.train.FloatList(value=)),
        })
```

`tf.train.FeatureList`: 它的参数是一个Feature的list, 

​		---- [Feature1, Feature2,...]

​		

```python
# tf.train.FeatureList
FeatureList = List[Feature]
```



```python
"context_idxs": tf.train.FeatureList(feature=
                                   [tf.train.Feature(int64_list=tf.train.Int64List(value=[])])
```

`tf.train.FeatureLists`: 它的参数是一个字典，k-v对中 , 	v 的类型是`FeatureList`。

```python
# tf.train.FeatureLists
FeatureLists = Dict[str, FeatureList]
```

```python
feature_lists=tf.train.FeatureLists(feature_list={
                "k1": tf.train.FeatureList(feature=                                    [tf.train.Feature(int64_list=tf.train.Int64List(value=[])]),
                "k2": tf.train.FeatureList(feature=                                          [tf.train.Feature(int64_list=tf.train.Int64List(value=v))])
            })
```

**我们需要根据我们的数据，找到每一个字段应该映射为**

- Feature或FeatureList,
-  多个Feature组成Features，
- 多个FeatureList组成FeatureLists, 
- 然后我们就定义了我们的一个训练数据对应的 Features，FeatureLists, 
- 再将其封装为 tf.train.Example 就可以写入 tfrecords二进制文件了。

```python
#传入的features对应一个 tf.train.Features
tf.train.Example(features=): 
#传入的context对应一个 tf.train.Features, features_lists对应一个tf.train.FeatureLists
tf.train.SequenceExample(context=, featurelists=): 
```

1. 这样就需要选择使用Example还是SequenceExample
2.  SequenceExample多了一个featurelists, 
3. 也就是说如果数据中存在字段，我们把它映射为了FeatureList而不是Feature, 那么就要用SequenceExample, 否则用Example。 ----??????

- **那么什么样的数据需要映射为FeatureList或Feature？**

- 我的理解是对于长度固定的字段类型，映射为Feature， 比如分类问题中的类别这个字段一般用一个数字表示，二分类就是0或1，那么就class=0映射为tf.train.Feature(tf.train.Int64List(value=[0])), 只要这个字段包含的数据维度是固定的，就可以封装为 Feature。

- 对于长度不固定的字段类型，映射为FeatureList。比如NLP样本有一个特征是一句话，那么一句话的长度是不固定的，NLP中一般是先分词，然后把每个词对应为该词在字典中的索引，一句话就用一个一维整形数组来表示 [2, 3, 5, 20, ...]，这个数组的长度是不固定的，我们就映射为

  ```python
  tf.train.FeatureList(feature=[tf.train.Feature(value=[v]) for v in [2, 3, 5, 20,...] ] )
  ```

## 3.数据写入

```python
writer = tf.python_io.TFRecordWriter(out_file)
for row in train_data:
    record = tf.train.SequenceExample(row)   # 将一行数据转换为定义的Example格式
   # record = tf.train.Example()
    writer.write(record.SerializeToString())
```

## 4.验证数据写入是否正确

## 5.训练时数据读取方式

## 构建TFRecord文件

​		接下来再来看

```python
with tf.io.TFRecordWriter(tfrecord_train) as writer: 
    for filename, label in zip(train_filenames, train_labels):
         # 读取数据集图片到内存，image 为一个 Byte 类型的字符串
        image = open(filename, 'rb').read()
        # 建立 tf.train.Feature 字典
        feature = {              
             # 图片是一个 Bytes 对象
            'image': tf.train.Feature(bytes_list=tf.train.BytesList(value=[image])),
             # 标签是一个 Int 对象
            'label': tf.train.Feature(int64_list=tf.train.Int64List(value=[label])) 
           
        }
        # 通过字典建立 Example
        example = tf.train.Example(features=tf.train.Features(feature=feature)) 
     	 # 将Example序列化并写入 TFRecord 文件
        writer.write(example.SerializeToString())  
        
```

- 其中`tfrecord_train`是用来保存`TFRecord`文件的绝对路径。
- `filename`是照片的绝对路径构成的列表，`lable`是照片对于的类别列表，两个列表位置相互对应，这里的`feature`需要根据待保存的数据类型构建，这里因为要保存图片数据以及图片的类别，因此需要建立两项，然后数据类型要对应。之后建立实例`example`，最后执行序列化并写入文件，完成`TFRecord`文件的构建。



























