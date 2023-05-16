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
    int tempPosX1;
    int tempPosY1;
    int posX1;
    int posY1;
    int posX2;
    int posY2;

    /**
     * 将自己所存储的图片解析成二维Pixel数组
     * @param aimVec 存储结果值的二维Pixel数组
     */
    void analyseImage(std::vector<std::vector<Pixel>> &aimVec);

    std::string getImagesFromPath(const std::string &path);

    void drawRect(const std::string &srcImageName);

private:
    static void mouseHandler(int event, int x, int y, int, void *);//x为横坐标

    //是否在拖动创建矩形
    bool isDragging;
};


#endif //GRABCUT_UICONTROLLER_H
