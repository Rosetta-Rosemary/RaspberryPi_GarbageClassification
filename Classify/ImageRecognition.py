import sys
import os
import numpy as np
import tensorflow as tf
from tensorflow.keras.preprocessing import image
model = tf.keras.models.load_model('./model_garbage_vgg16.h5')

def ImageRecognition(FileName):
    filepath = "./" + FileName
    img = image.load_img(filepath,target_size=(150,150))
    x = np.expand_dims(image.img_to_array(img),axis=0)
    images = np.vstack([x])
    classes = model.predict(images,batch_size=10)
    acc = np.argmax(classes)
    return acc