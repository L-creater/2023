# [DBNet](https://www.jianshu.com/p/1eb20aad90b8)

![image-20230515155156926](/home/ly/.config/Typora/typora-user-images/image-20230515155156926.png)





![image-20230524094017677](/home/ly/.config/Typora/typora-user-images/image-20230524094017677.png)

1. 通过回归来，同回归来获得框的坐标
2. 基于分割的，ground truth 基于图像中为每一个像素点都给予分类，人车房子树，上的每一个像素点都给予分类。
   1. 怎么做  -- FCN
   2. 

Deformable convolution -- 扩大感受野  --pytorch封装好了

![image-20230515155426898](/home/ly/.config/Typora/typora-user-images/image-20230515155426898.png) 

## 1. 背景介绍

> 文本检测分为基于回归和基于分割两种方法，DBNet 的原理是基于分割算法。对于一般分割算法流程：先通过网络输出文本分割的概率图，然后使用设定阈值将概率图转化为二值图，然后通过后处理得到检测结果（文本框坐标）。但是缺点在于阈值的选取非常关键。

DBNet 针对这个问题，提出可微分二值化的概念：即对每一个像素点进行自适应二值化，二值化阈值由网络学习得到，彻底将二值化这一步骤加入到网络里一起训练，这样最终的输出图对于阈值就会非常鲁棒。

**自适应阈值**

