//
// Created by DeathWind on 2023/5/14.
//

#ifndef GRABCUT_GRABCUTTER_H
#define GRABCUT_GRABCUTTER_H

#include <chrono>

#include "common.h"
#include "cut_alg/GMM.h"
#include "ui/UIController.h"

#include "graph.h"

typedef Graph<double, double, double> GraphType;

class GrabCutter {
public:
    /**
     * 默认不输出图像
     */
    GrabCutter();

    /**
     * 指定输出路径的构造函数，会生成图像输出
     * @param imageOutPutPath
     */
    explicit GrabCutter(const std::string &imageOutPutPath);

    /**
     * 对外接口
     * @param path 图片的全路径
     */
    void start(const std::string& path);

    /**
     * uiController在涂鸦时调用此方法进行对图像alpha的更新，
     * 让其为B_MUST和F_MUST
     * @param x 横坐标
     * @param y
     * @param aimAlpha 0表示背景，1表示前景
     */
    void modifyAlphaByUser(int x,int y,int aimAlpha);

    ~GrabCutter();

private:
    UIController uiController;
    ImageMat imageMat;
    double beta; //衡量对比度
    GMM bkGMM;
    GMM frGMM;

    GraphType* graph;

    //记录上次的能量
    double lastEnergy;

    /**
     * 通过画矩阵来进行初始训练
     */
    void initGrabCut();

    /**
     * 进一步的用户交互指定前景背景
     */
    void advanceGrabCut();

    /**
     * 把矩形以外的所有点变成B_MUST。注：默认为F_PROB
     * @param minX
     * @param minY
     * @param maxX
     * @param maxY
     */
    void updateMatByRect(int minX, int minY, int maxX, int maxY);

    /**
     * 使用kmeans初始化GMM并进行第零次（预）训练
     */
    void initGMM();

    /**
     * 计算β，以衡量对比度
     */
    void calculateBeta();

    /**
     * 开始训练
     * @param itTimes 迭代次数，不为正表示不限次数
     */
    void startGMM(int itTimes);

    /**
     * 生成流图，赋值好U和V
     */
    void generateGraph();

    /**
     * 计算两个像素之间的V
     * @param p1
     * @param p2
     * @return
     */
    double getVofPixels(Pixel &p1,Pixel &p2);
};


#endif //GRABCUT_GRABCUTTER_H
