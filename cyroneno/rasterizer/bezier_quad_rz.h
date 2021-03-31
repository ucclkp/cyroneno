// Copyright (c) 2019 ucclkp <ucclkp@gmail.com>.
// This file is part of cyroneno project.
//
// This program is licensed under GPLv3 license that can be
// found in the LICENSE file.

#ifndef CYRONENO_RASTERIZER_BEZIER_QUAD_RZ_H_
#define CYRONENO_RASTERIZER_BEZIER_QUAD_RZ_H_

#include "../color.hpp"
#include "target.h"
#include "../point.hpp"


namespace cyro {

    /**
     * Based on Alois Zingl "A Rasterizing Algorithm for Drawing Curves"
     */
    class BezierQuadRz {
    public:
        static void draw(
            const Point2I& p1, const Point2I& p2, const Point2I& p3,
            const Color4D& c, Target* target);
        static void draw(
            const Point2& p1, const Point2& p2, const Point2& p3,
            double w0, double w1, double w2,
            const Color4D& c, Target* target);
        static void drawAA(
            const Point2& p1, const Point2& p2, const Point2& p3, const Color4D& c, Target* target);

    private:
        static void drawSegment(
            const Point2I& p1, const Point2I& p2, const Point2I& p3, const Color4D& c, Target* target);

        static void drawParam(
            const Point2& p1, const Point2& p2, const Point2& p3,
            const Color4D& c, Target* target);
        static void drawParam(
            const Point2& p1, const Point2& p2, const Point2& p3,
            double w0, double w1, double w2,
            const Color4D& c, Target* target);
    };

}

#endif  // CYRONENO_RASTERIZER_BEZIER_QUAD_RZ_H_