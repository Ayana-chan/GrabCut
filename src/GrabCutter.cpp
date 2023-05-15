//
// Created by DeathWind on 2023/5/14.
//

#include "GrabCutter.h"
#include "cut_alg/Pixel.h"
#include "ui/UIController.h"
#include "util/OutputSwitcher.h"

using namespace std;

void GrabCutter::start() {
    OutputSwitcher outputSwitcher;
    outputSwitcher.switchOutputToFile(R"(D:\Code\C\clionCpp\GrabCut\output\output.txt)");

    RGB rgb(1,2,3);
    UIController uiController;
    int num=uiController.getImagesFromDirectory(R"(D:\Code\C\clionCpp\GrabCut\test_textures)");
    cout<<"Total "<<num<<" Image"<<endl;

//    uiController.initInteractivePage();

    auto pixels = UIController::analyseImage(uiController.imageMap["lm1.jpeg"]);
    cout<<"Image Size: "<<pixels.size()<<"*"<<pixels[0].size()<<endl;
    for(int i=0;i<pixels.size();i++){
        for(int j=0;j<pixels[0].size();j++){
            std::cout<<pixels[i][j];
        }
        std::cout<<std::endl;
    }
}
