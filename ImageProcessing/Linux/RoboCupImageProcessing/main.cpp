#include "MainWorkingFlow.hpp"
#include "Configs.hpp"

int main()
{
    using namespace RoboCup;
    MainWorkingFlow app(Configs(R"(E:\project\RoboCup\ImageProcessing\Linux\RoboCupImageProcessing\out\build\Windows-Debug\configs.yml)"));
    app.Run();
    return 0;
}
