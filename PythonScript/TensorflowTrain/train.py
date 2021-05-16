import os
from tensorflow.keras import layers
from tensorflow.keras import Model
from tensorflow.keras.applications.vgg16 import VGG16
from tensorflow.keras.preprocessing.image import ImageDataGenerator
import tensorflow as tf

gpus = tf.config.experimental.list_physical_devices(device_type='GPU')
cpus = tf.config.experimental.list_physical_devices(device_type='CPU')
print(gpus, cpus)

tf.config.experimental.set_virtual_device_configuration(
    gpus[0],
    [tf.config.experimental.VirtualDeviceConfiguration(memory_limit=4096)]
)

'''
model = VGG16(input_shape=(150,150,3),
                         include_top = True,
                         weights = None,
                         classes=7)
'''
model = tf.keras.models.load_model('./new_model_garbage_vgg16.h5')

base = './7/dataset/'
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

from tensorflow.keras.optimizers import RMSprop
model.compile(loss='categorical_crossentropy',optimizer=RMSprop(lr=0.0001),metrics=['acc'])


history = model.fit_generator(train_generator,
                              epochs=40,
                              validation_data=validation_generator,
                              verbose=1)

model.save('./new_model_garbage_vgg16.h5')