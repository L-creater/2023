# [conda](https://conda.io/projects/conda/en/latest/commands/create.html)

## conda creat  --name  yolov8

## 位置参数[](https://conda.io/projects/conda/en/latest/commands/create.html#Positional Arguments)

- package_spec

  在 conda 环境中安装或更新的包列表。

## 选项[](https://conda.io/projects/conda/en/latest/commands/create.html#options)

- --clone

  创建一个新环境作为现有本地环境的副本。

- --file

  从给定文件中读取包版本。可以传递重复的文件规范（例如 --file=file1 --file=file2）。

- --dev

  在包装器脚本中使用sys.executable -m conda而不是 CONDA_EXE。这主要用于在我们针对旧 Python 版本测试新 conda 源的测试期间使用。

## 目标环境规范[](https://conda.io/projects/conda/en/latest/commands/create.html#Target Environment Specification)

- -n, --name

  环境名称。

- -p, --prefix

  环境位置的完整路径（即前缀）。

### 进入已有的环境

conda activate 环境名





## 打包环境

安装conda-pack包

```bash
conda install -c conda-forge conda-pack
pip install conda-pack
```

将名字为 my_env 的虚拟环境 打包为 my_env.tar.gz（默认就是这形式）
conda pack -n my_env

\# -o 就是给导出的虚拟环境重新命名，所以导出来的虚拟环境名字为 out_name.tar.gz
conda pack -n my_env -o out_name.tar.gz

\# 指定虚拟环境包的输出路径
conda pack -p  /explicit/path/to/my_env



## 进入打包的环境

source activate tensorflow2.4

conda activate 环境名

部署新环境报错时，删除.bashrc 对应的提示   conda  语句