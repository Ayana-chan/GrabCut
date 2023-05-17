//
// Created by DeathWind on 2023/5/16.
//

#ifndef GRABCUT_IMAGEOUTPUTER_H
#define GRABCUT_IMAGEOUTPUTER_H

#include <opencv2/opencv.hpp>

#include "cut_alg/Pixel.h"

class ImageOutputer {
public:
    static cv::Mat generateTenColorImage(ImageMat &analysedImage);
    static cv::Mat generateHandledImage(ImageMat &analysedImage);
};


#endif //GRABCUT_IMAGEOUTPUTER_H
