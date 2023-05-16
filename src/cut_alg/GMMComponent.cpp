//
// Created by DeathWind on 2023/5/15.
//

#include "cut_alg/GMMComponent.h"

double GMMComponent::kmeansGetDistance(const Pixel &pixel) {
    double ans = std::pow(pixel.rgb.r - this->mean[0], 2) +
                 std::pow(pixel.rgb.g - this->mean[1], 2) +
                 std::pow(pixel.rgb.b - this->mean[2], 2);
    return ans;
}

GMMComponent::GMMComponent(GMM *master, int number) {
    this->number = number;
    this->mean = std::vector<double>(3, 0);
    this->cov = std::vector<std::vector<double>>(3, std::vector<double>(3));
    this->master = master;
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

void GMMComponent::kmeansFinishOneIteration() {
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

void GMMComponent::clearSamples() {
    samples.clear();
}

int GMMComponent::getSampleSize() {
    return samples.size();
}

void GMMComponent::printMeanStr() {
    std::cout<<"("<<mean[0]<<","<<mean[1]<<","<<mean[2]<<")";
}


