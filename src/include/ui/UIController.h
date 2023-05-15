//
// Created by DeathWind on 2023/5/14.
//

#ifndef GRABCUT_UICONTROLLER_H
#define GRABCUT_UICONTROLLER_H

#include <iostream>
#include <opencv2/opencv.hpp>
#include <unordered_map>
#include <vector>

#include "cut_alg/Pixel.h"

class UIController {
public:
    std::string imageName;
    cv::Mat srcImage;
    cv::Mat drawnImage;

    //矩形的对角坐标
    //x是横坐标！
    int posX1;
    int posY1;
    int posX2;
    int posY2;

    std::vector<std::vector<Pixel>> analyseImage();

    std::string getImagesFromPath(const std::string& path);
    void drawRect(const std::string& srcImageName);

private:
    static void mouseHandler(int event, int x, int y, int, void *);//x为横坐标

    //是否在拖动创建矩形
    bool isDragging;
};


#endif //GRABCUT_UICONTROLLER_H
