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

- zip  是将train_filenames和train_labels 打包形成元祖，一一对应。长度取两者小者
- 其中`tfrecord_train`是用来保存`TFRecord`文件的绝对路径。
- `filename`是照片的绝对路径构成的列表，`lable`是照片对于的类别列表，两个列表位置相互对应，这里的`feature`需要根据待保存的数据类型构建，这里因为要保存图片数据以及图片的类别，因此需要建立两项，然后数据类型要对应。之后建立实例`example`，最后执行序列化并写入文件，完成`TFRecord`文件的构建。





```python
Feature_description = {  # 定义Feature结构，告诉解码器每个Feature的类型是什么
    'image/encoded': tf.io.FixedLenFeature([], tf.string),
    'image/label': tf.io.FixedLenFeature([], tf.int64),
}

```



## 解码

```python
def _parse_example(example_string):  
    # 将 TFRecord 文件中的每一个序列化的 tf.train.Example 解码
    feature_dict = tf.io.parse_single_example(example_string,Feature_description)
    feature_dict['image/encoded'] = tf.io.decode_jpeg(feature_dict['image/encoded'], channels=3)  # 解码JPEG图片
    img = feature_dict['image/encoded']
    img = tf.image.resize(img, (96, 96))
    img = img/255
    return img, tf.stack([tf.cast(feature_dict['image/label'], 'int32')])

```

```python
tf.io.decode_jpeg(
    contents,
    channels=0,
    ratio=1,
    fancy_upscaling=True,
    try_recover_truncated=False,
    acceptable_fraction=1,
    dct_method='',
    name=None
)
#contents --字符串类型的张量。 0-D, 一个0维的string类型的张量。即JPEG编码的图像内容。
#ratio允许在解码期间将图像按比例缩小整数倍。允许的值有：1，2，4和8。这比之后再缩小图像要快得多
```

