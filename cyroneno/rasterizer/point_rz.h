// Copyright (c) 2019 ucclkp <ucclkp@gmail.com>.
// This file is part of cyroneno project.
//
// This program is licensed under GPLv3 license that can be
// found in the LICENSE file.

#ifndef CYRONENO_RASTERIZER_POINT_RZ_H_
#define CYRONENO_RASTERIZER_POINT_RZ_H_

#include "../color.hpp"
#include "target.h"


namespace cyro {

    class PointRz {
    public:
        /**
         * 以指定的颜色绘制指定坐标的位置。
         * 将进行透明度混合。
         */
        static void draw(int x, int y, const Color4D& c, Target* target);

        /**
         * 以指定的颜色绘制指定坐标的位置。
         * 将进行透明度混合。
         * 将在 x 方向和 y 方向上按小数部分插值
         */
        static void drawAA(double x, double y, const Color4D& c, Target* target);

        /**
         * 以指定的颜色绘制指定坐标的位置。
         * 将进行透明度混合。
         * 仅在 x 方向上按小数部分插值，y 值将直接砍掉小数部分
         */
        static void drawXAA(double x, double y, const Color4D& c, Target* target);

        /**
         * 以指定的颜色绘制指定坐标的位置。
         * 将进行透明度混合。
         * 仅在 y 方向上按小数部分插值，x 值将直接砍掉小数部分
         */
        static void drawYAA(double x, double y, const Color4D& c, Target* target);
    };

}

#endif  // CYRONENO_RASTERIZER_POINT_RZ_H_