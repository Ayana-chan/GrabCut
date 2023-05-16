//
// Created by DeathWind on 2023/5/15.
//

#ifndef GRABCUT_GMM_H
#define GRABCUT_GMM_H

#include <vector>

#include "cut_alg/GMMComponent.h"

class GMM {
public:
    GMM()=default;
    explicit GMM(int componentNum);

    std::vector<GMMComponent> componentList; //高斯分量

    void initByKmeans(std::vector<Pixel*> &pixels);

private:
    /**
     * 寻找距离最小的component来插入
     * @param pixel
     */
    void kmeansAddSample(Pixel &pixel);
};


#endif //GRABCUT_GMM_H
