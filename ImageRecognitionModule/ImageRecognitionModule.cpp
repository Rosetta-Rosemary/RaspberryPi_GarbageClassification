#include <ModelControl.hpp>

int GetPredictImage();

void InitCtrl()
{
    ModelControl *Ctrl = ModelControl::get_instacne();    
}

int main()
{
    std::thread threadCtrl(InitCtrl);
    std::thread Predict(GetPredictImage);
    std::cout << "Running" << std::endl;
    _sleep(120);
    std::cout << "Sleep for 120s Over and Exit" << std::endl;

    return 0;
}