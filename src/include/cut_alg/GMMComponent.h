//
// Created by DeathWind on 2023/5/15.
//

#ifndef GRABCUT_GMMCOMPONENT_H
#define GRABCUT_GMMCOMPONENT_H

#include <vector>

#include "cut_alg/RGB.h"
#include "cut_alg/GMM.h"

class GMMComponent {
public:
    double coefs; //权重pi，等于属于该分量的样本数占当前GMM总样本数的比例
    std::vector<double> mean; //均值，长度为3的向量
    std::vector<std::vector<double>> cov; //协方差，3*3的矩阵

private:
    GMM* master; //回指所属的GMM
    std::vector<RGB> samples; //当前GMM的训练样本
};


#endif //GRABCUT_GMMCOMPONENT_H
