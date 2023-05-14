//
// Created by DeathWind on 2023/5/14.
//

#include "ui/UIController.h"

int UIController::getImagesFromDirectory(const std::string& path) {
    std::vector<cv::String> fileFullNameList;
    cv::glob(path, fileFullNameList);

    for (auto & fileName : fileFullNameList) {

        int pos = fileName.find_last_of('\\');
        std::string img_name(fileName.substr(pos + 1));
        std::cout << "read new image: "<<img_name << std::endl;

        cv::Mat img = cv::imread(fileName);
        if (!img.data) {
            continue;
        }
        this->imageMap[fileName]=img;
    }

    return this->imageMap.size();
}

