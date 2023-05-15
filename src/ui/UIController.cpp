//
// Created by DeathWind on 2023/5/14.
//

#include "ui/UIController.h"


std::string UIController::getImagesFromPath(const std::string &path) {
    int pos = path.find_last_of('\\');
    std::string imgName(path.substr(pos + 1));
    std::cout << "read new image: " << imgName << " at " << path << std::endl;

    cv::Mat img = cv::imread(path);
    if (!img.data) {
        std::cerr << "WARNING: No Data" << std::endl;
    }
    this->srcImage = img;
    this->imageName=imgName;

    return imgName;
}

std::vector<std::vector<Pixel>> UIController::analyseImage() {
    std::vector<std::vector<Pixel>> analysedImage;

    for (int i = 0; i < this->srcImage.rows; i++) {
        analysedImage.push_back(*new std::vector<Pixel>());
        for (int j = 0; j < this->srcImage.cols; j++) {
            RGB rgb(this->srcImage.at<cv::Vec3b>(i, j));
            Pixel pixel(rgb);
            analysedImage[i].push_back(pixel);
        }
    }

    return analysedImage;//应该会触发返回值优化
}

void tempFunc(int state, void *pointer) {
    std::cout << "tempFunc" << std::endl;
}

void UIController::drawRect(const std::string &srcImageName) {

    //初始化矩形为整个图像
    this->posX1 = 0;
    this->posY1 = 0;
    this->posX2 = this->srcImage.cols - 1;
    this->posY2 = this->srcImage.rows - 1;
    this->isDragging = false;

    cv::namedWindow(srcImageName);
    cv::setMouseCallback(srcImageName, mouseHandler, this); //把当前对象也给穿过去

    cv::imshow(srcImageName, this->srcImage);
    //阻塞直到按下回车
    while (true) {
        int key = cv::waitKey() & 0xFF;
        if (key == 13 && !this->isDragging) {
            //按下回车
            break;
        }
    }
}

void UIController::mouseHandler(int event, int x, int y, int, void *srcUIController) {

    auto *uiCon = (UIController *) srcUIController;

    if (event == cv::EVENT_LBUTTONDOWN && !uiCon->isDragging) {
        //开始拖动
        std::cout << "Rect: Begin Dragging " << x << " " << y << std::endl;
        uiCon->isDragging = true;
        uiCon->posX1 = x;
        uiCon->posY1 = y;
    }else if(event == cv::EVENT_MOUSEMOVE && uiCon->isDragging){
        //正在拖动
    }else if (event == cv::EVENT_LBUTTONUP && uiCon->isDragging) {
        //结束拖动
        std::cout << "Rect: End   Dragging " << x << " " << y << std::endl;
        uiCon->isDragging = false;
        uiCon->posX2 = x;
        uiCon->posY2 = y;

        //将矩形坐标转化为左上角、右下角
        if(uiCon->posX1>uiCon->posX2){
            auto temp=uiCon->posX1;
            uiCon->posX1=uiCon->posX2;
            uiCon->posX2=temp;
        }
        if(uiCon->posY1>uiCon->posY2){
            auto temp=uiCon->posY1;
            uiCon->posY1=uiCon->posY2;
            uiCon->posY2=temp;
        }

        //画矩形
        //备份、还原图片
        uiCon->drawnImage=uiCon->srcImage.clone();
        cv::Rect rect;
        rect.x = uiCon->posX1;
        rect.y = uiCon->posY1;
        rect.width = uiCon->posX2-uiCon->posX1;
        rect.height = uiCon->posY2-uiCon->posY1;
        cv::rectangle(uiCon->drawnImage, rect, cv::Scalar(0, 0, 255), 2, 8, 0);
        cv::imshow(uiCon->imageName,uiCon->drawnImage);
    }
}

