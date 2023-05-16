//
// Created by DeathWind on 2023/5/15.
//

#ifndef GRABCUT_GMMCOMPONENT_H
#define GRABCUT_GMMCOMPONENT_H

#include <vector>

#include "cut_alg/Pixel.h"

class GMM;

class GMMComponent {
public:
    GMMComponent(GMM *master, int number);

    int number; //Component编号，0~K-1

    double coefs; //权重pi，等于属于该分量的样本数占当前GMM总样本数的比例
    std::vector<double> mean; //均值，长度为3的向量，对应r、g、b
    std::vector<std::vector<double>> cov; //协方差，3*3的矩阵

    //---kmeans

    /**
     * 返回距离（二范式）的平方
     * @param pixel
     * @return
     */
    double kmeansGetDistance(const Pixel &pixel);

    void kmeansInitBySample(Pixel &pixel);

    void kmeansAddSample(Pixel &pixel);

    //---calculate

    //kmeans只要计算mean就够了
    void calculateMean();

    //计算方差
    void calculateCov();

    //---sample

    void clearSamples();

    int getSampleSize();

    //---for print

    void printMean();

    void printCov();

private:
    GMM *master; //回指所属的GMM
    std::vector<Pixel *> samples; //自己的训练样本

    std::vector<std::vector<double>> inverseCov; //方差的逆矩阵
    double detCov; //方差的行列式结果

    void calculateDetCov();
    void calculateInverseCov();
};


#endif //GRABCUT_GMMCOMPONENT_H
