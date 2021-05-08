import os
import glob
import json

import tensorflow as tf
from tensorflow.keras.preprocessing.image import ImageDataGenerator
from tqdm import tqdm
from tensorflow.keras import optimizers
from tensorflow.keras import callbacks

from model import resnet50


Devices = "0"
os.environ["CUDA_VISIBLE_DEVICES"]=Devices

if Devices == "0":
    gpus = tf.config.experimental.list_physical_devices(device_type='GPU')
    cpus = tf.config.experimental.list_physical_devices(device_type='CPU')
    print(gpus, cpus)
    tf.config.experimental.set_virtual_device_configuration(
        gpus[0],
        [tf.config.experimental.VirtualDeviceConfiguration(memory_limit=4096)]
    )

def showTable(history):
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



def main():
    data_root = os.path.abspath(os.path.join(os.getcwd(), "../"))  # get data root path
    image_path = os.path.join(data_root, "dataset", "data")  # flower data set path
    train_dir = os.path.join(image_path, "train")
    validation_dir = os.path.join(image_path, "val")
    assert os.path.exists(train_dir), "cannot find {}".format(train_dir)
    assert os.path.exists(validation_dir), "cannot find {}".format(validation_dir)

 # create direction for saving weights
    if not os.path.exists("save_weights"):
        os.makedirs("save_weights")

    im_height = 224
    im_width = 224
    batch_size = 16
    epochs = 50
    num_classes = 20

    _R_MEAN = 123.68
    _G_MEAN = 116.78
    _B_MEAN = 103.94

    def pre_function(img):
        img = img - [_R_MEAN, _G_MEAN, _B_MEAN]

        return img

    train_image_generator = ImageDataGenerator(horizontal_flip=True,
                                               preprocessing_function=pre_function)

    validation_image_generator = ImageDataGenerator(preprocessing_function=pre_function)

    train_data_gen = train_image_generator.flow_from_directory(directory=train_dir,
                                                               batch_size=batch_size,
                                                               shuffle=True,
                                                               target_size=(im_height, im_width),
                                                               class_mode='categorical')
    total_train = train_data_gen.n

    # get class dict
    class_indices = train_data_gen.class_indices

    # transform value and key of dict
    inverse_dict = dict((val, key) for key, val in class_indices.items())
    # write dict into json file
    json_str = json.dumps(inverse_dict, indent=4)
    with open('class_indices.json', 'w') as json_file:
        json_file.write(json_str)

    val_data_gen = validation_image_generator.flow_from_directory(directory=validation_dir,
                                                                  batch_size=batch_size,
                                                                  shuffle=False,
                                                                  target_size=(im_height, im_width),
                                                                  class_mode='categorical')
    # img, _ = next(train_data_gen)
    total_val = val_data_gen.n
    print("using {} images for training, {} images for validation.".format(total_train,
                                                                           total_val))

    feature = resnet50(num_classes=num_classes, include_top=False)
    # 加载权重
    pre_weights_path = './pretrain_weights.ckpt'
    assert len(glob.glob(pre_weights_path+"*")), "cannot find {}".format(pre_weights_path)
    feature.load_weights(pre_weights_path)
    feature.trainable = False
    feature.summary()
    
    model = tf.keras.Sequential([feature,
                                 tf.keras.layers.GlobalAvgPool2D(),
                                 tf.keras.layers.Dropout(rate=0.5),
                                 tf.keras.layers.Dense(1024, activation="relu"),
                                 tf.keras.layers.Dropout(rate=0.5),
                                 tf.keras.layers.Dense(num_classes),
                                 tf.keras.layers.Softmax()])
    model.summary()

    callback_list = [
        callbacks.ReduceLROnPlateau(
            monitor='val_loss',
            factor=0.1,
            patience=3,
            verbose=1,
        ),
        callbacks.ModelCheckpoint(
            filepath='./save_weights/ResNet50.h5',
            monitor='val_acc',
            save_best_only=True,
            save_weights_only=True, 
            verbose=1
        )
    ]

    model.compile(loss='categorical_crossentropy',
        optimizer=optimizers.Adam(lr=0.0002),
        metrics=['acc'])

    history = model.fit_generator(
        train_data_gen,
        steps_per_epoch=64,
        epochs=epochs,
        validation_data=val_data_gen,
        callbacks=callback_list,
        verbose=1)

    showTable(history)

    

if __name__ == '__main__':
    main()
