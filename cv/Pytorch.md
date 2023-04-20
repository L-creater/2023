# Pytorch

## flatten

1. 无参数时，按一维展开
2. 有参数时，参数之前的唯度保持不变，后面全部展开为一维
3. 参数 0，1，2...、

## [Conv2d](https://pytorch.org/docs/stable/generated/torch.nn.Conv2d.html#torch.nn.Conv2d)

```python
torch.nn.Conv2d(*in_channels*, *out_channels*, *kernel_size*, *stride=1*, *padding=0*, *dilation=1*, *groups=1*, *bias=True*, *padding_mode='zeros'*, *device=None*, *dtype=None*)
```

1. in_channels  --通道大小
2. out_channels --滤波器数量
3. kernel_size --卷积核大小

## [Module](https://pytorch.org/docs/stable/generated/torch.nn.Module.html?highlight=module#torch.nn.Module)

