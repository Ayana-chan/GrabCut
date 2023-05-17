//
// Created by DeathWind on 2023/5/14.
//

#include "GrabCutter.h"
#include "util/OutputSwitcher.h"
#include "util/ImageOutputer.h"

//GMM的component数量（K）
#define GMM_K 5
//kmeans的迭代次数
#define KMEANS_IT_TIMES 3

using namespace std;

GrabCutter::GrabCutter() : bkGMM(GMM(GMM_K)), frGMM(GMM(GMM_K)) {}

void GrabCutter::start(std::string path) {
    //更改cout输出目的地
    OutputSwitcher outputSwitcher;
    outputSwitcher.switchOutputToFile(R"(D:\Code\C\clionCpp\GrabCut\output\output.txt)");

    //输入图像

    uiController.getImagesFromPath(path);
    uiController.analyseImage(imageMat);
    cout << "Image Size: " << imageMat.size() << "*" << imageMat[0].size() << endl;

    //矩形输入与初始化
    std::cout<<std::endl;
    uiController.drawRect(uiController.imageName);
    updateMatByRect(uiController.posX1, uiController.posY1,
                    uiController.posX2, uiController.posY2);
    std::cout<<std::endl;
    initGMM();

//    ImageOutputer::generateTenColorImage(imageMat);
}

void GrabCutter::updateMatByRect(int minX, int minY, int maxX, int maxY) {
    for (int i = 0; i < imageMat.size(); i++) {
        for (int j = 0; j < imageMat[0].size(); j++) {
            if (i <= minY || i >= maxY || j <= minX || j >= maxX) {
                imageMat[i][j].alpha = PixelBelongEnum::B_MUST;
            }
        }
    }
}

void GrabCutter::initGMM() {
    vector<Pixel *> frSamples;
    vector<Pixel *> bkSamples;
    for (auto &v : imageMat) {
        for (auto &p : v) {
            if (p.alpha == PixelBelongEnum::B_MUST || p.alpha == PixelBelongEnum::B_PROB) {
                bkSamples.push_back(&p);
            } else {
                frSamples.push_back(&p);
            }
        }
    }

    std::cout << "initByKmeans bkGMM with " << bkSamples.size() << " bkSamples" << std::endl;
    bkGMM.initByKmeans(bkSamples, KMEANS_IT_TIMES);
    std::cout << "initByKmeans frGMM with " << frSamples.size() << " frSamples" << std::endl;
    frGMM.initByKmeans(frSamples, KMEANS_IT_TIMES);

    //训练一次来生成方差等
    bkGMM.train();
    frGMM.train();
}






