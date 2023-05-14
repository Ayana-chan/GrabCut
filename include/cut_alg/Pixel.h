//
// Created by DeathWind on 2023/5/14.
//

#ifndef GRABCUT_PIXEL_H
#define GRABCUT_PIXEL_H

#include "cut_alg/RGB.h"

class Pixel {
public:
    RGB rgb;
    unsigned char alpha; //像素归属。 0背景 1前景
    unsigned char k; //属于第几个高斯分量。 [1,K]。和alpha一起唯一决定一个高斯分量。

    Pixel()=default;

    explicit Pixel(const RGB &rgb);

    Pixel(const RGB &rgb, unsigned char alpha);
};


#endif //GRABCUT_PIXEL_H
