//
// Created by DeathWind on 2023/5/14.
//

#include "ui/UIController.h"


int UIController::getImagesFromDirectory(const std::string &path) {
    std::vector<cv::String> fileFullNameList;
    cv::glob(path, fileFullNameList);

    for (auto &fileName : fileFullNameList) {

        int pos = fileName.find_last_of('\\');
        std::string img_name(fileName.substr(pos + 1));
        std::cout << "read new image: " << img_name << std::endl;

        cv::Mat img = cv::imread(fileName);
        if (!img.data) {
            continue;
        }
        this->imageMap[fileName] = img;
    }

    return this->imageMap.size();
}

std::vector<std::vector<Pixel>> UIController::analyseImage(const cv::Mat &image) {
    std::vector<std::vector<Pixel>> analysedImage;

    for (int i = 0; i < image.rows; i++) {
        analysedImage.push_back(*new std::vector<Pixel>());
        for (int j = 0; j < image.cols; j++) {
            RGB rgb = *new RGB(image.at<uchar>(i, j));
            Pixel pixel = *new Pixel(rgb);
            analysedImage[i].push_back(pixel);
        }
    }

    return analysedImage;//应该会触发返回值优化
}
void tempFunc(int state, void *pointer){
    std::cout<<"tempFunc"<<std::endl;
}
void UIController::initInteractivePage() {
    cv::createButton("abc", tempFunc);
    cv::waitKey(0);
}

