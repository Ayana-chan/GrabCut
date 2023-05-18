//
// Created by DeathWind on 2023/5/16.
//

#ifndef GRABCUT_IMAGEOUTPUTER_H
#define GRABCUT_IMAGEOUTPUTER_H

#include <opencv2/opencv.hpp>

#include "cut_alg/Pixel.h"

class ImageOutputer {
public:
    static std::string imageOutPutPath;

    static cv::Mat generateTenColorImage(ImageMat &analysedImage,const std::string& name="Ten Color Image");
    static cv::Mat generateHandledImage(ImageMat &analysedImage,const std::string& name="Handled Result");
};


#endif //GRABCUT_IMAGEOUTPUTER_H
