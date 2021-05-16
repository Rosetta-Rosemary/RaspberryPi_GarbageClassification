import tensorflow as tf
import os
from tensorflow.keras.preprocessing.image import ImageDataGenerator
base = './dataset/'
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
model = tf.keras.Sequential([
    tf.keras.layers.Conv2D(64,(3,3),activation='relu',input_shape=(150,150,3)),
    tf.keras.layers.MaxPool2D(2,2),
    tf.keras.layers.Conv2D(64,(3,3),activation='relu'),
    tf.keras.layers.MaxPool2D(2,2),
    tf.keras.layers.Conv2D(128,(3,3),activation='relu'),
    tf.keras.layers.MaxPool2D(2,2),
    tf.keras.layers.Conv2D(128,(3,3),activation='relu'),
    tf.keras.layers.MaxPool2D(2,2),
    tf.keras.layers.Flatten(),
    tf.keras.layers.Dropout(0.5),
    tf.keras.layers.Dense(512,activation='relu'),
    tf.keras.layers.Dense(4,activation='softmax')
])
model.summary()
from tensorflow.keras.optimizers import RMSprop
model.compile(loss='categorical_crossentropy',optimizer=RMSprop(lr=0.0001),metrics=['acc'])
history = model.fit_generator(train_generator,
                              epochs=100,
                              validation_data=validation_generator,
                              verbose=2)
model.save('./model_garbage.h5')
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

