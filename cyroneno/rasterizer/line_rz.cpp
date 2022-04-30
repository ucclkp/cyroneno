// Copyright (c) 2019 ucclkp <ucclkp@gmail.com>.
// This file is part of cyroneno project.
//
// This program is licensed under GPLv3 license that can be
// found in the LICENSE file.

#include "line_rz.h"

#include <algorithm>

#include "common.h"
#include "point_rz.h"
#include "../equations.h"


namespace {

    /**
     *  1 -> 0
     *  0 -> 0
     * -1 -> 1
     */
    int zeroOne(int val) {
        return (1 - val) / 2;
    }

}

namespace cyro {

    void LineRz::draw(
        const Point2I& p0, const Point2I& p1,
        int thick, int thick_type, const Color4D& c, Target* target)
    {
        if (thick < 1) {
            return;
        }
        int dx = std::abs(p1.x() - p0.x());
        int dy = std::abs(p1.y() - p0.y());
        if (dx == 0 && dy == 0) {
            return;
        }

        int x_inc = sgn(p1.x() - p0.x());
        int y_inc = sgn(p1.y() - p0.y());

        int xk = p0.x();
        int yk = p0.y();
        int ek = dx - dy;

        int x_off, y_off;
        int x_step, y_step;
        if (thick_type == THICK_CENTER) {
            // 从中间往两边扩散的话，如果要做到对称，
            // 线宽就必须是奇数
            if (thick && (thick & 1) == 0) {
                ++thick;
            }
            x_off = -thick / 2 * x_inc;
            y_off = -thick / 2 * y_inc;
            x_step = x_inc;
            y_step = y_inc;
        } else if (thick_type == THICK_END) {
            x_off = 0;
            y_off = 0;
            x_step = x_inc;
            y_step = y_inc;
        } else {
            x_off = 0;
            y_off = 0;
            x_step = -x_inc;
            y_step = -y_inc;
        }

        for (;;) {
            if (dx >= dy) {
                int y = yk + y_off;
                for (int i = 0; i < thick; ++i) {
                    PointRz::draw(xk, y, c, target);
                    y += y_step;
                }
            } else {
                int x = xk + x_off;
                for (int i = 0; i < thick; ++i) {
                    PointRz::draw(x, yk, c, target);
                    x += x_step;
                }
            }

            int exy = ek;
            int ex = exy + dy;
            int ey = exy - dx;
            if (ex + exy >= zeroOne(y_inc)) {
                if (xk == p1.x()) return;
                xk += x_inc;
                // 将误差值移动至新的对角线像素
                ek -= dy;
            }
            if (ey + exy <= -zeroOne(y_inc)) {
                if (yk == p1.y()) return;
                yk += y_inc;
                // 将误差值移动至新的对角线像素
                ek += dx;
            }
        }
    }

    void LineRz::drawSeg(
        const Point2I& p0, const Point2I& p1, const Color4D& c, Target* target)
    {
        int x0 = p0.x(), y0 = p0.y();
        int x1 = p1.x(), y1 = p1.y();

        int dx = std::abs(x1 - x0);
        int dy = std::abs(y1 - y0);

        int x_inc = sgn(x1 - x0);
        int y_inc = sgn(y1 - y0);

        int xk = x0;
        int yk = y0;
        int prev_xk = xk;
        int prev_yk = yk;
        int ek = dx - dy;

        for (;;) {
            int exy = ek;
            int ex = exy + dy;
            int ey = exy - dx;
            if (ex + exy >= zeroOne(x_inc)) {
                if (xk == x1) return;
                xk += x_inc;
                ek -= dy;
            }
            if (ey + exy <= -zeroOne(x_inc)) {
                if (yk == y1) return;
                yk += y_inc;
                ek += dx;
            }

            PointRz::draw(prev_xk, prev_yk, c, target);
            prev_xk = xk;
            prev_yk = yk;
        }
    }

    void LineRz::drawAA(
        const Point2& p0, const Point2& p1,
        double thick, const Color4D& c, Target* target)
    {
        double dx = std::abs(p1.x() - p0.x());
        double dy = std::abs(p1.y() - p0.y());
        if (dx < std::numeric_limits<double>::epsilon() &&
            dy < std::numeric_limits<double>::epsilon())
        {
            return;
        }

        int x_sign = sgn2(p1.x() - p0.x());
        int y_sign = sgn2(p1.y() - p0.y());

        int xk = int(x_sign == 1 ? std::floor(p0.x()) : std::ceil(p0.x()));
        int yk = int(y_sign == 1 ? std::floor(p0.y()) : std::ceil(p0.y()));
        int x1 = int(x_sign == 1 ? std::ceil(p1.x()) : std::floor(p1.x()));
        int y1 = int(y_sign == 1 ? std::ceil(p1.y()) : std::floor(p1.y()));

        int x_inc = sgn(x1 - xk);
        int y_inc = sgn(y1 - yk);

        // 初始误差
        double ek = dx * (yk - p0.y())*y_sign - dy * (xk - p0.x())*x_sign + (dx - dy);

        // 等式两边因子为 1 时的门限值
        double th = std::sqrt(dx*dx + dy*dy);

        // 从扩张中心到线边缘的门限值
        double thick_th = th * (thick + 1) / 2;

        // 从扩张中心到线边缘的路程
        double thick_dis;
        if (thick < 1) thick_dis = thick - 1;
        else thick_dis = (thick - 1) / 2;

        for (;;) {
            double cur_e = ek - (dx - dy);
            if (dx >= dy) {
                int count = int(std::ceil(thick_th / dx));
                double tmp_e = cur_e - dx * count;
                double ex_a;
                // 处理浮点数头/尾
                if ((xk - p0.x())*x_sign < 0) ex_a = 1 - std::abs(p0.x() - xk);
                else if ((xk - p1.x())*x_sign > 0) ex_a = 1 - std::abs(p1.x() - xk);
                else ex_a = 1;

                int y = yk - y_sign * count;
                for (int i = 0; i <= 2 * count; ++i) {
                    float alpha = float((1.0 - std::min(std::max(std::abs(tmp_e) / th - thick_dis, 0.0), 1.0)) * ex_a);
                    PointRz::draw(xk, y, c.mulA(alpha), target);
                    tmp_e += dx;
                    y += y_sign;
                }
            } else {
                int count = int(std::ceil(thick_th / dy));
                double tmp_e = cur_e + dy * count;
                double ex_a;
                // 处理浮点数头/尾
                if ((yk - p0.y())*y_sign < 0) ex_a = 1 - std::abs(p0.y() - yk);
                else if ((yk - p1.y())*y_sign > 0) ex_a = 1 - std::abs(p1.y() - yk);
                else ex_a = 1;

                int x = xk - x_sign * count;
                for (int i = 0; i <= 2 * count; ++i) {
                    float alpha = float((1.0 - std::min(std::max(std::abs(tmp_e) / th - thick_dis, 0.0), 1.0)) * ex_a);
                    PointRz::draw(x, yk, c.mulA(alpha), target);
                    tmp_e -= dy;
                    x += x_sign;
                }
            }

            double exy = ek;
            double ex = exy + dy;
            double ey = exy - dx;
            if (ex + exy >= 0) {
                if (xk == x1) return;
                xk += x_inc;
                // 将误差值移动至新的对角线像素。下同。
                ek -= dy;
            }
            if (ey + exy <= 0) {
                if (yk == y1) return;
                yk += y_inc;
                ek += dx;
            }
        }
    }

}