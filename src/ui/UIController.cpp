//
// Created by DeathWind on 2023/5/14.
//

#include "ui/UIController.h"
#include "GrabCutter.h"

UIController::UIController(GrabCutter *grabCutter) {
    this->master = grabCutter;
}

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
    posX1 = 0;
    posY1 = 0;
    posX2 = srcImage.cols - 1;
    posY2 = srcImage.rows - 1;
    rect.x = posX1;
    rect.y = posY1;
    rect.width = posX2 - posX1;
    rect.height = posY2 - posY1;
    isDragging = false;

    cv::namedWindow(srcImageName);
    cv::setMouseCallback(srcImageName, rectMouseHandler, this); //把当前对象也给传过去

    cv::imshow(srcImageName, this->srcImage);
    //阻塞直到按下回车
    while (true) {
        int key = cv::waitKey() & 0xFF;
        if (key == 13 && !isDragging) {
            //按下回车
            break;
        }
    }
    std::cout << "Rect: From (" << this->posY1 << "," << this->posX1 << ") To (" << this->posY2 << "," << this->posX2
              << ")" << std::endl;
}

bool UIController::additionalDrawImage() {

    drawnImage = srcImage(rect);
    auto windowName = "Advance";
    cv::namedWindow(windowName);
    cv::setMouseCallback(windowName, advanceMouseHandler, this); //把当前对象也给传过去

    cv::imshow(windowName, drawnImage);
    //阻塞直到按下回车
    while (true) {
        int key = cv::waitKey() & 0xFF;
        if (key == 13 && !this->isDragging) {
            //按下回车
            return true;
        }else if(key == 'x'){
            return false;
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
        cv::imshow(uiCon->imageName, uiCon->rectedImage); //TODO 更顺滑的绘画
    }
}

//static
void UIController::advanceMouseHandler(int event, int x, int y, int, void *srcUIController) {

    auto *uiCon = (UIController *) srcUIController;

    auto leftColor = cv::Scalar(255, 255, 255);
    auto rightColor = cv::Scalar(0, 255, 0);

    int radius = 1;

    //左右要互斥
    if (event == cv::EVENT_LBUTTONDOWN && !uiCon->isDraggingLeft && !uiCon->isDraggingRight) {
        //开始拖动
//        std::cout << "Advance: Begin Dragging Left  " << x << " " << y << std::endl;
        uiCon->isDraggingLeft = true;
        cv::circle(uiCon->drawnImage, cv::Point(x, y), radius, leftColor, -1);
    } else if (event == cv::EVENT_RBUTTONDOWN && !uiCon->isDraggingLeft && !uiCon->isDraggingRight) {
        //开始拖动
//        std::cout << "Advance: Begin Dragging Right " << x << " " << y << std::endl;
        uiCon->isDraggingRight = true;
        cv::circle(uiCon->drawnImage, cv::Point(x, y), radius, rightColor, -1);
    } else if (event == cv::EVENT_MOUSEMOVE) {
        //绘制的时候，半径若为1，则会画出十字形，中上下左右五个点
        if (uiCon->isDraggingLeft || uiCon->isDraggingRight) {
            //左键为背景，右键为前景
            int alphaFlag;//表示目标flag，0为背景1为前景
            //正在拖动
            if (uiCon->isDraggingLeft) {
//            std::cout << "Advance: Move  Dragging Left  " << x << " " << y << std::endl;
                cv::circle(uiCon->drawnImage, cv::Point(x, y), radius, leftColor, -1);
                alphaFlag=0;
            } else {
                cv::circle(uiCon->drawnImage, cv::Point(x, y), radius, rightColor, -1);
//            std::cout << "Advance: Move  Dragging Right " << x << " " << y << std::endl;
                alphaFlag=1;
            }
            //修改alpha
            int rows=uiCon->drawnImage.rows;
            int cols=uiCon->drawnImage.cols;
            //防止界外涂鸦
            if(x>=0 && x<cols && y>=0 && y<rows){
                uiCon->master->modifyAlphaByUser(x,y,alphaFlag);
                if(x>0){
                    uiCon->master->modifyAlphaByUser(x-1,y,alphaFlag);
                }
                if(x<cols-1){
                    uiCon->master->modifyAlphaByUser(x+1,y,alphaFlag);
                }
                if(y>0){
                    uiCon->master->modifyAlphaByUser(x,y-1,alphaFlag);
                }
                if(y<rows-1){
                    uiCon->master->modifyAlphaByUser(x,y+1,alphaFlag);
                }
            }
        }
    } else if (event == cv::EVENT_LBUTTONUP) {
        //结束拖动
//        std::cout << "Advance: End   Dragging Left  " << x << " " << y << std::endl;
        uiCon->isDraggingLeft = false;
    } else if (event == cv::EVENT_RBUTTONUP) {
        //结束拖动
//        std::cout << "Advance: End   Dragging Right " << x << " " << y << std::endl;
        uiCon->isDraggingRight = false;
    }

    //一直刷新
    cv::imshow("Advance", uiCon->drawnImage);
}





