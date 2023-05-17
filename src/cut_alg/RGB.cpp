//
// Created by DeathWind on 2023/5/14.
//

#include <iostream>

#include "cut_alg/RGB.h"

RGB::RGB(unsigned char r, unsigned char g, unsigned char b) : r(r), g(g), b(b) {}

//输入顺序为BGR
RGB::RGB(cv::Vec3b vec) : r(vec[2]), g(vec[1]), b(vec[0])  {}

double RGB::calDistanceTo(RGB &aim) {
    double ans = std::pow(r - aim.r, 2) +
                 std::pow(g - aim.g, 2) +
                 std::pow(b - aim.b, 2);
    return ans;
}

//RGB::operator std::vector<uchar>() const {
//    std::vector<uchar> res(3);
//    res[0]=this->r;
//    res[1]=this->g;
//    res[2]=this->b;
//    return res;
//}

std::ostream &operator<<(std::ostream &os, const RGB &o) {
    os<<"(R:"<<(int)o.r<<",G:"<<(int)o.g<<",B:"<<(int)o.b<<")";
    return os;
}



