import os
import numpy as np
from tensorflow.keras.preprocessing import image
import tensorflow as tf
model = tf.keras.models.load_model("./model_garbage_vgg16.h5")
filepath = './test_data/'
keys = os.listdir(filepath)
print(keys)
for fn in keys:
    path = filepath + fn
    img = image.load_img(path,target_size=(150,150))
    x = image.img_to_array(img)
    x = np.expand_dims(x,axis=0)
    images = np.vstack([x])
    classes = model.predict(images,batch_size=10)
    print(fn+" result:",classes)#饭是第一个 水果第二个 玻璃第三个 电池第四个
    result = np.argmax(classes)
    if result == 0:
        print(fn+" is 剩饭")
    elif result == 1:
        print(fn+" is 水果")
    elif result == 2:
        print(fn+" is 玻璃")
    elif result == 3:
        print(fn +" is 电池")
