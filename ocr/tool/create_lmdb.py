import re
import lmdb
import cv2
import numpy as np
import os



# # OUT_PATH = '/home/ly/demo/ocr/ocr_data/CRNN_DATA/test_lmdb'
# OUT_PATH = '/home/ly/demo/ocr/ocr_data/CRNN_DATA/train_lmdb'

# IN_PATH = '/home/ly/demo/ocr/ocr_data/train.txt'
# # IN_PATH = '/home/ly/demo/ocr/ocr_data/test.txt'
# PREFIX = '/home/ly/demo/ocr/ocr_data/CRNN_DATA/images'

OUT_PATH = '/home/ly/demo/ocr/ocr_data/minidata/train'
PREFIX = '/home/ly/demo/ocr/CRNN/data/images'
LABEL_PATH = '/home/ly/demo/ocr/CRNN/data/labels/train1.txt'


def checkImageIsValid(imageBin):
    if imageBin is None:
        return False
    try:
        imageBuf = np.fromstring(imageBin, dtype=np.uint8)
        img = cv2.imdecode(imageBuf, cv2.IMREAD_GRAYSCALE)
        imgH, imgW = img.shape[0], img.shape[1]
    except:
        return False
    else:
        if imgH * imgW == 0:
            return False
    return True


def writeCache(env, cache):
    with env.begin(write=True) as txn:
        for k, v in cache.items():
            if isinstance(v, bytes):
                txn.put(k.encode(), v) # 添加数据和键值
            elif isinstance(v, str):
                txn.put(k.encode(), v.encode())


def createDataset(outputPath, imagePathList, labelList, lexiconList=None, checkValid=True):
    """
    Create LMDB dataset for CRNN training.
    ARGS:
        outputPath    : LMDB output path
        imagePathList : list of image path
        labelList     : list of corresponding groundtruth texts
        lexiconList   : (optional) list of lexicon lists
        checkValid    : if true, check the validity of every image
    """
    assert (len(imagePathList) == len(labelList))
    nSamples = len(imagePathList)
    env = lmdb.open(outputPath, map_size=1099511627776)
    cache = {}
    cnt = 1
    for i in range(nSamples):
        imagePath = os.path.join(PREFIX, imagePathList[i]).split()[0].replace('\n', '').replace('\r\n', '')
        print(imagePath)
        label = ''.join(labelList[i])
        print(label)
        # if not os.path.exists(imagePath):
        #     print('%s does not exist' % imagePath)
        #     continue

        with open(imagePath, 'rb') as f:
            imageBin = f.read()

        if checkValid:
            if not checkImageIsValid(imageBin):
                print('%s is not a valid image' % imagePath)
                continue
        imageKey = 'image-%09d' % cnt
        labelKey = 'label-%09d' % cnt
        cache[imageKey] = imageBin
        cache[labelKey] = label
        if lexiconList:
            lexiconKey = 'lexicon-%09d' % cnt
            cache[lexiconKey] = ' '.join(lexiconList[i])
        if cnt % 1000 == 0:
            writeCache(env, cache)
            cache = {}
            print('Written %d / %d' % (cnt, nSamples))
        cnt += 1
        print(cnt)
    nSamples = cnt - 1
    cache['num-samples'] = str(nSamples)
    writeCache(env, cache)
    print('Created dataset with %d samples' % nSamples)


if __name__ == '__main__':
    outputPath = OUT_PATH
    if not os.path.exists(OUT_PATH):
        os.mkdir(OUT_PATH)
    imgdata = open(LABEL_PATH)
    imagePathList = list(imgdata)

    labelList = []
    for line in imagePathList:
        word = line.split()[1]
        # word = re.sub('[\d+_]', '', word)
        # word = word[ :-4]
        labelList.append(word)
    createDataset(outputPath, imagePathList, labelList)

