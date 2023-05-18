//
// Created by DeathWind on 2023/5/15.
//

#include <cmath>

#include "cut_alg/GMMComponent.h"

using namespace std;

#define MINIMUM_DET 0.00001

GMMComponent::GMMComponent(GMM *master, int number) {
    this->number = number;
    this->mean = std::vector<double>(3, 0);
    this->cov = std::vector<std::vector<double>>(3, std::vector<double>(3));
    this->covInverse = std::vector<std::vector<double>>(3, std::vector<double>(3));
    this->master = master;
}

double GMMComponent::getProbability(Pixel &pixel) {

    //若权重为0（没有样本），则返回-1
    if (coefs == 0) {
        return -1;
    }

    double ans1 = -log(coefs);

    double ans2 = 0.5 * log(covDet);

    //012
    //345
    //678
    //p^T*M*p=0RR+4GG+8BB+(1+3)RG+(2+6)RB+(5+7)GB
    double dr = pixel.rgb.r - mean[0];
    double dg = pixel.rgb.g - mean[1];
    double db = pixel.rgb.b - mean[2];

    double ans3 = covInverse[0][0] * dr * dr +
                  covInverse[1][1] * dg * dg +
                  covInverse[2][2] * db * db +
                  (covInverse[0][1] + covInverse[1][0]) * dr * dg +
                  (covInverse[0][2] + covInverse[2][0]) * dr * db +
                  (covInverse[1][2] + covInverse[2][1]) * dg * db;
    ans3 /= 2;

    double ans = ans1 + ans2 + ans3;

    return ans;
}

void GMMComponent::addSample(Pixel &pixel) {
    samples.emplace_back(&pixel);
    //修改点的信息
    pixel.k=number;
}

double GMMComponent::kmeansGetDistance(const Pixel &pixel) {
    double ans = std::pow(pixel.rgb.r - this->mean[0], 2) +
                 std::pow(pixel.rgb.g - this->mean[1], 2) +
                 std::pow(pixel.rgb.b - this->mean[2], 2);
    return ans;
}

void GMMComponent::kmeansInitBySample(Pixel &pixel) {
    mean[0] = pixel.rgb.r;
    mean[1] = pixel.rgb.g;
    mean[2] = pixel.rgb.b;
}

void GMMComponent::kmeansAddSample(Pixel &pixel) {
    samples.emplace_back(&pixel);
    //修改点的信息
    pixel.k = number;
}

void GMMComponent::calculateCoefs(int &sum) {
    coefs = (double) (getSampleSize()) / sum;
}

void GMMComponent::calculateMean() {
    //若没有任何点在里面，则不更新mean
    if (samples.empty()) {
        return;
    }

    mean[0] = mean[1] = mean[2] = 0;
    for (Pixel *s:samples) {
        mean[0] += s->rgb.r;
        mean[1] += s->rgb.g;
        mean[2] += s->rgb.b;
    }
    mean[0] /= samples.size();
    mean[1] /= samples.size();
    mean[2] /= samples.size();
}

void GMMComponent::calculateCov() {
    //若没有任何点在里面，则不更新参数
    if (samples.empty()) {
        return;
    }

    //cov(X,Y)=EXY-EX*EY
    double covRR = 0;
    double covGG = 0;
    double covBB = 0;
    double covRG = 0;
    double covGB = 0;
    double covRB = 0;
    for (Pixel *p:samples) {
        covRR += p->rgb.r * p->rgb.r;
        covGG += p->rgb.g * p->rgb.g;
        covBB += p->rgb.b * p->rgb.b;
        covRG += p->rgb.r * p->rgb.g;
        covRB += p->rgb.r * p->rgb.b;
        covGB += p->rgb.g * p->rgb.b;
    }
    covRR = covRR / samples.size() - mean[0] * mean[0];
    covGG = covGG / samples.size() - mean[1] * mean[1];
    covBB = covBB / samples.size() - mean[2] * mean[2];
    covRG = covRG / samples.size() - mean[0] * mean[1];
    covGB = covGB / samples.size() - mean[1] * mean[2];
    covRB = covRB / samples.size() - mean[0] * mean[2];

    cov[0][0] = covRR;
    cov[0][1] = covRG;
    cov[0][2] = covRB;
    cov[1][0] = covRG;
    cov[1][1] = covGG;
    cov[1][2] = covGB;
    cov[2][0] = covRB;
    cov[2][1] = covGB;
    cov[2][2] = covBB;

    //计算行列式
    calculateDetCov();
    //几乎纯色的图像里协方差的行列式非常接近于0，取log之后极小，同时也会导致逆矩阵非常大，因此添加噪声
    if (covDet <= MINIMUM_DET) {
        //在对角线上添加噪声，暂不知副作用
        cov[0][0] += 0.01;
        cov[1][1] += 0.01;
        cov[2][2] += 0.01;
        //重新计算
        calculateDetCov();
    }

    //计算逆矩阵
    calculateInverseCov();
}

