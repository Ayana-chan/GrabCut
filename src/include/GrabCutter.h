//
// Created by DeathWind on 2023/5/14.
//

#ifndef GRABCUT_GRABCUTTER_H
#define GRABCUT_GRABCUTTER_H

#include "common.h"
#include "cut_alg/GMM.h"
#include "ui/UIController.h"

class GrabCutter {
public:
    GrabCutter();

    void start(std::string path);

    ~GrabCutter()=default;

private:
    UIController uiController;
    ImageMat imageMat;
    GMM bkGMM;
    GMM frGMM;

    /**
     * 把矩形以外的所有点变成B_MUST。注：默认为F_PROB
     * @param minX
     * @param minY
     * @param maxX
     * @param maxY
     */
    void updateMatByRect(int minX,int minY,int maxX,int maxY);

    void initGMM();
};


#endif //GRABCUT_GRABCUTTER_H
