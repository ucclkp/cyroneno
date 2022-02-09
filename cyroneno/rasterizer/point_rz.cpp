// Copyright (c) 2019 ucclkp <ucclkp@gmail.com>.
// This file is part of cyroneno project.
//
// This program is licensed under GPLv3 license that can be
// found in the LICENSE file.

#include "point_rz.h"

#include <cmath>


namespace cyro {

    void PointRz::draw(int x, int y, const Color4D& c, Target* target) {
        target->drawColor(x, y, c);
    }

    void PointRz::drawAA(double x, double y, const Color4D& c, Target* target) {
        float dx = float(x - std::floor(x));
        float dy = float(y - std::floor(y));

        if (dx == 0 && dy == 0) {
            // 不需要插值
            target->drawColor(int(x), int(y), c);
        } else if (dx > 0 && dy == 0) {
            // x 轴插值
            target->drawColor(int(x), int(y), c * (1 - dx));
            target->drawColor(int(x + 1), int(y), c * dx);
        } else if (dy > 0 && dx == 0) {
            // y 轴插值
            target->drawColor(int(x), int(y), c * (1 - dy));
            target->drawColor(int(x), int(y + 1), c * dy);
        } else {
            // xy 轴插值
            target->drawColor(int(x), int(y), c * ((1 - dx) * (1 - dy)));
            target->drawColor(int(x + 1), int(y), c * (dx * (1 - dy)));
            target->drawColor(int(x), int(y + 1), c * ((1 - dx) * dy));
            target->drawColor(int(x + 1), int(y + 1), c * (dx * dy));
        }
    }

    void PointRz::drawXAA(double x, double y, const Color4D& c, Target* target) {
        float dx = float(x - std::floor(x));

        if (dx == 0) {
            // 不需要插值
            target->drawColor(int(x), int(y), c);
        } else {
            // x 轴插值
            target->drawColor(int(x), int(y), c * (1 - dx));
            target->drawColor(int(x + 1), int(y), c * dx);
        }
    }

    void PointRz::drawYAA(double x, double y, const Color4D& c, Target* target) {
        float dy = float(y - std::floor(y));

        if (dy == 0) {
            // 不需要插值
            target->drawColor(int(x), int(y), c);
        } else {
            // y 轴插值
            target->drawColor(int(x), int(y), c * (1 - dy));
            target->drawColor(int(x), int(y + 1), c * dy);
        }
    }

}