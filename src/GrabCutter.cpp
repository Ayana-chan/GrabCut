//
// Created by DeathWind on 2023/5/14.
//

#include "GrabCutter.h"
#include "util/OutputSwitcher.h"
#include "util/ImageOutputer.h"

//GMM迭代次数（非正则视为无限迭代直至收敛）
#define GMM_IT_TIMES 0
//GMM的component数量（K）
#define GMM_K 5
//kmeans的迭代次数
#define KMEANS_IT_TIMES 10
//确定为背景（前景）时，与t（s）相连的边的权值。不知多少合适，很大很小也不太影响效率
#define MUST_VALUE 5000
//累计距离（用于计算β）的最小值，小于此值则补到此值（防止纯色）
#define MINIMUM_TOTAL_DISTANCE 0.00001
//能量减少的比例小于此值时，视为收敛
#define CONVERGENCE_CHANGE_RATIO 0.03


using namespace std;

GrabCutter::GrabCutter() : uiController(this), bkGMM(GMM(GMM_K)), frGMM(GMM(GMM_K)) {
    graph = nullptr;
}

GrabCutter::GrabCutter(const std::string &imageOutPutPath) :GrabCutter(){
    ImageOutputer::imageOutPutPath=imageOutPutPath;
}

GrabCutter::~GrabCutter() {
    delete (graph);
}

void GrabCutter::start(const std::string &path) {
    //更改cout输出目的地
    OutputSwitcher outputSwitcher;
    outputSwitcher.switchOutputToFile(ImageOutputer::imageOutPutPath+"output.txt");

    //输入图像

    uiController.getImagesFromPath(path);
    uiController.analyseImage(imageMat);
    cout << "Image Size: " << imageMat.size() << "*" << imageMat[0].size() << endl;

    initGrabCut();

    advanceGrabCut();


}

void GrabCutter::modifyAlphaByUser(int x, int y, int aimAlpha) {
//    cout<<"modifyAlphaByUser: "<<x<<" "<<y<<" "<<aimAlpha;
    PixelBelongEnum newAlpha;
    if (aimAlpha == 0) {
        newAlpha = PixelBelongEnum::B_MUST;
    } else {
        newAlpha = PixelBelongEnum::F_MUST;
    }
    auto baseX=uiController.rect.x;
    auto baseY=uiController.rect.y;
    imageMat[baseY+y][baseX+x].alpha = newAlpha;
}

void GrabCutter::initGrabCut() {

    cout << "--------------- initGrabCut ---------------" << endl;

    //矩形输入与初始化

    std::cout << std::endl;
    uiController.drawRect(uiController.imageName);
    updateMatByRect(uiController.posX1, uiController.posY1,
                    uiController.posX2, uiController.posY2);
    std::cout << std::endl;
    auto start = std::chrono::system_clock::now();
    initGMM();

    //计算初始化时间
    auto end1 = std::chrono::system_clock::now();
    auto elapsed1 = std::chrono::duration_cast<std::chrono::milliseconds>(end1 - start).count();
    cout << "===== GMM INIT DURATION: " << elapsed1 << "ms =====" << endl;
    //ImageOutputer::generateTenColorImage(imageMat);

    //迭代训练

    calculateBeta();
    startGMM(GMM_IT_TIMES);

    //计算训练时间
    auto end2 = std::chrono::system_clock::now();
    auto elapsed2 = std::chrono::duration_cast<std::chrono::milliseconds>(end2 - end1).count();
    cout << "===== GMM TRAIN DURATION: " << elapsed2 << "ms =====" << endl;
    //计算总时间
    auto end = std::chrono::system_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    cout << "===== GMM TOTAL DURATION: " << elapsed << "ms =====" << endl;

    ImageOutputer::generateTenColorImage(imageMat);
    ImageOutputer::generateHandledImage(imageMat);
}

void GrabCutter::advanceGrabCut() {

    cout << "--------------- advanceGrabCut ---------------" << endl;

    while(true) {

        bool isContinue = uiController.additionalDrawImage();
        if(!isContinue){
            cout<<"Exit advanceGrabCut"<<endl;
            break;
        }

        startGMM(1);

        ImageOutputer::generateTenColorImage(imageMat);
        ImageOutputer::generateHandledImage(imageMat);
    }
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

    std::cout << std::endl << "--- INIT GMM COMPLETE ---" << std::endl << std::endl;
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
    if (totalDistance < MINIMUM_TOTAL_DISTANCE) {
        totalDistance = MINIMUM_TOTAL_DISTANCE;
    }

    double edgeNum = 4 * rows * cols;//总量
    //right-up、right-down、down会去掉三个cols
    //right-up、right、right-down会去掉三个rows
    //但右上角的right-up和右下角的right-down重复了，则补回2
    edgeNum -= 3 * cols + 3 * rows - 2;

    beta = double(1) / (2 * totalDistance / edgeNum);
    cout << "Beta: " << beta << endl;
}