1. tf.io.parse_single_example来进行解码
2.  [tf.io.decode_jpeg](https://www.tensorflow.org/api_docs/python/tf/io/decode_jpeg)
   1. 参数
   2. ` acceptable_fraction=1,` 
      - 0：使用 JPEG 编码图像中的通道数。
      - 1：输出灰度图。
      - 3：输出一张RGB图像。
   3. 返回类型为uint8的张量
3. 

## 加载TFRecord文件生成数据集

```python
"""**************************
加载TFRecord文件生成数据集
**************************"""
#train_file  --是TFRecord文件所在位置
raw_dataset = tf.data.TFRecordDataset(train_file)  # 读取 TFRecord 文件

#repeat(count)函数是这批数据重复，可以指定参数，默认行为（如果 count是None或-1）是无限期重复数据集。。写这个的目的是当队列里最后一批数据（batch）不足
#时可以补，不然会出现个报错 “OutOfRangeError: End of sequence”
raw_dataset = raw_dataset.repeat()

#用map函数解析数据。这里map需要传入一个自定义函数来解析，相当于把之前的转换在逆进行一次。简单来说就是告诉map函数这个文件该如何解码。---_parse_example --具体解码细节
train_datasets = raw_dataset.map(_parse_example)

# 用来打乱数据集中数据顺序,
# shuffle是防止数据过拟合的重要手段，然而不当的buffer size，会导致shuffle无意义，？？？？
train_datasets = train_datasets.shuffle(buffer_size=1024)

train_datasets = train_datasets.batch(batch)

train_datasets = train_datasets.prefetch(buffer_size=-1)  #开启预加载
#prefetch是可以减少GPU空闲时间，提前加载后面批次的数据，参数是GPU预读取的数据数量，这里调成根据GPU状态动态调整。


#处理 val_file
raw_dataset2 = tf.data.TFRecordDataset(ver_file)  # 读取 TFRecord 文件
raw_dataset2 = raw_dataset2.repeat()
val_datasets = raw_dataset2.map(_parse_example)
val_datasets = val_datasets.batch(batch)
```

### 关于buffer_size在shuffle和prefetch

- Dataset.prefetch() 中的 buffer_size 仅影响生成下一个元素所需的时间。

  - buffer_size --- 一个 tf.int64 标量 tf.Tensor，表示预取时将缓冲的最大元素数。如果使用值 tf.data.AUTOTUNE，则缓冲区大小是动态调整的。

  

  创建一个`Dataset`从该数据集中预取元素的 。

  大多数数据集输入管道应以调用结束`prefetch`。这允许在处理当前元素时准备后面的元素。这通常会提高延迟和吞吐量，但代价是使用额外的内存来存储预取的元素。

  

  

# [What is the proper use of Tensorflow dataset prefetch and cache options?](https://stackoverflow.com/questions/63796936/what-is-the-proper-use-of-tensorflow-dataset-prefetch-and-cache-options)

当 GPU 在当前批次上进行前向/反向传播时，我们希望 CPU 处理下一批数据，使其立即准备就绪。作为计算机中最昂贵的部分，我们希望 GPU 能够充分在训练期间一直使用。我们称这种消费者/生产者重叠，其中消费者是 GPU，生产者是 CPU。

使用 tf.data，您可以通过在管道末端（批处理后）简单调用 dataset.prefetch(1) 来完成此操作。这将始终预取一批数据并确保始终有一个准备就绪。

在某些情况下，预取多个批次可能很有用。例如，如果预处理的持续时间变化很大，预取 10 个批次将平均处理 10 个批次的时间，而不是有时等待更长的批次。

举一个具体的例子，假设 10% 的批次需要 10 秒来计算，而 90% 的批次需要 1 秒。如果 GPU 需要 2 秒来训练一个批次，那么通过预取多个批次，您可以确保我们永远不会等待这些罕见的更长的批次。”

我不太确定如何确定每个批次的处理时间，但这是下一步。如果您的批次处理时间大致相同，那么我相信 prefetch(batch_size=1) 应该足够了，因为您的 GPU 不会等待 CPU 完成处理计算量大的批次。











- [Dataset.shuffle](https://tensorflow.google.cn/api_docs/python/tf/data/Dataset#shuffle)() 中的 buffer_size 会影响数据集的随机性，从而影响元素的生成顺序。

buffer_size::一个[`tf.int64`](https://tensorflow.google.cn/api_docs/python/tf#int64)标量[`tf.Tensor`](https://tensorflow.google.cn/api_docs/python/tf/Tensor)，表示新数据集将从中采样的此数据集中的元素数。 ---**如果你不想要懂原理，想要真正地、有效地用到这个shuffle方法，直接把size设置为整个数据集大小，或者成倍大小（感觉没必要）。**

该数据集用`buffer_size`元素填充缓冲区，然后从该缓冲区中随机采样元素，用新元素替换所选元素。为了完美改组，需要缓冲区大小大于或等于数据集的完整大小。

例如，如果您的数据集包含 10,000 个元素但`buffer_size`设置为 1,000，则`shuffle`最初只会从缓冲区的前 1,000 个元素中选择一个随机元素。一旦选择了一个元素，它在缓冲区中的空间将被下一个（即第 1,001 个）元素替换，从而保持 1,000 个元素的缓冲区。







```python
"""******************
创建网络模型设置优化器
******************"""
optimizer = tf.keras.optimizers.Adam(learning_rate=0.01)
loss_fn = tf.losses.SparseCategoricalCrossentropy()

model = resnet(10575)

model.compile(loss=loss_fn,
              optimizer=optimizer,
              metrics=[metrics.MySparseAccuracy(batch)])


"""************************
    设置回调保存模型
************************"""
checkpoint_dir = "./ckpt1"
if not os.path.exists(checkpoint_dir):
  os.makedirs(checkpoint_dir)

ckpt_path = tf.train.latest_checkpoint('./ckpt1')
if ckpt_path is not None:
            print("[*] load ckpt from {}".format(ckpt_path))
            model.load_weights(ckpt_path)
else:
            print("[*] training from scratch.")

callbacks = [tf.keras.callbacks.ModelCheckpoint(
            filepath=checkpoint_dir + '/ckpt-loss{loss:.2f}' + 'resnet_accuracy{myaccuracy:.3f}',
            save_freq='epoch',
            save_weights_only=True,
            monitor='myaccuracy',
            verbose=1
        )]

steps_per_epoch = 408072 //batch
val_steps = 45341 // batch

"""***********************
        开始训练
***********************"""
print('Start training...')
model.fit(train_datasets, epochs=epochs,callbacks=callbacks, steps_per_epoch=steps_per_epoch,
          validation_data=val_datasets, validation_steps=val_steps)
print("training done!")
```




### steps_per_epoch 和val_steps

steps_per_epoch

相当于有多少批次的数据从队列传过来，data_size是数据的数量，在整除batch就相当于所有数据跑一次了。用整除也是为了防止之前说的那个报错。

val_steps

仅当提供了 validation_data 并且是 tf.data 数据集时才相关。在每个纪元结束时执行验证时停止之前要绘制的总步骤数（样本批次）。如果 'validation_steps' 为 None，验证将运行直到 validation_data 数据集耗尽。在无限重复的数据集的情况下，它将陷入无限循环。如果指定了“validation_steps”并且仅使用部分数据集，则评估将从每个时期的数据集开头开始。这确保每次都使用相同的验证样本。
