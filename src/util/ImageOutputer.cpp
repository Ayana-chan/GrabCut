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
                        p[j][1] = 0;
                        p[j][2] = 0;
                        break;
                    case 1:
                        p[j][0] = 255;
                        p[j][1] = 255;
                        p[j][2] = 255;
                        break;
                    case 2:
                        p[j][0] = 255;
                        p[j][1] = 0;
                        p[j][2] = 0;
                        break;
                    case 3:
                        p[j][0] = 90;
                        p[j][1] = 90;
                        p[j][2] = 0;
                        break;
                    case 4:
                        p[j][0] = 120;
                        p[j][1] = 120;
                        p[j][2] = 0;
                        break;
                }
            } else {
                switch (pixel.k) {
                    case 0:
                        p[j][0] = 120;
                        p[j][1] = 120;
                        p[j][2] = 0;
                        break;
                    case 1:
                        p[j][0] = 150;
                        p[j][1] = 150;
                        p[j][2] = 0;
                        break;
                    case 2:
                        p[j][0] = 180;
                        p[j][1] = 180;
                        p[j][2] = 0;
                        break;
                    case 3:
                        p[j][0] = 210;
                        p[j][1] = 210;
                        p[j][2] = 0;
                        break;
                    case 4:
                        p[j][0] = 240;
                        p[j][1] = 240;
                        p[j][2] = 0;
                        break;
                }
            }
        }
    }
    imshow("Output Image", image);
    cv::waitKey(0);

    return image;
}
