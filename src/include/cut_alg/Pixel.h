//
// Created by DeathWind on 2023/5/14.
//

#ifndef GRABCUT_PIXEL_H
#define GRABCUT_PIXEL_H

#include "cut_alg/RGB.h"
#include "cut_alg/PixelBelongEnum.h"
#include <vector>


class Pixel {
public:
    RGB rgb;
    PixelBelongEnum alpha; //像素归属，默认为归属前景F_PROB
    unsigned char k; //属于第几个高斯分量。 [1,K]。和alpha一起唯一决定一个高斯分量。

    Pixel()=default;
    explicit Pixel(const RGB &rgb);
    Pixel(const RGB &rgb, PixelBelongEnum alpha);

    friend std::ostream &operator<<(std::ostream &os, const Pixel &o);
};

typedef std::vector<std::vector<Pixel>> ImageMat;


#endif //GRABCUT_PIXEL_H
