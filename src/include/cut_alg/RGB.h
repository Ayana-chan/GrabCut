//
// Created by DeathWind on 2023/5/14.
//

#ifndef GRABCUT_RGB_H
#define GRABCUT_RGB_H

#include <opencv2/opencv.hpp>

class RGB {
public:
    unsigned char r;
    unsigned char g;
    unsigned char b;

    RGB()=default;
    RGB(unsigned char r, unsigned char g, unsigned char b);
    explicit RGB(cv::Vec3b vec);

    friend std::ostream &operator<<(std::ostream &os, const RGB &o);
};




#endif //GRABCUT_RGB_H
