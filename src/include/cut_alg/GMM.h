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
};


#endif //GRABCUT_GMM_H
