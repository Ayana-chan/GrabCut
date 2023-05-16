//
// Created by DeathWind on 2023/5/15.
//

#ifndef GRABCUT_GMM_H
#define GRABCUT_GMM_H

#include <vector>

#include "cut_alg/GMMComponent.h"

class GMM {
public:
    explicit GMM(int componentNum);

    std::vector<GMMComponent> componentList; //高斯分量

    /**
     * 寻找距离最小的component来插入
     * @param pixel
     */
    void kmeansAddSample(Pixel &pixel);

    /**
     * 使用k-means算法对GMM进行初始化
     * @param pixels 样本集
     * @param itTimes k-means迭代次数
     */
    void initByKmeans(std::vector<Pixel*> &pixels,int itTimes);

private:

};


#endif //GRABCUT_GMM_H
