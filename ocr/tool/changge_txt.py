import re

ff = open('/home/ly/demo/ocr/CRNN/data/labels/test1.txt','w')  #打开一个文件，可写模式
with open('/home/ly/demo/ocr/CRNN/data/labels/test.txt','r') as f:  #打开一个文件只读模式
    line = f.readlines()
    i = 0
    for line_list in line:
        line_new =line_list.replace('\n','')  #将换行符替换为空('')
        print(line_new)
        line_new = str(line_new) #主要是这一步 将之前列表数据转为str才能加入列表
        line_new1 = line_new[:-4]
        c = re.sub('[\d+_]', '', line_new1)
        line_new = line_new +' '+ c +'\n'
        i += 1
        print(line_new)
        ff.write(line_new) #写入一个新文件中
