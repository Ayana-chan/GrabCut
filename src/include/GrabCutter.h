//
// Created by DeathWind on 2023/5/14.
//

#ifndef GRABCUT_GRABCUTTER_H
#define GRABCUT_GRABCUTTER_H

#include "ui/UIController.h"

class GrabCutter {
public:
    GrabCutter();

    void start();

private:
    UIController uiController;
    std::vector<std::vector<Pixel>> imageMat;

    /**
     * 把矩形以外的所有点变成B_MUST。注：默认为F_PROB
     * @param minX
     * @param minY
     * @param maxX
     * @param maxY
     */
    void updateMatByRect(int minX,int minY,int maxX,int maxY);
};


#endif //GRABCUT_GRABCUTTER_H
