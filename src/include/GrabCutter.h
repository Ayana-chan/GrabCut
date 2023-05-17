//
// Created by DeathWind on 2023/5/14.
//

#ifndef GRABCUT_GRABCUTTER_H
#define GRABCUT_GRABCUTTER_H

#include "common.h"
#include "cut_alg/GMM.h"
#include "ui/UIController.h"

#include "graph.h"

typedef Graph<double, double, double> GraphType;

class GrabCutter {
public:
    GrabCutter();

    void start(std::string path);

    ~GrabCutter();

private:
    UIController uiController;
    ImageMat imageMat;
    double beta; //衡量对比度
    GMM bkGMM;
    GMM frGMM;

    GraphType* graph;

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

    double getVofPixels(Pixel &p1,Pixel &p2);
};


#endif //GRABCUT_GRABCUTTER_H
