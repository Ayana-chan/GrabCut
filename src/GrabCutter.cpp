//
// Created by DeathWind on 2023/5/14.
//

#include "GrabCutter.h"
#include "cut_alg/Pixel.h"
#include "ui/UIController.h"
#include "util/OutputSwitcher.h"

using namespace std;

void GrabCutter::start() {
    //更改cout输出目的地
//    OutputSwitcher outputSwitcher;
//    outputSwitcher.switchOutputToFile(R"(D:\Code\C\clionCpp\GrabCut\output\output.txt)");

    //输入图像

    std::string imageFileName = "lm1.jpeg";
    this->uiController.getImagesFromPath(R"(D:\Code\C\clionCpp\GrabCut\test_textures\)" + imageFileName);

    this->uiController.analyseImage(this->imageMat);
    cout << "Image Size: " << this->imageMat.size() << "*" << this->imageMat[0].size() << endl;

    //矩形输入与初始化

    this->uiController.drawRect(this->uiController.imageName);
    updateMatByRect(this->uiController.posX1, this->uiController.posY1,
                    this->uiController.posX2, this->uiController.posY2);
    //TODO 使用kmeans初始化GMM模型

}

void GrabCutter::updateMatByRect(int minX, int minY, int maxX, int maxY) {
    for (int i = 0; i < this->imageMat.size(); i++) {
        if(i>minX && i<maxX){
            continue;
        }
        for (int j = 0; j < this->imageMat[0].size(); j++) {
            if(j>minY && j<maxY){
                continue;
            }
            this->imageMat[i][j].alpha=PixelBelongEnum::B_MUST;
        }
    }
}




