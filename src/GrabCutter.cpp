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
//β的最小值，小于此值视作0
#define MINIMUM_BETA 0.00001

using namespace std;

GrabCutter::GrabCutter() : bkGMM(GMM(GMM_K)), frGMM(GMM(GMM_K)) {
    graph= nullptr;
}

GrabCutter::~GrabCutter() {
    delete(graph);
}

void GrabCutter::start(std::string path) {
    //更改cout输出目的地
//    OutputSwitcher outputSwitcher;
//    outputSwitcher.switchOutputToFile(R"(D:\Code\C\clionCpp\GrabCut\output\output.txt)");

    //输入图像

    uiController.getImagesFromPath(path);
    uiController.analyseImage(imageMat);
    cout << "Image Size: " << imageMat.size() << "*" << imageMat[0].size() << endl;

    //矩形输入与初始化

    std::cout << std::endl;
    uiController.drawRect(uiController.imageName);
    updateMatByRect(uiController.posX1, uiController.posY1,
                    uiController.posX2, uiController.posY2);
    std::cout << std::endl;
    initGMM();
    //ImageOutputer::generateTenColorImage(imageMat);

    //迭代训练

    calculateBeta();
    startGMM(1);

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

void GrabCutter::calculateBeta() {

    double totalDistance = 0;

    auto rows = imageMat.size();
    auto cols = imageMat[0].size();

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            //每个像素都分为4个方向来匹配邻居，这样所有像素计算完毕后就能覆盖所有邻居关系
            //right-up
            if (i > 0 && j < cols - 1) {
                totalDistance += imageMat[i][j].rgb.calDistanceTo(imageMat[i - 1][j + 1].rgb);
            }
            //right
            if (j < cols - 1) {
                totalDistance += imageMat[i][j].rgb.calDistanceTo(imageMat[i][j + 1].rgb);
            }
            //right-down
            if (i < rows - 1 && j < cols - 1) {
                totalDistance += imageMat[i][j].rgb.calDistanceTo(imageMat[i + 1][j + 1].rgb);
            }
            //down
            if (i < rows - 1) {
                totalDistance += imageMat[i][j].rgb.calDistanceTo(imageMat[i + 1][j].rgb);
            }
        }
    }

    //纯色图的累计距离极小，可能导致溢出
    if (totalDistance < MINIMUM_BETA) {
        totalDistance = MINIMUM_BETA;
    }

    double edgeNum = 4 * rows * cols;//总量
    //right-up、right-down、down会去掉三个cols
    //right-up、right、right-down会去掉三个rows
    //但右上角的right-up和右下角的right-down重复了，则补回2
    edgeNum -= 3 * cols + 3 * rows - 2;

    beta = double(1) / (2 * totalDistance / edgeNum);
}

void GrabCutter::startGMM(int itTimes) {
    //当前正在进行的迭代次数
    int it = 0;
    //不限制迭代次数，直到收敛
    if (itTimes <= 0) {
        itTimes = -1;
    }

    while (itTimes != 0) {
        if (itTimes > 0) {
            --itTimes;
        }
        ++it;
        cout << "--- Start Train " << it << "---" << endl;

        //迭代主体

        //初始化
        bkGMM.initTrain();
        frGMM.initTrain();

        //step1:分配样本
        for (auto &v:imageMat) {
            for (auto &p:v) {
                if (p.alpha == PixelBelongEnum::B_MUST || p.alpha == PixelBelongEnum::B_PROB) {
                    bkGMM.addSample(p);
                } else {
                    frGMM.addSample(p);
                }
            }
        }

        //step2:GMM根据样本进行学习
        bkGMM.train();
        frGMM.train();

        //TODO step3:切割
        generateGraph();
        double energy=graph->maxflow();
        cout<<"=== Max Flow Energy: "<<energy<<" ==="<<endl;
    }
}

void GrabCutter::generateGraph() {

    int rows = imageMat.size();
    int cols = imageMat[0].size();

    int nodeNum = rows * cols + 2;//可能不需要这个+2
    int edgeNum = 4 * rows * cols - (3 * cols + 3 * rows - 2); //n-link
    edgeNum += 2 * nodeNum; //t-link

    delete(graph);
    graph= new GraphType(nodeNum, edgeNum);

    //U
    for (int i = 0; i < imageMat.size(); i++) {
        for (int j = 0; j < imageMat[0].size(); j++) {
            auto &pixel = imageMat[i][j];
            int index = graph->add_node();
            //source前景，sink背景 TODO 确定的像素点可能不需要去跑模型
            graph->add_tweights(index, frGMM.getMinProbability(pixel), bkGMM.getMinProbability(pixel));
        }
    }

    //V
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            int currIndex=i*rows+j;
            //right-up
            if (i > 0 && j < cols - 1) {
                double vv= getVofPixels(imageMat[i][j],imageMat[i-1][j+1]);
                graph->add_edge(currIndex,currIndex-cols+1,vv,vv);//双向边，但最小割只会切一条
            }
            //right
            if (j < cols - 1) {
                double vv=getVofPixels(imageMat[i][j],imageMat[i][j+1]);
                graph->add_edge(currIndex,currIndex+1,vv,vv);//双向边，但最小割只会切一条
            }
            //right-down
            if (i < rows - 1 && j < cols - 1) {
                double vv=getVofPixels(imageMat[i][j],imageMat[i+1][j+1]);
                graph->add_edge(currIndex,currIndex+cols+1,vv,vv);//双向边，但最小割只会切一条
            }
            //down
            if (i < rows - 1) {
                double vv=getVofPixels(imageMat[i][j],imageMat[i-1][j]);
                graph->add_edge(currIndex,currIndex+cols,vv,vv);//双向边，但最小割只会切一条
            }
        }
    }

}

double GrabCutter::getVofPixels(Pixel &p1,Pixel &p2) {
    double v=p1.rgb.calDistanceTo(p2.rgb);
    v*=-beta;
    v=exp(v);
    v*=50; //gamma（γ）
    return v;
}









