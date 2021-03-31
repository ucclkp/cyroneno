// Copyright (c) 2019 ucclkp <ucclkp@gmail.com>.
// This file is part of cyroneno project.
//
// This program is licensed under GPLv3 license that can be
// found in the LICENSE file.

#ifndef CYRONENO_RASTERIZER_TRIANGLE_RZ_H_
#define CYRONENO_RASTERIZER_TRIANGLE_RZ_H_

#include "../color.hpp"
#include "target.h"
#include "../point.hpp"


namespace cyro {

    class TriangleRz {
    public:
        static void draw(
            const Point2I& p0, const Point2I& p1, const Point2I& p2,
            const Color4D& c0, const Color4D& c1, const Color4D& c2, Target* target);
        static void drawAA(
            const Point2& p0, const Point2& p1, const Point2& p2,
            const Color4D& c0, const Color4D& c1, const Color4D& c2, Target* target);
    };

}

#endif  // CYRONENO_RASTERIZER_TRIANGLE_RZ_H_