# DBNet

![image-20230515155156926](/home/ly/.config/Typora/typora-user-images/image-20230515155156926.png)









Deformable convolution -- 扩大感受野  --pytorch封装好了

![image-20230515155426898](/home/ly/.config/Typora/typora-user-images/image-20230515155426898.png) 

# DBNet

代码中的路径改动  --数据路径data_path全部改为绝对路径 ， 

除了requirements 外  还要安装addict

1. icdar2015.yaml   --data_path
2. icdar2015_resnet18_FPN_DBhead_polyLR.yaml
3. tools/train.py  --下图

![image-20230519153600605](/home/ly/.config/Typora/typora-user-images/image-20230519153600605.png)

 **error:**

File "/content/drive/MyDrive/DBNet.pytorch/models/backbone/shufflenetv2.py", line 7, in <module>    from torchvision.models.utils import load_state_dict_from_url ModuleNotFoundError: No module named 'torchvision.models.utils'

```

查阅了torchvision的代码库后，有一个解决方案：

请注意，此语法仅适用于更高版本的 PyTorch。

来自 .utils import load_state_dict_from_url 的原始代码不适用。您不能从 .utils 导入 load_state_dict_from_url。

将 .utils 更改为 torch.hub 可以解决问题。

from torch.hub import load_state_dict_from_url
```

