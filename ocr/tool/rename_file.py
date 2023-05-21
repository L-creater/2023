

import os

path = '/home/ly/demo/ocr/ocr_data/data/images'

# 获取该目录下所有文件，存入列表中
fileList = os.listdir(path)

n = 0
for i in fileList:
    # 设置旧文件名（就是路径+文件名）
    oldname = path + os.sep + fileList[n]  # os.sep添加系统分隔符

    name_ = fileList[n]
    word = name_[ :-4]
    # 设置新文件名
    # newname = path + os.sep + 'a' + str(n + 1) + '.JPG'
    newname = path + os.sep + word + '.png'
    os.rename(oldname, newname)  # 用os模块中的rename方法对文件改名
    print(oldname, '======>', newname)

    n += 1