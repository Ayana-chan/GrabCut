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

class GrabCutter;

class UIController {
public:
    GrabCutter* master; //回指所属的GrabCutter
    std::string imageName;
    cv::Mat srcImage;

    cv::Mat rectedImage;//画了矩形的图
    cv::Mat drawnImage;//用户交互涂鸦的图

    //用户绘制的矩形
    cv::Rect rect;
    //矩形的对角坐标
    //x是横坐标！
    int posX1;
    int posY1;
    int posX2;
    int posY2;

    UIController(GrabCutter* grabCutter);

    std::string getImagesFromPath(const std::string &path);

    /**
     * 将自己所存储的图片解析成二维Pixel数组
     * @param aimVec 存储结果值的二维Pixel数组
     */
    void analyseImage(std::vector<std::vector<Pixel>> &aimVec);

    /**
     * 要求用户在图上绘制矩形
     * 会发生阻塞
     * @param srcImageName
     */
    void drawRect(const std::string &srcImageName);

    /**
     * 要求用户进一步指定前景背景
     * 会发生阻塞
     */
    void additionalDrawImage();

private:
    //暂时存放矩形的第一个点，使得出现非法操作时可以回滚
    int tempPosX1;
    int tempPosY1;

    //鼠标是否在拖动状态（画矩形）
    bool isDragging;

    //鼠标左键或右键是否在拖动状态
    bool isDraggingLeft;
    bool isDraggingRight;

    static void rectMouseHandler(int event, int x, int y, int, void *);//x为横坐标
    static void advanceMouseHandler(int event, int x, int y, int, void *);//x为横坐标
};


#endif //GRABCUT_UICONTROLLER_H
