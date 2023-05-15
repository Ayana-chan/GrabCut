//
// Created by DeathWind on 2023/5/15.
//

#include "cut_alg/GMM.h"

GMM::GMM(int componentNum) {
    for(int i=0;i<componentNum;i++){
        this->componentList.emplace_back(GMMComponent());
    }
}
