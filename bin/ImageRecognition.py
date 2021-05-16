import os
import numpy as np
from tensorflow.keras.preprocessing import image
import tensorflow as tf 
from model import resnet50
import os
import json
from PIL import Image
import glob
import sys
from picamera import PiCamera

im_height = 224
im_width = 224
num_classes = 20

feature = resnet50(num_classes=num_classes, include_top=False)
feature.trainable = False
model = tf.keras.Sequential([feature,
                             tf.keras.layers.GlobalAvgPool2D(),
                             tf.keras.layers.Dropout(rate=0.5),
                             tf.keras.layers.Dense(1024, activation="relu"),
                             tf.keras.layers.Dropout(rate=0.5),
                             tf.keras.layers.Dense(num_classes),
                             tf.keras.layers.Softmax()])

weights_path = './save_weights/ResNet50.h5'
assert len(glob.glob(weights_path+"*")), "cannot find {}".format(weights_path)
model.load_weights(weights_path)

def ImageRecognition(img_path = "./Image.jpg"):
    # load image
    assert os.path.exists(img_path), "file: '{}' dose not exist.".format(img_path)
    img = Image.open(img_path)
    # resize image to 224x224
    img = img.resize((im_width, im_height))
    _R_MEAN = 123.68
    _G_MEAN = 116.78
    _B_MEAN = 103.94
    img = np.array(img).astype(np.float32)
    img = img - [_R_MEAN, _G_MEAN, _B_MEAN]
    img = (np.expand_dims(img, 0))
    result = np.squeeze(model.predict(img))
    predict_class = np.argmax(result)
    return predict_class

def TakePicture():
    camera = PiCamera()   
    camera.resolution = (756,756)
    camera.framerate = 60
    camera.start_preview()
    camera.capture('Image.jpeg')
    camera.stop_preview()
    camera.close()