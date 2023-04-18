# cv

初学者教程

1. 设置TensorFlow
2.  加载数据集
3. 构建机器学习模型
4. 训练并评估模型

```python
import tensorflow as tf

#加载并准备 MNIST 数据集。将样本数据从整数转换为浮点数：
mnist = tf.keras.datasets.mnist
(x_train, y_train), (x_test, y_test) = mnist.load_data()
x_train, x_test = x_train / 255.0, x_test / 255.0


model = tf.keras.models.Sequential([
  tf.keras.layers.Flatten(input_shape=(28, 28)),
  tf.keras.layers.Dense(128, activation='relu'),
  tf.keras.layers.Dropout(0.2),
  tf.keras.layers.Dense(10)
])
//对于每个样本，模型都会返回一个包含 logits 或 log-odds 分数的向量，每个类一个。



predictions = model(x_train[:1]).numpy()
tf.nn.softmax(predictions).numpy()
#可以将 tf.nn.softmax 烘焙到网络最后一层的激活函数中。虽然这可以使模型输出更易解释，但不建议使用这种方式，因为在使用 softmax 输出时不可能为所有模型提供精确且数值稳定的损失计算。


loss_fn = tf.keras.losses.SparseCategoricalCrossentropy(from_logits=True)
#此损失等于 true 类的负对数概率：如果模型确定类正确，则损失为零。这个未经训练的模型给出的概率接近随机（每个类为 1/10），因此初始损失应该接近 -tf.math.log(1/10) ~= 2.3。

loss_fn(y_train[:1], predictions).numpy()


//准备训练
#在开始训练之前，使用 Keras Model.compile 配置和编译模型。将 optimizer 类设置为 adam，将 loss 设置为您之前定义的 loss_fn 函数，并通过将 metrics 参数设置为 accuracy 来指定要为模型评估的指标。
model.compile(optimizer='adam',
              loss=loss_fn,
              metrics=['accuracy'])


//训练并评估
model.fit(x_train, y_train, epochs=5)
##Model.evaluate 方法通常在 "Validation-set" 或 "Test-set" 上检查模型性能。
model.evaluate(x_test,  y_test, verbose=2)




#如果您想让模型返回概率，可以封装经过训练的模型，并将 softmax 附加到该模型：
probability_model = tf.keras.Sequential([
  model,
  tf.keras.layers.Softmax()
])
```



## [加载和预处理图像](https://tensorflow.google.cn/tutorials/load_data/images?hl=zh-cn)

- 首先，您将使用高级 Keras 预处理效用函数（例如 [`tf.keras.utils.image_dataset_from_directory`](https://tensorflow.google.cn/api_docs/python/tf/keras/utils/image_dataset_from_directory?hl=zh-cn)）和层（例如 [`tf.keras.layers.Rescaling`](https://tensorflow.google.cn/api_docs/python/tf/keras/layers/Rescaling?hl=zh-cn)）来读取磁盘上的图像目录。
- 然后，您将[使用 tf.data](https://tensorflow.google.cn/guide/data?hl=zh-cn) 从头编写自己的输入流水线。
- 最后，您将从 [TensorFlow Datasets](https://tensorflow.google.cn/datasets?hl=zh-cn) 中的大型[目录](https://tensorflow.google.cn/datasets/catalog/overview?hl=zh-cn)下载数据集。

# TensorFlow：Image classification

本教程遵循基本的机器学习工作流程：

检查和理解数据
构建输入管道
构建模型
训练模型
测试模型

改进模型并重复该过程

创建模型

- 顺序模型 --Sequential

```python
num_classes = len(class_names)

model = Sequential([
  layers.Rescaling(1./255, input_shape=(img_height, img_width, 3)),
  layers.Conv2D(16, 3, padding='same', activation='relu'),
  layers.MaxPooling2D(),
  layers.Conv2D(32, 3, padding='same', activation='relu'),
  layers.MaxPooling2D(),
  layers.Conv2D(64, 3, padding='same', activation='relu'),
  layers.MaxPooling2D(),
  layers.Flatten(),
  layers.Dense(128, activation='relu'),
  layers.Dense(num_classes)
])
```

编译模型

[Model.compile](https://tensorflow.google.cn/api_docs/python/tf/keras/Model#compile)

- 选择[`tf.keras.optimizers.Adam`](https://tensorflow.google.cn/api_docs/python/tf/keras/optimizers/Adam)优化器

- [`tf.keras.losses.SparseCategoricalCrossentropy`](https://tensorflow.google.cn/api_docs/python/tf/keras/losses/SparseCategoricalCrossentropy)损失函数。
- 要查看每个训练时期的训练和验证准确性，请将`metrics`参数传递给[`Model.compile`](https://tensorflow.google.cn/api_docs/python/tf/keras/Model#compile)。

```python
model.compile(optimizer='adam',
              loss=tf.keras.losses.SparseCategoricalCrossentropy(from_logits=True),
              metrics=['accuracy'])
```

Model summary

使用Keras方法查看网络的所有层[`Model.summary`](https://tensorflow.google.cn/api_docs/python/tf/keras/Model#summary)：

[fit](https://tensorflow.google.cn/api_docs/python/tf/keras/Model#fit)---训练模型

`model.fit()`

```python
epochs=10
history = model.fit(
  train_ds,
  validation_data=val_ds,
  epochs=epochs
)
```

## 过拟合

训练准确度随时间线性增加，而验证准确度在训练过程中停滞在 60% 左右。此外，训练和验证准确性之间的准确性差异很明显——[过度拟合](https://tensorflow.google.cn/tutorials/keras/overfit_and_underfit)的迹象。

当训练示例数量较少时，模型有时会从训练示例中的噪声或不需要的细节中学习——在一定程度上会对模型在新示例上的性能产生负面影响。这种现象被称为过度拟合。这意味着该模型将很难在新数据集上进行泛化。

### 	对抗过度拟合

- 数据扩充

- Dropout

  

## error

运行错误：Could not create cudnn handle: CUDNN_STATUS_INTERNAL_ERROR

![image-20230417190618836](/home/ly/.config/Typora/typora-user-images/image-20230417190618836.png)

解决：

TensorFlow2

```python
from tensorflow.compat.v1 import ConfigProto
from tensorflow.compat.v1 import InteractiveSession

config = ConfigProto()
config.gpu_options.allow_growth = True
session = InteractiveSession(config=config)
```

## 选择适合的cuda版本

```bash
export CUDA_HOME=/usr/local/cuda-11.0
export LD_LIBRARY_PATH=/usr/local/cuda-11.0/lib64:$LD_LIBRARY_PATH
export PATH=/usr/local/cuda-11.0/bin:$PATH

source ~/.bashrc
```

**cond打包环境时，只会打包对应环境已经下载的包，不包括base包内的东西。**
