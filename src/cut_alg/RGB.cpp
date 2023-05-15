//
// Created by DeathWind on 2023/5/14.
//

#include <iostream>

#include "cut_alg/RGB.h"

RGB::RGB(unsigned char r, unsigned char g, unsigned char b) : r(r), g(g), b(b) {}

//输入顺序为BGR
RGB::RGB(cv::Vec3b vec) : r(vec[2]), g(vec[1]), b(vec[0])  {}
