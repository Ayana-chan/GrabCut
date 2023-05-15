//
// Created by DeathWind on 2023/5/14.
//

#include "GrabCutter.h"
#include "cut_alg/Pixel.h"
#include "ui/UIController.h"
#include "util/OutputSwitcher.h"

void GrabCutter::start() {
    OutputSwitcher outputSwitcher;
//    outputSwitcher.switchOutputToFile(R"(D:\Code\C\clionCpp\GrabCut\output\output.txt)");

    RGB rgb(1,2,3);
    UIController uiController;
    int num=uiController.getImagesFromDirectory(R"(D:\Code\C\clionCpp\GrabCut\test_textures)");
    std::cout<<"Total "<<num<<" Image"<<std::endl;

    uiController.initInteractivePage();
}