void GrabCutter::startGMM(int itTimes) {
    //当前正在进行的迭代次数
    int it = 0;

    //itTimes不为正则无限迭代（直到收敛）
    while (it<itTimes || itTimes<=0) {
        ++it;
        cout << endl << "------------ Start Train " << it << "------------" << endl << endl;

        //迭代主体

        //初始化
        bkGMM.initTrain();
        frGMM.initTrain();

        auto start = std::chrono::system_clock::now();

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

        //计算分配与学习的时间
        auto end1 = std::chrono::system_clock::now();
        auto elapsed1 = std::chrono::duration_cast<std::chrono::milliseconds>(end1 - start).count();
        cout << "  === GMM Dispatch and Learn DURATION: " << elapsed1 << "ms" << " ===" << endl;

        //step3:切割重整
        generateGraph();

        //计算生成图像时间
        auto end2 = std::chrono::system_clock::now();
        auto elapsed2 = std::chrono::duration_cast<std::chrono::milliseconds>(end2 - end1).count();
        cout << "  === GMM GenerateGraph DURATION: " << elapsed2 << "ms" << " ===" << endl;

        cout << "--- Maxflow ... ---" << endl;
        double energy = graph->maxflow();
        cout << "$$$ Max Flow Energy: " << energy << " $$$" << endl;

        //计算max-flow时间
        auto end3 = std::chrono::system_clock::now();
        auto elapsed3 = std::chrono::duration_cast<std::chrono::milliseconds>(end3 - end2).count();
        cout << "  === GMM MaxFlow DURATION: " << elapsed3 << "ms" << " ===" << endl;

        //修改前景背景
        for (int i = 0; i < imageMat.size(); i++) {
            for (int j = 0; j < imageMat[0].size(); j++) {
                //用户指定的不能改
                if (imageMat[i][j].alpha == PixelBelongEnum::B_MUST ||
                    imageMat[i][j].alpha == PixelBelongEnum::F_MUST) {
                    continue;
                }

                int currIndex = i * imageMat[0].size() + j;
                if (graph->what_segment(currIndex) == GraphType::SOURCE) {
                    //前景
                    imageMat[i][j].alpha = PixelBelongEnum::F_PROB;
                } else {
                    //背景
                    imageMat[i][j].alpha = PixelBelongEnum::B_PROB;
                }
//                if (i == j) {
//                    cout << imageMat[i][j] << endl;
//                }
            }
        }

//        ImageOutputer::generateTenColorImage(imageMat);
//        ImageOutputer::generateHandledImage(imageMat);
//        cv::waitKey(0);

        //迭代收敛判断（仅无限迭代时时）
        if(itTimes<=0){
            //第一次不算
            if(it!=1){
                double deltaEnergy=lastEnergy-energy;
                double ratio=deltaEnergy/lastEnergy;
                cout << "$$$ Max Flow Energy Decrease Ratio: " << ratio << " $$$" << endl;
                if(ratio<=CONVERGENCE_CHANGE_RATIO){
                    //收敛
                    cout << "--- Convergence in: " << it << " times ---" << endl;
                    return;
                }
            }
            //不收敛
            lastEnergy=energy;
        }

    }
}

void GrabCutter::generateGraph() {

    cout << "--- Generate Graph ... ---" << endl;

    int rows = imageMat.size();
    int cols = imageMat[0].size();

    int nodeNum = rows * cols + 2;//可能不需要这个+2
    int edgeNum = 4 * rows * cols - (3 * cols + 3 * rows - 2); //n-link
    edgeNum += 2 * nodeNum; //t-link

    delete (graph);
    graph = new GraphType(nodeNum, edgeNum);

    //U
    for (int i = 0; i < imageMat.size(); i++) {
        for (int j = 0; j < imageMat[0].size(); j++) {
            auto &pixel = imageMat[i][j];
            int index = graph->add_node();
            if (pixel.alpha == PixelBelongEnum::B_MUST) {
                graph->add_tweights(index, 0, MUST_VALUE);
            } else if (pixel.alpha == PixelBelongEnum::F_MUST) {
                graph->add_tweights(index, MUST_VALUE, 0);
            } else {
                //source前景，sink背景
                graph->add_tweights(index, bkGMM.getMinProbability(pixel), frGMM.getMinProbability(pixel));
//                if (i == j) {
//                    cout << "*** add_tweights: " << i << " " << j << " source: " << bkGMM.getMinProbability(pixel)
//                         << " sink: "
//                         << frGMM.getMinProbability(pixel) << endl;
//                }
            }
        }
    }

    //V
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            int currIndex = i * cols + j;
            //right-up
            if (i > 0 && j < cols - 1) {
                double vv = getVofPixels(imageMat[i][j], imageMat[i - 1][j + 1]);
                graph->add_edge(currIndex, currIndex - cols + 1, vv, vv);//双向边，但最小割只会切一条
            }
            //right
            if (j < cols - 1) {
                double vv = getVofPixels(imageMat[i][j], imageMat[i][j + 1]);
                graph->add_edge(currIndex, currIndex + 1, vv, vv);//双向边，但最小割只会切一条
//                if(i==j){
//                    cout<<"*** V: "<<vv<<" ***"<<endl;
//                }
            }
            //right-down
            if (i < rows - 1 && j < cols - 1) {
                double vv = getVofPixels(imageMat[i][j], imageMat[i + 1][j + 1]);
                graph->add_edge(currIndex, currIndex + cols + 1, vv, vv);//双向边，但最小割只会切一条
            }
            //down
            if (i < rows - 1) {
                double vv = getVofPixels(imageMat[i][j], imageMat[i + 1][j]);
                graph->add_edge(currIndex, currIndex + cols, vv, vv);//双向边，但最小割只会切一条
            }
        }
    }

}

double GrabCutter::getVofPixels(Pixel &p1, Pixel &p2) {
    double v = p1.rgb.calDistanceTo(p2.rgb);
    v *= -beta;
    v = exp(v);
    v *= 50; //gamma（γ）
    return v;
}

















