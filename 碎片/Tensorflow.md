# Tensorflow



## tf.keras.utils.get_file()

[tf.keras.utils.get_file](https://www.tensorflow.org/api_docs/python/tf/keras/utils/get_file#args_1)

## 记录图片的数量

```python
image_count = len(list(data_dir.glob('*/*.jpg')))
print(image_count)
```

​	

```python
#获取一种图片
roses = list(data_dir.glob('roses/*'))
#打开图片
PIL.Image.open(str(roses[0]))
```

## [tf.keras.utils.image_dataset_from_directory](https://tensorflow.google.cn/api_docs/python/tf/keras/utils/image_dataset_from_directory#args)

从磁盘上的图像目录到`tf.data.Dataset`