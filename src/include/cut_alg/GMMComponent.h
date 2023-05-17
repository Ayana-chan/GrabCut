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

    //---gmm

    /**
     * 获取一个像素属于该高斯分量的概率，也即返回D
     * @param pixel
     * @return 若权重为0（没有样本），则返回-1
     */
    double getProbability(Pixel &pixel);

    void addSample(Pixel &pixel);

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

    //计算权重。需要传入总个数。权重可能为0
    void calculateCoefs(int &sum);

    //计算均值。kmeans只要计算mean就够了
    void calculateMean();

    //计算方差
    void calculateCov();

    //---sample

    void clearSamples();

    int getSampleSize();

    //---for print

    void printParameters();

    void printMean();

    void printCov();

private:
    double coefs; //权重pi，等于属于该分量的样本数占当前GMM总样本数的比例
    std::vector<double> mean; //均值，长度为3的向量，对应r、g、b
    std::vector<std::vector<double>> cov; //协方差，3*3的矩阵

    GMM *master; //回指所属的GMM
    std::vector<Pixel *> samples; //自己的训练样本

    std::vector<std::vector<double>> covInverse; //方差的逆矩阵
    double covDet; //方差的行列式结果

    void calculateDetCov();

    void calculateInverseCov();
};


#endif //GRABCUT_GMMCOMPONENT_H
