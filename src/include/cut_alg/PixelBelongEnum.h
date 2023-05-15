//
// Created by DeathWind on 2023/5/15.
//

#ifndef GRABCUT_PIXELBELONGENUM_H
#define GRABCUT_PIXELBELONGENUM_H


enum class PixelBelongEnum {
    B_MUST = 0, //用户指定的背景
    B_PROB = 1, //归类为背景
    F_MUST = 2, //用户指定的前景
    F_PROB = 3, //归类为前景
};


#endif //GRABCUT_PIXELBELONGENUM_H
