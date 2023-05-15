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

    ~GrabCutter();

private:
    UIController uiController;
};


#endif //GRABCUT_GRABCUTTER_H
