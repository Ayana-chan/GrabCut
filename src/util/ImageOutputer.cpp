//
// Created by DeathWind on 2023/5/16.
//

#include "util/ImageOutputer.h"

cv::Mat ImageOutputer::generateImage(ImageMat &analysedImage) {

    cv::Mat image(analysedImage.size(),analysedImage[0].size(),CV_8UC3);
    for (int i = 0; i < image.rows; i++)
    {
        auto *p = image.ptr<cv::Vec3b>(i);
        for (int j = 0; j < image.cols; j++)
        {
            p[j][0]=analysedImage[i][j].rgb.b;
            p[j][1]=analysedImage[i][j].rgb.g;
            p[j][2]=analysedImage[i][j].rgb.r;
        }
    }
    imshow("Output Image", image);
    cv::waitKey(0);

    return image;
}
