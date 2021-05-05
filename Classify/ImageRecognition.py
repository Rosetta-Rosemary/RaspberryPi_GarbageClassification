import os
import numpy as np
from tensorflow.keras.preprocessing import image
import tensorflow as tf 
model = tf.keras.models.load_model("./model_garbage_vgg16.h5")
import sys
from picamera import PiCamera

def ImageRecognition():
    filepath = "./Image.jpeg"
    img = image.load_img(filepath,target_size=(150,150))
    x = np.expand_dims(image.img_to_array(img),axis=0)
    images = np.vstack([x])
    classes = model.predict(images,batch_size=10)
    acc = np.argmax(classes)
    return acc

def TakePicture():
    camera = PiCamera()   
    camera.resolution = (756,756)
    camera.framerate = 60
    camera.start_preview()
    camera.capture('Image.jpeg')
    camera.stop_preview()
    camera.close()