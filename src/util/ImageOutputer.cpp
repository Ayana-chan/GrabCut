//
// Created by DeathWind on 2023/5/16.
//

#include "util/ImageOutputer.h"

cv::Mat ImageOutputer::generateTenColorImage(ImageMat &analysedImage) {

    std::cout<<"generateTenColorImage..."<<std::endl;
    cv::Mat image(analysedImage.size(), analysedImage[0].size(), CV_8UC3);
    for (int i = 0; i < image.rows; i++) {
        auto *p = image.ptr<cv::Vec3b>(i);
        for (int j = 0; j < image.cols; j++) {
            auto &pixel = analysedImage[i][j];
            if (pixel.alpha == PixelBelongEnum::B_PROB || pixel.alpha == PixelBelongEnum::B_MUST) {
                switch (pixel.k) {
                    case 0:
                        p[j][0] = 0;
                        p[j][1] = 255;
                        p[j][2] = 0;
                        break;
                    case 1:
                        p[j][0] = 0;
                        p[j][1] = 255;
                        p[j][2] = 127;
                        break;
                    case 2:
                        p[j][0] = 34;
                        p[j][1] = 139;
                        p[j][2] = 34;
                        break;
                    case 3:
                        p[j][0] = 35;
                        p[j][1] = 142;
                        p[j][2] = 107;
                        break;
                    case 4:
                        p[j][0] = 140;
                        p[j][1] = 199;
                        p[j][2] = 0;
                        break;
                }
            } else {
                switch (pixel.k) {
                    case 0:
                        p[j][0] = 0;
                        p[j][1] = 255;
                        p[j][2] = 255;
                        break;
                    case 1:
                        p[j][0] = 18;
                        p[j][1] = 153;
                        p[j][2] = 255;
                        break;
                    case 2:
                        p[j][0] = 87;
                        p[j][1] = 207;
                        p[j][2] = 227;
                        break;
                    case 3:
                        p[j][0] = 0;
                        p[j][1] = 215;
                        p[j][2] = 255;
                        break;
                    case 4:
                        p[j][0] = 132;
                        p[j][1] = 227;
                        p[j][2] = 255;
                        break;
                }
            }
        }
    }
    imshow("Output Image", image);
    cv::waitKey(0);

    return image;
}

cv::Mat ImageOutputer::generateHandledImage(ImageMat &analysedImage) {

    std::cout<<"generateTenColorImage..."<<std::endl;
    cv::Mat image(analysedImage.size(), analysedImage[0].size(), CV_8UC3);
    for (int i = 0; i < image.rows; i++) {
        auto *p = image.ptr<cv::Vec3b>(i);
        for (int j = 0; j < image.cols; j++) {
            auto &pixel = analysedImage[i][j];
            if (pixel.alpha == PixelBelongEnum::B_PROB || pixel.alpha == PixelBelongEnum::B_MUST) {
                p[j][0] = 0;
                p[j][1] = 0;
                p[j][2] = 0;
            } else {
                p[j][0] = pixel.rgb.b;
                p[j][1] = pixel.rgb.g;
                p[j][2] = pixel.rgb.r;
            }
        }
    }
    imshow("Output Image", image);
    cv::waitKey(0);

    return image;
}
