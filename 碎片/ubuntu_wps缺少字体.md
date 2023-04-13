方法一：解压到 WPS的默认字体文件夹中(wps-office)，然后重启WPS即可

```bash
sudo unzip wps_symbol_fonts.zip -d /usr/share/fonts/wps-office

```

tips：如果`/usr/share/fonts/`目录下没有`wps-office`文件夹，则使用方法二。

方法二：

- 将字体库文件解压

```bash
sudo unzip wps_symbol_fonts.zip		# 解压zip文件
```

- 更新字体

```bash
# 生成字体索引
sudo mkfontscale
sudo mkfontdir
# 更新字体缓存
sudo fc-cache
```

- 重新打开WPS，提示不再出现。

方法三：图形化操作

- 点击zip文件，鼠标右键 -> 解压，解压后会得到一些tff文件
- 双击点开tff文件，点击安装（全部的tff文件都操作一遍即可）

# 字体文件在目录下