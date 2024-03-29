//
// Created by DeathWind on 2023/5/15.
//
#include <random>

#include "cut_alg/GMM.h"

GMM::GMM(int componentNum) {
    for (int i = 0; i < componentNum; i++) {
        this->componentList.emplace_back(GMMComponent(this, i));
    }
}

void GMM::initTrain() {
    //清空样本
    for (auto &component:componentList) {
        component.clearSamples();
    }
}

void GMM::addSample(Pixel &pixel) {

    double min=-1;
    int minArg=0;
    for(int i=0;i<componentList.size();i++){
        double pro=componentList[i].getProbability(pixel);
        //无样本的分量，忽视
        if(pro<0){
            continue;
        }
        if(min<0 || pro<min){
            min=pro;
            minArg=i;
        }
    }

    componentList[minArg].addSample(pixel);
}

void GMM::train() {

    int sum = 0;//记录总样本数
    for (auto &component:componentList) {
        sum += component.getSampleSize();
    }

    for (auto &component:componentList) {
        component.calculateCoefs(sum);
        component.calculateMean();
        component.calculateCov();

        //打印训练后参数信息
        component.printParameters();
    }
}

void GMM::initByKmeans(std::vector<Pixel *> &pixels, int itTimes) {

    //挑选样本来初始化
    //随机数
    std::random_device rd;
    std::mt19937 gen((unsigned int)time(nullptr));
    std::uniform_int_distribution<int> dis(0, pixels.size() - 1);

    std::cout << "Init RGB: ";
    for (auto &component : componentList) {
        //std::cout<<"rand: " << dis(gen) << " ";
        Pixel &p = *pixels[dis(gen)];
        std::cout << p.rgb << " ";
        component.kmeansInitBySample(p);
    }
    std::cout << std::endl;

    //迭代
    std::cout << "--- Begin K-Means Iteration ---" << std::endl;
    for (int i = 0; i < itTimes; i++) {
        //把像素加入到最匹配的模型中
        for (auto p:pixels) {
            kmeansAddSample(*p);
        }

        //更新模型参数（mean）
        for (auto &component : componentList) {
            component.calculateMean();
            std::cout << "{size: " << component.getSampleSize() << ", mean: ";
            component.printMean();
            std::cout << "} ";
            //清空sample（最后一次不清空，用于GMM计算）
            if (i != itTimes - 1) {
                component.clearSamples();
            }
        }
        std::cout << std::endl;
    }
    std::cout << "--- End   K-Means Iteration ---" << std::endl;
}

double GMM::getMinProbability(Pixel &pixel) {

    double min=-1;
    for(int i=0;i<componentList.size();i++){
        double pro=componentList[i].getProbability(pixel);
        //无样本的分量，忽视
        if(pro<0){
            continue;
        }
        if(min<0 || pro<min){
            min=pro;
        }
    }

    return min;
}

void GMM::kmeansAddSample(Pixel &pixel) {
    double min = componentList[0].kmeansGetDistance(pixel);
    int minArg = 0;
    for (int i = 1; i < componentList.size(); i++) {
        double dis = componentList[i].kmeansGetDistance(pixel);
        //若两个模型完全一样，则不可避免地导致下标大的模型一个都吃不到
        if (dis < min) {
            min = dis;
            minArg = i;
        }
    }
    componentList[minArg].kmeansAddSample(pixel);
}






