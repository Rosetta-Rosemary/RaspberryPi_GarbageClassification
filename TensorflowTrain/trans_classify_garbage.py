import os
from tensorflow.keras import layers
from tensorflow.keras import Model
from tensorflow.keras.applications.vgg16 import VGG16
from tensorflow.keras.applications.vgg19 import VGG19
from tensorflow.keras.preprocessing.image import ImageDataGenerator
import tensorflow as tf
import numpy as np

gpus = tf.config.experimental.list_physical_devices(device_type='GPU')
cpus = tf.config.experimental.list_physical_devices(device_type='CPU')
print(gpus, cpus)

tf.config.experimental.set_virtual_device_configuration(
    gpus[0],
    [tf.config.experimental.VirtualDeviceConfiguration(memory_limit=4096)]
)

batch_size = 32

model = VGG16(input_shape=(150,150,3),
                         include_top = True,
                         weights = None,
                         classes=10)
from tensorflow.keras.optimizers import RMSprop
model.compile(loss='categorical_crossentropy',optimizer=RMSprop(lr=0.0001),metrics=['acc'])

'''
loadModel = "./10/10_model_garbage_vgg16.h5"
model = tf.keras.models.load_model(loadModel)
'''
# model.summary()
base = './10/dataset/'
train_datagen = ImageDataGenerator(rescale=1./255.,
                                   rotation_range=40,
                                   width_shift_range=0.2,
                                   height_shift_range=0.2,
                                   shear_range=0.2,
                                   zoom_range=0.2,
                                   horizontal_flip=True,
                                   fill_mode='nearest')

validation_datagen = ImageDataGenerator(rescale=1./255.)
train_generator = train_datagen.flow_from_directory(base+'train/',
                                                    target_size=(150,150),
                                                    class_mode='categorical')
validation_generator = validation_datagen.flow_from_directory(base+'validation/',
                                                    target_size=(150,150),
                                                    class_mode='categorical')

print('--------------------------------------------')
print(train_generator.class_indices)   # 输出对应的标签文件夹
print('--------------------------------------------')

history = model.fit_generator(train_generator,
                              epochs=5,
                              validation_data=validation_generator,
                              verbose=1,
                              shuffle = True)

model.save('./10/1_10_model_garbage_vgg16.h5')

import matplotlib.pyplot as plt
loss = history.history['loss']
acc =  history.history['acc']
val_loss = history.history['val_acc']
val_acc = history.history['val_acc']
epochs = range(len(acc))
plt.plot(epochs,acc,'r',label='Training accuracy')
plt.plot(epochs,val_acc,'b',label='Validation accuracy')
plt.legend(loc=0)
plt.title("Training and Validation accuracy")
plt.figure()
plt.plot(epochs,loss,'r',label='Training loss')
plt.plot(epochs,val_loss,'b',label='Validation loss')
plt.legend(loc=0)
plt.title("Training and validation loss")
plt.show()