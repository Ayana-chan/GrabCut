//
// Created by DeathWind on 2023/5/15.
//
#include <random>

#include "cut_alg/GMM.h"

GMM::GMM(int componentNum) {
    for(int i=0;i<componentNum;i++){
        this->componentList.emplace_back(GMMComponent(this,i));
    }
}

void GMM::initByKmeans(std::vector<Pixel*> &pixels,int itTimes) {

    //挑选样本来初始化
    //随机数
    std::random_device rd;
    auto gen = std::default_random_engine(rd());
    std::uniform_int_distribution<int> dis(0,pixels.size()-1);

    std::cout<<"Init RGB: ";
    for (auto & component : componentList) {
        //std::cout<<"rand: " << dis(gen) << " ";
        Pixel &p=*pixels[dis(gen)];
        std::cout<<p.rgb<<" ";
        component.kmeansInitBySample(p);
    }
    std::cout<<std::endl;

    //迭代
    std::cout<<"--- Begin K-Means Iteration ---"<<std::endl;
    for(int i=0;i<itTimes;i++) {
        //把像素加入到最匹配的模型中
        for (auto p:pixels) {
            kmeansAddSample(*p);
        }

        //更新模型参数（mean）
        for (auto &component : componentList) {
            component.calculateMean();
            std::cout << "{size: " << component.getSampleSize() << ", mean: ";
            component.printMean();
            std::cout<<"} ";
            //清空sample
            component.clearSamples();
        }
        std::cout << std::endl;
    }
    std::cout<<"--- End   K-Means Iteration ---"<<std::endl;
}

void GMM::kmeansAddSample(Pixel &pixel) {
    double min=componentList[0].kmeansGetDistance(pixel);
    int minArg=0;
    for(int i=1;i<componentList.size();i++){
        double dis=componentList[i].kmeansGetDistance(pixel);
        //若两个模型完全一样，则不可避免地导致下标大的模型一个都吃不到
        if(dis<min){
            min=dis;
            minArg=i;
        }
    }
    componentList[minArg].kmeansAddSample(pixel);
}