void GMMComponent::clearSamples() {
    samples.clear();
}

int GMMComponent::getSampleSize() {
    return samples.size();
}

void GMMComponent::printParameters() {
    std::cout<<std::endl<<"--- GMM Component "<<number<<" Para begin---"<<std::endl;
    std::cout<<"simple size: "<<getSampleSize()<<std::endl;
    std::cout<<"coefs: "<<coefs<<std::endl;
    std::cout<<std::endl;
    std::cout<<"mean: ";
    printMean();
    std::cout<<std::endl;
    std::cout<<std::endl;
    printCov();
    std::cout<<"--- GMM Component "<<number<<" Para end---"<<std::endl;
    std::cout<<std::endl;
}

void GMMComponent::printMean() {
    std::cout << "(" << mean[0] << "," << mean[1] << "," << mean[2] << ")";
}

void GMMComponent::printCov() {
    std::cout << "cov:" << std::endl;
    for (int i = 0; i < cov.size(); i++) {
        for (int j = 0; j < cov[0].size(); j++) {
            std::cout << cov[i][j];
            if (j < cov[0].size() - 1) {
                std::cout << ",";
            } else {
                std::cout << ";"<<std::endl;
            }
        }
    }
    std::cout << std::endl;
    std::cout << "det: " << covDet<<std::endl<<std::endl;
    std::cout << "cov inverse:" << std::endl;
    for (int i = 0; i < covInverse.size(); i++) {
        for (int j = 0; j < covInverse[0].size(); j++) {
            std::cout << covInverse[i][j];
            if (j < covInverse[0].size() - 1) {
                std::cout << ",";
            } else {
                std::cout << ";"<<std::endl;
            }
        }
    }
}


void GMMComponent::calculateDetCov() {
    covDet = cov[0][0] * (cov[1][1] * cov[2][2] - cov[1][2] * cov[2][1])
             - cov[0][1] * (cov[1][0] * cov[2][2] - cov[1][2] * cov[2][0])
             + cov[0][2] * (cov[1][0] * cov[2][1] - cov[1][1] * cov[2][0]);
}

void GMMComponent::calculateInverseCov() {
    //A^-1 = A* / |A|
    //A*的每一项为除自己所在行列以外剩余矩阵的行列式，再附上对应的正负好，计算完后再转置
    covInverse[0][0] = (cov[1][1] * cov[2][2] - cov[1][2] * cov[2][1]) / covDet;
    covInverse[1][0] = -(cov[1][0] * cov[2][2] - cov[1][2] * cov[2][0]) / covDet;
    covInverse[2][0] = (cov[1][0] * cov[2][1] - cov[1][1] * cov[2][0]) / covDet;
    covInverse[0][1] = -(cov[0][1] * cov[2][2] - cov[0][2] * cov[2][1]) / covDet;
    covInverse[1][1] = (cov[0][0] * cov[2][2] - cov[0][2] * cov[2][0]) / covDet;
    covInverse[2][1] = -(cov[0][0] * cov[2][1] - cov[0][1] * cov[2][0]) / covDet;
    covInverse[0][2] = (cov[0][1] * cov[1][2] - cov[0][2] * cov[1][1]) / covDet;
    covInverse[1][2] = -(cov[0][0] * cov[1][2] - cov[0][2] * cov[1][0]) / covDet;
    covInverse[2][2] = (cov[0][0] * cov[1][1] - cov[0][1] * cov[1][0]) / covDet;
}














