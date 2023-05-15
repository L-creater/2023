from torch.utils.data import Dataset
from PIL import Image
from torchvision import transforms
 
import lmdb
import six
import cv2
 
class LmdbData(Dataset):
  def __init__(self, txt_path,maxlen,labeltool,transform = None, target_transform = None):
    super(LmdbData, self).__init__()
    self.labeltool=labeltool
    self.to_Tensor = transforms.ToTensor()
    self.maxlen=maxlen
    self.env = lmdb.open(txt_path,readonly=True)
    txn = self.env.begin()
    self.len = int(txn.get('num-samples'.encode()))
    # print("Read",self.len)
    self.transform = transform
    self.resize = transforms.Resize(32,120)
    self.target_transform = target_transform
  def __getitem__(self, index):
    assert index <= len(self), 'index range error'
    index += 1
    with self.env.begin(write=False) as txn:
        img_key = 'img_'+str(index)
        imgbuf = txn.get(img_key.encode('utf-8'))
 
        buf = six.BytesIO()
        buf.write(imgbuf)
        buf.seek(0)
        try:
            img = Image.open(buf).convert('RGB')
            # img.save('gao.jpg')
            # print(img)
 
            
        except IOError:
            print('Corrupted image for %d' % index)
            return self[index + 1]
        # print("PPP")
        label_key = 'lab_'+str(index)
        label = txn.get(label_key.encode()).decode()
 
    return img, label
  def __len__(self):
    return self.len