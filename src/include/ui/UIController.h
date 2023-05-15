//
// Created by DeathWind on 2023/5/14.
//

#ifndef GRABCUT_UICONTROLLER_H
#define GRABCUT_UICONTROLLER_H

#include <iostream>
#include <opencv2/opencv.hpp>
#include <unordered_map>
#include <vector>

#include "cut_alg/Pixel.h"

class UIController {
public:
    std::unordered_map<std::string ,cv::Mat> imageMap;

    static std::vector<std::vector<Pixel>> analyseImage(const cv::Mat &image);

    int getImagesFromDirectory(const std::string& path);
    void initInteractivePage();

};


#endif //GRABCUT_UICONTROLLER_H
