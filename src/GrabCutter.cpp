//
// Created by DeathWind on 2023/5/14.
//

#include "GrabCutter.h"
#include "cut_alg/Pixel.h"
#include "ui/UIController.h"
#include "util/OutputSwitcher.h"

using namespace std;

GrabCutter::GrabCutter() {
    this->uiController = *new UIController();
}

GrabCutter::~GrabCutter() {
    delete (&this->uiController);
}

void GrabCutter::start() {
//    OutputSwitcher outputSwitcher;
//    outputSwitcher.switchOutputToFile(R"(D:\Code\C\clionCpp\GrabCut\output\output.txt)");

    uiController.getImagesFromPath(R"(D:\Code\C\clionCpp\GrabCut\test_textures\lm1.jpeg)");

    auto pixels = uiController.analyseImage();
    cout << "Image Size: " << pixels.size() << "*" << pixels[0].size() << endl;
//    for(int i=0;i<pixels.size();i++){
//        for(int j=0;j<pixels[0].size();j++){
//            std::cout<<pixels[i][j];
//        }
//        std::cout<<std::endl;
//    }

    uiController.drawRect(uiController.imageName);
}


