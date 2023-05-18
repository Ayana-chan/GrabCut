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
    this->imageName = imgName;

    return imgName;
}

void UIController::analyseImage(std::vector<std::vector<Pixel>> &aimVec) {

    for (int i = 0; i < this->srcImage.rows; i++) {
        aimVec.emplace_back(std::vector<Pixel>());
        for (int j = 0; j < this->srcImage.cols; j++) {
            RGB rgb(this->srcImage.at<cv::Vec3b>(i, j));
            Pixel pixel(rgb);
            aimVec[i].push_back(pixel);
        }
    }
}

void UIController::drawRect(const std::string &srcImageName) {

    //初始化矩形为整个图像
    this->posX1 = 0;
    this->posY1 = 0;
    this->posX2 = this->srcImage.cols - 1;
    this->posY2 = this->srcImage.rows - 1;
    this->isDragging = false;

    cv::namedWindow(srcImageName);
    cv::setMouseCallback(srcImageName, rectMouseHandler, this); //把当前对象也给传过去

    cv::imshow(srcImageName, this->srcImage);
    //阻塞直到按下回车
    while (true) {
        int key = cv::waitKey() & 0xFF;
        if (key == 13 && !this->isDragging) {
            //按下回车
            break;
        }
    }
    std::cout << "Rect: From (" << this->posY1 << "," << this->posX1 << ") To (" << this->posY2 << "," << this->posX2
              << ")" << std::endl;
}

void UIController::additionalDrawImage() {
    std::cout << "hello1" << std::endl;
    drawnImage = srcImage(rect);
    std::cout << "hello2" << std::endl;
    auto windowName = "Advance";
    cv::namedWindow(windowName);
    cv::setMouseCallback(windowName, advanceMouseHandler, this); //把当前对象也给传过去

    cv::imshow(windowName, drawnImage);
    //阻塞直到按下回车
    while (true) {
        int key = cv::waitKey() & 0xFF;
        if (key == 13 && !this->isDragging) {
            //按下回车
            break;
        }
    }
}


//static
void UIController::rectMouseHandler(int event, int x, int y, int, void *srcUIController) {

    auto *uiCon = (UIController *) srcUIController;

    if (event == cv::EVENT_LBUTTONDOWN && !uiCon->isDragging) {
        //开始拖动
//        std::cout << "Rect: Begin Dragging " << x << " " << y << std::endl;
        uiCon->isDragging = true;
        uiCon->tempPosX1 = x;
        uiCon->tempPosY1 = y;
    } else if (event == cv::EVENT_MOUSEMOVE && uiCon->isDragging) {
        //正在拖动
    } else if (event == cv::EVENT_LBUTTONUP && uiCon->isDragging) {
        //结束拖动
//        std::cout << "Rect: End   Dragging " << x << " " << y << std::endl;
        uiCon->isDragging = false;

        //防止越界（鼠标在图像外释放也会算）
        if (x > uiCon->srcImage.cols - 1) {
            x = uiCon->srcImage.cols - 1;
        }
        if (y > uiCon->srcImage.rows - 1) {
            y = uiCon->srcImage.rows - 1;
        }

        //如果原地点了一下或者拉了直线则不算数
        if (uiCon->tempPosX1 == x || uiCon->tempPosY1 == y) {
            return;
        }

        uiCon->posX2 = x;
        uiCon->posY2 = y;

        uiCon->posX1 = uiCon->tempPosX1;
        uiCon->posY1 = uiCon->tempPosY1;

        //将矩形坐标转化为左上角、右下角
        if (uiCon->posX1 > uiCon->posX2) {
            auto temp = uiCon->posX1;
            uiCon->posX1 = uiCon->posX2;
            uiCon->posX2 = temp;
        }
        if (uiCon->posY1 > uiCon->posY2) {
            auto temp = uiCon->posY1;
            uiCon->posY1 = uiCon->posY2;
            uiCon->posY2 = temp;
        }

        //画矩形
        //备份、还原图片
        uiCon->rectedImage = uiCon->srcImage.clone();
        uiCon->rect.x = uiCon->posX1;
        uiCon->rect.y = uiCon->posY1;
        uiCon->rect.width = uiCon->posX2 - uiCon->posX1;
        uiCon->rect.height = uiCon->posY2 - uiCon->posY1;
        cv::rectangle(uiCon->rectedImage, uiCon->rect, cv::Scalar(0, 0, 255), 2, 8, 0);
        cv::imshow(uiCon->imageName, uiCon->rectedImage);
    }
}

//static
void UIController::advanceMouseHandler(int event, int x, int y, int, void *srcUIController) {

    auto *uiCon = (UIController *) srcUIController;

    //左右要互斥
    if (event == cv::EVENT_LBUTTONDOWN && !uiCon->isDraggingLeft && !uiCon->isDraggingRight) {
        //开始拖动
        std::cout << "Advance: Begin Dragging Left  " << x << " " << y << std::endl;
        uiCon->isDraggingLeft = true;
    } else if (event == cv::EVENT_RBUTTONDOWN && !uiCon->isDraggingLeft && !uiCon->isDraggingRight) {
        std::cout << "Advance: Begin Dragging Right " << x << " " << y << std::endl;
        uiCon->isDraggingRight = true;
    } else if (event == cv::EVENT_MOUSEMOVE) {
        //正在拖动
        if (uiCon->isDraggingLeft) {
            std::cout << "Advance: Move  Dragging Left  " << x << " " << y << std::endl;
            cv::circle(uiCon->drawnImage, cv::Point(x, y), 1, cv::Scalar(255, 255, 255), -1);

        } else if (uiCon->isDraggingRight) {
            cv::circle(uiCon->drawnImage, cv::Point(x, y), 1, cv::Scalar(0, 255, 0), -1);
            std::cout << "Advance: Move  Dragging Right " << x << " " << y << std::endl;
        }
    } else if (event == cv::EVENT_LBUTTONUP) {
        //结束拖动
        std::cout << "Advance: End   Dragging Left  " << x << " " << y << std::endl;
        uiCon->isDraggingLeft = false;
    } else if (event == cv::EVENT_RBUTTONUP) {
        //结束拖动
        std::cout << "Advance: End   Dragging Right " << x << " " << y << std::endl;
        uiCon->isDraggingRight = false;
    }

    //一直刷新
    cv::imshow("Advance", uiCon->drawnImage);
}



