# cv

1. 滤波器的权重数据要按 (output_channel, input_channel, height, width) 的顺序书写.

   1. output_channel  ---滤波器个数
   2. input_channel --- 通道数

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



## 加载和预处理图像

- 首先，您将使用高级 Keras 预处理效用函数（例如 [`tf.keras.utils.image_dataset_from_directory`](https://tensorflow.google.cn/api_docs/python/tf/keras/utils/image_dataset_from_directory?hl=zh-cn)）和层（例如 [`tf.keras.layers.Rescaling`](https://tensorflow.google.cn/api_docs/python/tf/keras/layers/Rescaling?hl=zh-cn)）来读取磁盘上的图像目录。
- 然后，您将[使用 tf.data](https://tensorflow.google.cn/guide/data?hl=zh-cn) 从头编写自己的输入流水线。
- 最后，您将从 [TensorFlow Datasets](https://tensorflow.google.cn/datasets?hl=zh-cn) 中的大型[目录](https://tensorflow.google.cn/datasets/catalog/overview?hl=zh-cn)下载数据集。