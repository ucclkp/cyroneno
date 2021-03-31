// Copyright (c) 2019 ucclkp <ucclkp@gmail.com>.
// This file is part of cyroneno project.
//
// This program is licensed under GPLv3 license that can be
// found in the LICENSE file.

#ifndef CYRONENO_RASTERIZER_CIRCLE_RZ_H_
#define CYRONENO_RASTERIZER_CIRCLE_RZ_H_

#include "../color.hpp"
#include "target.h"
#include "../point.hpp"


namespace cyro {

    /**
     * Based on Alois Zingl "A Rasterizing Algorithm for Drawing Curves"
     */
    class CircleRz {
    public:
        static void draw(
            const Point2I& center, int r, const Color4D& c, Target* target);
        static void draw(
            const Point2I& center, int r,
            int thick, int thick_type, const Color4D& c, Target* target);

        static void drawEllipse(
            const Point2I& center, int a, int b, const Color4D& c, Target* target);

        static void drawAA(
            const Point2& center, double r, const Color4D& c, Target* target);

    private:
        static void drawOld(
            const Point2I& center, int r, const Color4D& c, Target* target);
    };

}

#endif  // CYRONENO_RASTERIZER_CIRCLE_RZ_H_