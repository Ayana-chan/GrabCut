//
// Created by DeathWind on 2023/5/14.
//

#include "cut_alg/Pixel.h"

Pixel::Pixel(const RGB &rgb) : rgb(rgb), alpha(0), k(0) {}

Pixel::Pixel(const RGB &rgb, unsigned char alpha) : rgb(rgb), alpha(alpha), k(0) {}
