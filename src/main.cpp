#include <iostream>
#include <fstream>

#include "cut_alg/Pixel.h"
#include "ui/UIController.h"
#include "util/OutputSwitcher.h"

int main() {
    std::cout << "Hello, World!" << std::endl;

    OutputSwitcher outputSwitcher;
//    outputSwitcher.switchOutputToFile(R"(D:\Code\C\clionCpp\GrabCut\output\output.txt)");

    RGB rgb(1,2,3);
    UIController uiController;
    int num=uiController.getImagesFromDirectory(R"(D:\Code\C\clionCpp\GrabCut\test_textures)");
    std::cout<<num<<std::endl;

    return 0;
}
