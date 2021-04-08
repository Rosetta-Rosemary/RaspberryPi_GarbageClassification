#ifndef _PYTHONINTERFACE_H_
#define __PYTHONINTERFACE_H_

#include <Python.h>
#include <iostream>
#include <mutex>
#include <thread>

class PythonAPI
{
public:
    static PythonAPI *get_instacne()
    {
        static PythonAPI pythonAPI;
        return &pythonAPI;
    };

    ~PythonAPI()
    {
        std::cout << "delete" << std::endl;
        Py_Finalize();
    };
    

    bool CapturePhoto();
    friend int GetPredictImage();

    bool getExit(){return bExit;};
    std::mutex *getMutex(){return &mtxGetPredict;};

    void ExitPython(){bExit = true;mtxGetPredict.unlock();}

    std::mutex mtxPython;
private:
    PythonAPI()
    {
        std::cout << "python api create" << std::endl; 
        Py_Initialize();
        std::cout << "python init" << std::endl;

    };
    PythonAPI(const PythonAPI &obj){};

    void Init();

    std::mutex mtxGetPredict;

    bool bExit = false;


};

#endif