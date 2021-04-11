def ImageRecognition(FileName):
    filepath = "./" + FileName
    img = image.load_img(filepath,target_size=(150,150))
    x = np.expand_dims(image.img_to_array(img),axis=0)
    images = np.vstack([x])
    classes = model.predict(images,batch_size=10)
    acc = np.argmax(classes)
    return acc

def add(a):
    return a + 1