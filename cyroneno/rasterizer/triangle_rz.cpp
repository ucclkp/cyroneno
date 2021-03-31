// Copyright (c) 2019 ucclkp <ucclkp@gmail.com>.
// This file is part of cyroneno project.
//
// This program is licensed under GPLv3 license that can be
// found in the LICENSE file.

#include "triangle_rz.h"

#include <algorithm>

#include "../equations.h"
#include "point_rz.h"


namespace cyro {

    void TriangleRz::draw(
        const Point2I& p0, const Point2I& p1, const Point2I& p2,
        const Color4D& c0, const Color4D& c1, const Color4D& c2, Target* target)
    {
        Line2DEqu line01(p0, p1);
        Line2DEqu line02(p0, p2);
        Line2DEqu line12(p1, p2);
        auto min_x = std::min({ p0.x, p1.x, p2.x });
        auto max_x = std::max({ p0.x, p1.x, p2.x });
        auto min_y = std::min({ p0.y, p1.y, p2.y });
        auto max_y = std::max({ p0.y, p1.y, p2.y });

        for (int y = min_y; y <= max_y; ++y) {
            for (int x = min_x; x <= max_x; ++x) {
                double α = line01.cal(x, y) / line01.cal(p2.x, p2.y);
                double β = line02.cal(x, y) / line02.cal(p1.x, p1.y);
                double γ = line12.cal(x, y) / line12.cal(p0.x, p0.y);
                if (α > 0 && β > 0 && γ > 0) {
                    auto c = c2 * α + c1 * β + c0 * γ;
                    PointRz::draw(x, y, c, target);
                }
            }
        }
    }

    void TriangleRz::drawAA(
        const Point2& p0, const Point2& p1, const Point2& p2,
        const Color4D& c0, const Color4D& c1, const Color4D& c2, Target* target)
    {
    }

}