![img](https:////upload-images.jianshu.io/upload_images/1845179-286a4215a4f09883.png?imageMogr2/auto-orient/strip|imageView2/2/w/411/format/webp)



上图中蓝线部分就是传统的文本检测算法流程：

- 首先，通过设置一个固定阈值将分割网络训练得到的概率图(segmentation map)转化为二值图(binarization map)。
- 然后，使用一些启发式技术(例如像素聚类)将像素分组为文本实例。

上图红色部分就是 DBNet 的算法流程：

> 通过分割网络获取概率图和阈值图，阈值图是网络预测得出，并不是固定的值，这样就可以很好将背景与前景分离出来，但是这样的操作会给训练带来梯度不可微的情况，对此对于二值化提出了一个叫做 **Differentiable Binarization**来解决不可微的问题。

## 2. DBNet 算法整体架构

![img](https:////upload-images.jianshu.io/upload_images/1845179-3e65c57d573cfda1.png?imageMogr2/auto-orient/strip|imageView2/2/w/962/format/webp)

上图是 DBNet 模型网络结构示意图，主要分为 3 个模块：

- **第一模块（1）**：左边的红框使用的是一个 **FPN**  结构，分为自底向上的卷积操作与自顶向下的上采样，以此来获取多尺度的特征。1 图下面部分是 3x3 的卷积操作，按照卷积公式分别获取原图大小比例的 `1/2、1/4、1/8、1/16、1/32` 的特征图；然后自顶向下进行上采样 x2，然后与自底向上生成的相同大小的特征图融合；融合之后再采用 3x3 的卷积消除上采样的混叠效应；最后对每层输出结果进行上采样，统一为 1/4 大小的特征图。
- **第二模块（2）**：将 1/4 大小的特征图经过一系列卷积和转置卷积的机构获取概率图 **P** 和阈值图 **T**，可参考 FCN 网络结构，目的是生成与原图一样大小的特征图 P 和 T。
- **第三模块（3）**：将特征图 P 和 T 经过 **DB 方法**（后续介绍）得到近似二值图。

经过上面三个模块，可以得到概率图、阈值图和近似二值图。在训练过程中，对这三个图进行监督学习，更新各个模块的参数。在推理过程中，直接使用概率图，然后使用固定阈值获取结果。

下面介绍可微二值化的发展流程：

**标准二值化**
 在传统的图像分割算法中，我们获取概率图后，会使用标准二值化（Standard Binarize）方法进行处理，将低于阈值的像素点置0，高于阈值的像素点置1，公式如下：
 ![B_{i, j}=\left\{\begin{array}{c} 1, \text { if } P_{i, j}>=t \\ 0, \text { otherwise } \end{array}\right.](https://math.jianshu.com/math?formula=B_%7Bi%2C%20j%7D%3D%5Cleft%5C%7B%5Cbegin%7Barray%7D%7Bc%7D%201%2C%20%5Ctext%20%7B%20if%20%7D%20P_%7Bi%2C%20j%7D%3E%3Dt%20%5C%5C%200%2C%20%5Ctext%20%7B%20otherwise%20%7D%20%5Cend%7Barray%7D%5Cright.)
 ![t](https://math.jianshu.com/math?formula=t) 是预先设置的阈值，![（i,j）](https://math.jianshu.com/math?formula=%EF%BC%88i%2Cj%EF%BC%89)代表的是像素点坐标位置。可见标准的二值化是不可微的，所以也就无法放入到网络中进行优化学习。

**可微二值化**
 可微二值化就是将标准二值化中的阶跃函数进行了近似，公式如下所示：
 ![\hat{\boldsymbol{B}}=\frac{1}{1+e^{-k\left(P_{i, j} - T_{i, j}\right)}}](https://math.jianshu.com/math?formula=%5Chat%7B%5Cboldsymbol%7BB%7D%7D%3D%5Cfrac%7B1%7D%7B1%2Be%5E%7B-k%5Cleft(P_%7Bi%2C%20j%7D%20-%20T_%7Bi%2C%20j%7D%5Cright)%7D%7D)
 可微二值化本质上是一个 带系数 k 的 sigmoid 函数，取值范围为![(0,1)](https://math.jianshu.com/math?formula=(0%2C1))，k 是膨胀因子（经验型设置为50）。![P_{i,j}](https://math.jianshu.com/math?formula=P_%7Bi%2Cj%7D) 是指概率图像素点，![T_{i,j}](https://math.jianshu.com/math?formula=T_%7Bi%2Cj%7D) 是指阈值图像素点。

标准二值化和可微二值化的对比如下图 (a) 所示，SB 曲线代表标准二值化，DB 代表可微二值化，可以看到曲线变得更为平滑，也就是可微：

![img](https:////upload-images.jianshu.io/upload_images/1845179-5928f1c6a6094c71.png?imageMogr2/auto-orient/strip|imageView2/2/w/414/format/webp)

除了可微之外，DB 方法也会改善算法的性能，在反向传播是梯度的计算上进行观察。当使用交叉熵损失（![y=1](https://math.jianshu.com/math?formula=y%3D1)代表文字区域）时，正负样本的 loss 分别为 ![l_+](https://math.jianshu.com/math?formula=l_%2B) 和 ![l_-](https://math.jianshu.com/math?formula=l_-)，公式如下：
![image-20230524165322540](/home/ly/.config/Typora/typora-user-images/image-20230524165322540.png)

对输入 ![x = P_{i, j}-T_{i, j}](https://math.jianshu.com/math?formula=x%20%3D%20P_%7Bi%2C%20j%7D-T_%7Bi%2C%20j%7D) 求偏导，则会得到：
 ![\frac{\delta l_{+}}{\delta x}=-k f(x) e^{-k x}, \\ \frac{\delta l_{-}}{\delta x}=-k f(x) ,](https://math.jianshu.com/math?formula=%5Cfrac%7B%5Cdelta%20l_%7B%2B%7D%7D%7B%5Cdelta%20x%7D%3D-k%20f(x)%20e%5E%7B-k%20x%7D%2C%20%5C%5C%20%5Cfrac%7B%5Cdelta%20l_%7B-%7D%7D%7B%5Cdelta%20x%7D%3D-k%20f(x)%20%2C)

可以看到 增强因子k 对于错误预测对梯度的影响变大了，从而可以促进模型的优化过程产生更为清晰的预测结果。对于![x>0](https://math.jianshu.com/math?formula=x%3E0)时，按照（a）图属于正样本（文字区域），![x<0](https://math.jianshu.com/math?formula=x%3C0)时属于负样本（非文字区域）。
 上图（b）是指 ![l_{+}](https://math.jianshu.com/math?formula=l_%7B%2B%7D) 的导数曲线，如果发生漏报（正样本被预测为负样本![x<0](https://math.jianshu.com/math?formula=x%3C0)），图（b）小于 0 的部分导数非常大，证明损失也是非常大的，则更能清晰的进行梯度回传。同理，图（c）代表 ![l_{-}](https://math.jianshu.com/math?formula=l_%7B-%7D)的导数曲线，当发生误报（负样本被预测为正样本![x>0](https://math.jianshu.com/math?formula=x%3E0)），导数也是非常大的，损失也比较大。

## 3. 真实标签生成

DB 网络中，训练过程中网络有 3 个输出：概率图、阈值图和近似二值图：

- 概率图：图中每个像素点的值为该位置属于文本区域的概率。
- 阈值图：图中每个像素点的值为该位置的二值化阈值。
- 近似二值图：由概率图和阈值图通过 DB 算法计算得到，图中像素的值为 0 或 1。

概率图的标签 ![G_{s}](https://math.jianshu.com/math?formula=G_%7Bs%7D)和阈值图标签![G_{d}](https://math.jianshu.com/math?formula=G_%7Bd%7D)，DB 网络参考 PSENet 中的方法，使用扩张和收缩的方式构建阈值图和概率图。在该方法中，对于一幅文字图像，文本区域的每个多边形使用一组线段![G=\left\{S_{k}\right\}_{k=1}^{n}](https://math.jianshu.com/math?formula=G%3D%5Cleft%5C%7BS_%7Bk%7D%5Cright%5C%7D_%7Bk%3D1%7D%5E%7Bn%7D) 来进行描述， n 为线段个数。

**概率图标签![G_{s}](https://math.jianshu.com/math?formula=G_%7Bs%7D)**
 对于概率图和近似二值图来说，使用收缩的方式构建标签（Vatti clipping算法），收缩的偏移量D由多边形的周长 L 和面积 A 计算得到，公式如下：其中， r 是收缩因子，实验中经验设置为 0.4 。
 ![D=\frac{A\left(1-r^{2}\right)}{L}](https://math.jianshu.com/math?formula=D%3D%5Cfrac%7BA%5Cleft(1-r%5E%7B2%7D%5Cright)%7D%7BL%7D)

**阈值图标签![G_{d}](https://math.jianshu.com/math?formula=G_%7Bd%7D)**

- 首先使用![G_{s}](https://math.jianshu.com/math?formula=G_%7Bs%7D)计算过程中的偏移量 D 进行多边形的扩充。得到![G_{s}](https://math.jianshu.com/math?formula=G_%7Bs%7D)和![G_{d}](https://math.jianshu.com/math?formula=G_%7Bd%7D)之间的区域。
- 计算之间区域到原始框的距离，并得到最近边（长方形就是 4 条边）的距离。最外面的大框线上区域和最里面的小框线上区域计算为 D ，原始框位置的距离为 0。
- 进行第一次的归一化（除以D），这样距离控制到 [0,1] 之间，并且最中间的区域越接近0，越里面和越外面的区域越接近1。然后使用 1-X 操作，让越中心的距离为 1，越边缘的距离为 0。（这样图片显示就是中间亮两头暗）。
- 最终再进行缩放，比如归一化到 [0.3，0.7] 的值。

## 4. 损失函数

> 由于在训练阶段输出 3 个预测图（大小与原图一致），所以在损失函数中，也需要有对应的真实标签去构建 3  部分损失函数。总的损失函数的公式定义如下：
>  ![L=L_{b}+\alpha \times L_{s}+\beta \times L_{t}](https://math.jianshu.com/math?formula=L%3DL_%7Bb%7D%2B%5Calpha%20%5Ctimes%20L_%7Bs%7D%2B%5Cbeta%20%5Ctimes%20L_%7Bt%7D)
>  其中，![L](https://math.jianshu.com/math?formula=L)为总的损失，![L_{b}](https://math.jianshu.com/math?formula=L_%7Bb%7D)为近似二值图的损失，使用 Dice 损失；![L_{s}](https://math.jianshu.com/math?formula=L_%7Bs%7D)为概率图损失，使用带 OHEM 的 Dice 损失；![L_{t}](https://math.jianshu.com/math?formula=L_%7Bt%7D)为阈值图损失，使用预测值和标签间的 𝐿1 距离。其中，𝛼 和 𝛽 为权重系数。
>  接下来分析这 3 个loss:
>  1）首先是Dice Loss，Dice Loss是比较预测结果跟标签之间的相似度，常用于二值图像分割。
>  ![diceloss =1-\frac{2 \times intersection area }{total area}](https://math.jianshu.com/math?formula=diceloss%20%3D1-%5Cfrac%7B2%20%5Ctimes%20intersection%20area%20%7D%7Btotal%20area%7D)
>  2）其次是MaskL1 Loss，是计算预测值和标签间的𝐿1距离
>  3）最后是Balance Loss，是带OHEM的Dice Loss，目的是为了改善正负样本不均衡的问题。OHEM 为一种特殊的自动采样方式，可以自动的选择难样本进行loss的计算，从而提升模型的训练效果。
>
> **Dice Loss (迁移)**
>  ![dice loss=1-\frac{2|X \bigcap Y|}{|X|+|Y|}](https://math.jianshu.com/math?formula=dice%20loss%3D1-%5Cfrac%7B2%7CX%20%5Cbigcap%20Y%7C%7D%7B%7CX%7C%2B%7CY%7C%7D)
>  其中 |X∩Y| 是X和Y之间的交集，|X|和|Y|分表表示X和Y的元素的个数，其中，分子的系数为2，是因为分母存在重复计算X和Y之间的共同元素的原因。
>  同时，一般会加入平滑因子，防止分子分母全为0。对于分割任务而言，|X| 和 |Y| 代表分割的 ground truth 和 predict_mask。
>
> 计算步骤：
>
> - 首先，使用预测图 predict_mask 和 ground truth 之间的点乘。
>
>   ![img](https:////upload-images.jianshu.io/upload_images/1845179-7e7ce24b51a1634a.png?imageMogr2/auto-orient/strip|imageView2/2/w/517/format/webp)
>
> - 逐元素相乘的结果元素的相加和。
>
>   ![img](https:////upload-images.jianshu.io/upload_images/1845179-fbdff8ac3dc421c3.png?imageMogr2/auto-orient/strip|imageView2/2/w/499/format/webp)
>
> - 计算|X|和|Y|，这里可以采用直接元素相加，也可以采用元素平方求和的方法：
>
>   ![img](https:////upload-images.jianshu.io/upload_images/1845179-95b8f11a36e921d3.png?imageMogr2/auto-orient/strip|imageView2/2/w/502/format/webp)

## 5. 模型训练流程

以 PaddleOCR ([https://github.com/PaddlePaddle/PaddleOCR/blob/0791714b91/deploy/lite/readme.md](https://links.jianshu.com/go?to=https%3A%2F%2Fgithub.com%2FPaddlePaddle%2FPaddleOCR%2Fblob%2F0791714b91%2Fdeploy%2Flite%2Freadme.md)) 为例，模型的训练流程分为以下部分：

- 数据预处理：主要包括解析图片、真实标签处理、随机裁剪和图片增强。
- 模型结构部分：分为 Backbone、Neck、Head 部分。
- loss 部分：分为 ![概率图 L_{s}、阈值图 L_{t}、近似二值图 L_{b}](https://math.jianshu.com/math?formula=%E6%A6%82%E7%8E%87%E5%9B%BE%20L_%7Bs%7D%E3%80%81%E9%98%88%E5%80%BC%E5%9B%BE%20L_%7Bt%7D%E3%80%81%E8%BF%91%E4%BC%BC%E4%BA%8C%E5%80%BC%E5%9B%BE%20L_%7Bb%7D)。
- metric 部分：经过后处理之后进行评价指标的计算。

**5.1 数据预处理**
 数据预处理包含以下部分：
 DecodeImage 解析图片、DetLabelEncode 解析 label 文件、IaaAugment 进行数据增强、EastRandomCropData 随机裁剪（裁剪到指定 size (960, 960, 3)）、MakeBorderMap 阈值图真实标签生成（batch, 960, 960）、MakeShrinkMap 概率图标签生成（batch, 960, 960）、NormalizeImage 归一化、ToCHWImage 纬度变化为（3, 960, 960）、KeepKeys 指定格式。

![img](https:////upload-images.jianshu.io/upload_images/1845179-dd5cbbf79634fac2.png?imageMogr2/auto-orient/strip|imageView2/2/w/1083/format/webp)

经过数据预处理 image 字段的大小变为（batch, 3, 960, 960）。

**5.2 模型前向传播**
 模型依次经过 backbone（MobileNetV3）--> neck（DBFPN） --> head（DBHead） 步骤，纬度变化如下所示：



```undefined
输入的 images size： [2, 3, 960, 960]，batch 为 2
backbone 输出1：backbone behind [2, 16, 240, 240]，尺寸对应 1/4
backbone 输出2：backbone behind [2, 24, 120, 120]，尺寸对应 1/8
backbone 输出3：backbone behind [2, 56, 60, 60]，尺寸对应 1/16
backbone 输出4：backbone behind [2, 480, 30, 30]，尺寸对应 1/32
neck 输出：neck behind [2, 96, 240, 240]，输出 1/4
head 输出：head behind [2, 3, 960, 960]，与原图大小一致，分别代表 shrink_maps, threshold_maps, binary_maps，大小都为 [960, 960]
```

**5.3 后处理**
 后处理的逻辑流程如下：

- 首先，对概率图进行固定阈值处理，得到分割图。
- 对分割图计算轮廓，遍历每个轮廓，去除太小的预测；对每个轮廓计算包围矩形，然后计算该矩形的预测score。
- 对矩形进行反向shrink操作，得到真实矩形大小；最后还原到原图 size 就可以了。

下面对 python 代码进行解析：
 pred 是模型的输出，shape 为 （1，height, weight），训练阶段都为（1，960，960），推理阶段则不一定。

![img](https:////upload-images.jianshu.io/upload_images/1845179-63efdb7943ddf531.png?imageMogr2/auto-orient/strip|imageView2/2/w/806/format/webp)

查看 boxes_from_bitmap 的代码：



```python
def boxes_from_bitmap(self, pred, _bitmap, dest_width, dest_height):
        '''
        _bitmap: single map with shape (1, H, W),
                whose values are binarized as {0, 1}
        '''

        bitmap = _bitmap
        height, width = bitmap.shape
        
        # findContours 获取轮廓，如长方形获取四点顶点坐标
        outs = cv2.findContours((bitmap * 255).astype(np.uint8), cv2.RETR_LIST,
                                cv2.CHAIN_APPROX_SIMPLE)
        # py2、py3 不同版本的情况
        if len(outs) == 3:
            img, contours, _ = outs[0], outs[1], outs[2]
        elif len(outs) == 2:
            contours, _ = outs[0], outs[1]
        
        # 文本框最大数量
        num_contours = min(len(contours), self.max_candidates)

        boxes = []
        scores = []
        for index in range(num_contours):
            contour = contours[index]
            #  计算最小包围矩，获取四个坐标点，左上为起点（顺时针）
            points, sside = self.get_mini_boxes(contour)
            # 长方形中宽高最小值过滤
            if sside < self.min_size:
                continue
            points = np.array(points)
            # 利用 points 内部预测概率值，计算出一个score,作为实例的预测概率
            score = self.box_score_fast(pred, points.reshape(-1, 2))
            # score 得分的过滤
            if self.box_thresh > score:
                continue
            # shrink反向还原，之前概率图进行了缩放，故还原
            box = self.unclip(points).reshape(-1, 1, 2)
            box, sside = self.get_mini_boxes(box)
            if sside < self.min_size + 2:
                continue
            box = np.array(box)
            
            # 还原到原始坐标，反向还原之后，还需要还原到原始图片（原始图片在预处理时被缩放处理）
            box[:, 0] = np.clip(
                np.round(box[:, 0] / width * dest_width), 0, dest_width)
            box[:, 1] = np.clip(
                np.round(box[:, 1] / height * dest_height), 0, dest_height)
            boxes.append(box.astype(np.int16))
            scores.append(score)
        return np.array(boxes, dtype=np.int16), scores

def get_mini_boxes(self, contour):
        # 返回点集 cnt 的最小外接矩形：# 得到最小外接矩形的（中心(x,y), (宽,高), 旋转角度）
        bounding_box = cv2.minAreaRect(contour)
        # 排序，最终以左上的坐标为起点，顺时针排列四个坐标点
        points = sorted(list(cv2.boxPoints(bounding_box)), key=lambda x: x[0])

        index_1, index_2, index_3, index_4 = 0, 1, 2, 3
        if points[1][1] > points[0][1]:
            index_1 = 0
            index_4 = 1
        else:
            index_1 = 1
            index_4 = 0
        if points[3][1] > points[2][1]:
            index_2 = 2
            index_3 = 3
        else:
            index_2 = 3
            index_3 = 2

        box = [
            points[index_1], points[index_2], points[index_3], points[index_4]
        ]
        return box, min(bounding_box[1])
```

## 6. 模型推理流程

如果不考虑 label，则其处理逻辑和训练逻辑有一点不一样，其把图片统一 resize 到指定的长度进行预测。

**数据预处理**
 数据预处理没有训练阶段的数据增强、随机裁剪和生成标签部分，但是会存在一个 resize 的操作，将宽高设置为 32 的倍数。

DetResizeForTest 步骤如下：
 1）对图片进行等比例的放缩，设置最大的尺寸为 960。如 3 张图片分别为 (720，1280)、(230，230)、(1150，720)。
 2）对图片进行放缩，以最大边为准（操作960的直接放缩到 960），缩小至能被 32 整除的最大尺寸。则输出：(512，960，3)、(224，224，3)、(960，576，3)。由于宽高都为 32 的倍数，则放缩的比例不统一，但尽可能相差不大。

后续的操作就与训练阶段一致，通过网络结构，然后进行后处理获得结果。



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

