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

void GMM::initByKmeans(std::vector<Pixel*> &pixels) {

    //挑选样本来初始化
    //随机数
    std::random_device rd;
    auto gen = std::default_random_engine(rd());
    std::uniform_int_distribution<int> dis(0,pixels.size()-1);

    std::cout<<"Init RGB: ";
    for (int i=0; i<componentList.size(); ++i) {
        //std::cout<<"rand: " << dis(gen) << " ";
        Pixel &p=*pixels[dis(gen)];
        std::cout<<p.rgb<<" ";
        componentList[i].kmeansInitBySample(p);
    }
    std::cout<<std::endl;
}

void GMM::kmeansAddSample(Pixel &pixel) {
    double min=componentList[0].kmeansGetDistance(pixel);
    int minArg=0;
    for(int i=1;i<componentList.size();i++){
        double dis=componentList[i].kmeansGetDistance(pixel);
        if(dis<min){
            min=dis;
            minArg=i;
        }
    }
    componentList[minArg].kmeansAddSample(pixel);
}
