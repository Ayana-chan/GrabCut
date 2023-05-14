//
// Created by DeathWind on 2023/5/14.
//

#ifndef GRABCUT_UICONTROLLER_H
#define GRABCUT_UICONTROLLER_H

#include <iostream>
#include <opencv2/opencv.hpp>
#include <unordered_map>

class UIController {
public:
    std::unordered_map<std::string ,cv::Mat> imageMap;

    int getImagesFromDirectory(const std::string& path);
};


#endif //GRABCUT_UICONTROLLER_H
