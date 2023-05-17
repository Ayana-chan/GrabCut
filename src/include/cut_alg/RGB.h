//
// Created by DeathWind on 2023/5/14.
//

#ifndef GRABCUT_RGB_H
#define GRABCUT_RGB_H

#include <opencv2/opencv.hpp>

class RGB {
public:
    uchar r;
    uchar g;
    uchar b;

    RGB() = default;

    RGB(uchar r, uchar g, uchar b);

    explicit RGB(cv::Vec3b vec);

    double calDistanceTo(RGB &aim);

//    operator std::vector<uchar>() const;
    friend std::ostream &operator<<(std::ostream &os, const RGB &o);
};


#endif //GRABCUT_RGB_H
