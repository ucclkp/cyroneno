// Copyright (c) 2019 ucclkp <ucclkp@gmail.com>.
// This file is part of cyroneno project.
//
// This program is licensed under GPLv3 license that can be
// found in the LICENSE file.

#ifndef CYRONENO_RASTERIZER_LINE_RZ_H_
#define CYRONENO_RASTERIZER_LINE_RZ_H_

#include "../color.hpp"
#include "target.h"
#include "../point.hpp"

namespace cyro {

    /**
     * Based on Alois Zingl "A Rasterizing Algorithm for Drawing Curves"
     */
    class LineRz {
    public:
        static void draw(
            const Point2I& p0, const Point2I& p1,
            int thick, int thick_type, const Color4D& c, Target* target);
        static void drawSeg(
            const Point2I& p0, const Point2I& p1,
            const Color4D& c, Target* target);
        static void drawAA(
            const Point2& p0, const Point2& p1,
            double thick, const Color4D& c, Target* target);
    };

}

#endif  // CYRONENO_RASTERIZER_LINE_RZ_H_