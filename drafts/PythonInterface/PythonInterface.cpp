#include <PythonInterface.hpp>

void PythonAPI::Init()
{
    mtxGetPredict.lock();
}

bool PythonAPI::CapturePhoto()
{
    //Init Python model
    if(!Py_IsInitialized())
    {
        //自己编译的时候记得修改PythonHome
        Py_SetPythonHome(L"C:/Users/Rosetta/.conda/envs/Python7"); 
        Py_Initialize();
    }

    try
    {  
        mtxPython.lock();
        PyRun_SimpleString("import sys");
	    PyRun_SimpleString("from picamera import PiCamera");
        PyRun_SimpleString("camera = PiCamera()");
        PyRun_SimpleString("camera.resolution = (756,756)");
        PyRun_SimpleString("camera.framerate = 60");
        PyRun_SimpleString("camera.start_preview()");
        PyRun_SimpleString("camera.capture('Image.jpeg')");
        PyRun_SimpleString("camera.stop_preview()");
        PyRun_SimpleString("camera.close()");
        std::cout << "get Image " << std::endl;
        mtxPython.unlock();
        mtxGetPredict.unlock();
    }
    catch(...)
    {
        PyErr_Print();
        PyErr_Clear();
        Py_Finalize();
        return false;
    }
    return true;
}

int GetPredictImage()
{
    PythonAPI *pyapi = PythonAPI::get_instacne();

 //Init Python model
    if(!Py_IsInitialized())
    {
           Py_Initialize();
    }
    try
    {  
        pyapi->getMutex()->lock();
        pyapi->mtxPython.lock();
        PyRun_SimpleString("import sys");
	    PyRun_SimpleString("import os");
        PyRun_SimpleString("import socket");
        PyRun_SimpleString("import numpy as np");
        PyRun_SimpleString("import tensorflow as tf");
        PyRun_SimpleString("from tensorflow.keras.preprocessing import image");
        PyRun_SimpleString("model = tf.keras.models.load_model('./model_garbage_vgg16.h5')");
        pyapi->mtxPython.unlock();
        while(1)
        {
            std::cout << "wait mutex" << std::endl;
            pyapi->getMutex()->lock();
            std::cout << "get mutex" << std::endl;
            if(pyapi->getExit())
            {
                std::cout << "Close Model" << std::endl;
                break;
            }
            pyapi->mtxPython.lock();
            PyRun_SimpleString("filepath = './Image.jpeg'");
            PyRun_SimpleString("img = image.load_img(filepath,target_size=(150,150))");
            PyRun_SimpleString("x = np.expand_dims(image.img_to_array(img),axis=0)");
            PyRun_SimpleString("images = np.vstack([x])");
            PyRun_SimpleString("classes = model.predict(images,batch_size=10)");
            PyRun_SimpleString("acc =  np.argmax(classes)");
            PyRun_SimpleString("udpMsg = 'Classify ' + str(acc)");
            PyRun_SimpleString("client_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)");
            PyRun_SimpleString("PORT = 26601");
            PyRun_SimpleString("server_address = ('127.0.0.1', PORT)");
            PyRun_SimpleString("client_socket.sendto(udpMsg.encode(), server_address)");    
            PyRun_SimpleString("print(udpMsg)");    
            std::cout << "run success" << std::endl;
            _sleep(5);
            pyapi->mtxPython.unlock();
        }
        
    }
    catch(...)
    {
        PyErr_Print();
        PyErr_Clear();
        return true;
    }

    return true;
}