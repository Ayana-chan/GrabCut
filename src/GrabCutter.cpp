//
// Created by DeathWind on 2023/5/14.
//

#include "GrabCutter.h"
#include "util/OutputSwitcher.h"
#include "util/ImageOutputer.h"

#define KMEANS_IT_TIMES 5

using namespace std;

GrabCutter::GrabCutter() {
    bkGMM=GMM(5);
    frGMM=GMM(5);
}

void GrabCutter::start() {
    //更改cout输出目的地
//    OutputSwitcher outputSwitcher;
//    outputSwitcher.switchOutputToFile(R"(D:\Code\C\clionCpp\GrabCut\output\output.txt)");

    //输入图像

    std::string imageFileName = "lm1.jpeg";
    uiController.getImagesFromPath(R"(D:\Code\C\clionCpp\GrabCut\test_textures\)" + imageFileName);

    uiController.analyseImage(imageMat);
    cout << "Image Size: " << imageMat.size() << "*" << imageMat[0].size() << endl;

    //矩形输入与初始化

    uiController.drawRect(uiController.imageName);
    updateMatByRect(uiController.posX1, uiController.posY1,
                    uiController.posX2, uiController.posY2);
    initGMM();
    ImageOutputer::generateImage(imageMat);
}

void GrabCutter::updateMatByRect(int minX, int minY, int maxX, int maxY) {
    for (int i = 0; i < imageMat.size(); i++) {
        if(i>minX && i<maxX){
            continue;
        }
        for (int j = 0; j < imageMat[0].size(); j++) {
            if(j>minY && j<maxY){
                continue;
            }
            imageMat[i][j].alpha=PixelBelongEnum::B_MUST;
        }
    }
}

void GrabCutter::initGMM() {
    vector<Pixel*> frSamples;
    vector<Pixel*> bkSamples;
    for(auto & v : imageMat){
        for(auto & p : v){
            if(p.alpha==PixelBelongEnum::B_MUST || p.alpha==PixelBelongEnum::B_PROB){
                bkSamples.push_back(&p);
            }else{
                frSamples.push_back(&p);
            }
        }
    }

    std::cout<<"initByKmeans bkGMM with " << bkSamples.size()<< " bkSamples"<<std::endl;
    bkGMM.initByKmeans(bkSamples,KMEANS_IT_TIMES);
    std::cout<<"initByKmeans frGMM with "<< frSamples.size()<< " frSamples"<<std::endl;
    frGMM.initByKmeans(frSamples,KMEANS_IT_TIMES);
}






