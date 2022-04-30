// Copyright (c) 2019 ucclkp <ucclkp@gmail.com>.
// This file is part of cyroneno project.
//
// This program is licensed under GPLv3 license that can be
// found in the LICENSE file.

#include "circle_rz.h"

#include "common.h"
#include "point_rz.h"


namespace cyro {

    void CircleRz::draw(
        const Point2I& center, int r, const Color4D& c, Target* target)
    {
        if (r <= 0) {
            return;
        }

        int xk = -r;
        int yk = 0;
        int ek = 2 * (1 - r);

        PointRz::draw(-r + center.x(), 0 + center.y(), c, target);
        PointRz::draw(0 + center.x(), r + center.y(), c, target);
        PointRz::draw(r + center.x(), 0 + center.y(), c, target);
        PointRz::draw(0 + center.x(), -r + center.y(), c, target);

        while (xk < 0) {
            int exy = ek;
            int ex = exy - (2 * xk + 1);
            int ey = exy - (2 * yk + 1);

            if (ey + exy <= 0) {
                ++yk;
                ek += (2 * yk + 1);
            }
            if (ex + exy >= 0/* || ek - (2 * yk + 1) + ek > 0*/) {
                ++xk;
                ek += (2 * xk + 1);
            }

            if (xk != 0) {
                PointRz::draw(xk  + center.x(), yk  + center.y(), c, target);
                PointRz::draw(-xk + center.x(), yk  + center.y(), c, target);
                PointRz::draw(xk  + center.x(), -yk + center.y(), c, target);
                PointRz::draw(-xk + center.x(), -yk + center.y(), c, target);
            }
        }
    }

    void CircleRz::draw(
        const Point2I& center, int r,
        int thick, int thick_type, const Color4D& c, Target* target)
    {
        if (r < 1 || thick < 1) {
            return;
        }

        int r_in;
        int r_out;
        if (thick_type == THICK_CENTER) {
            // 从中间往两边扩散的话，如果要做到对称，
            // 线宽就必须是奇数
            if (thick && (thick & 1) == 0) {
                ++thick;
            }
            r_in = r - thick / 2;
            r_out = r + thick / 2;
            if (r_in < 1) {
                r_in = 1;
                thick = r_out - r_in + 1;
            }
        } else if (thick_type == THICK_END) {
            r_in = r;
            r_out = r + thick - 1;
        } else {
            r_in = r - thick + 1;
            r_out = r;
            if (r_in < 1) {
                r_in = 1;
                thick = r_out - r_in + 1;
            }
        }

        int xk_in = -r_in;
        int yk_in = 0;

        int xk_out = -r_out;
        int yk_out = 0;

        int ek_in = 2 * (1 - r_in);
        int ek_out = 2 * (1 - r_out);

        for (int i = 0; i < thick; ++i) {
            PointRz::draw(-r_in - i + center.x(), 0 + center.y(), c, target); // 左
            PointRz::draw(0 + center.x(), r_in + i + center.y(), c, target);  // 上
            PointRz::draw(r_in + i + center.x(), 0 + center.y(), c, target);  // 右
            PointRz::draw(0 + center.x(), -r_in - i + center.y(), c, target); // 下
        }

        int prev_x_out = xk_out - 1;
        while (xk_out < 0) {
            int exy_out = ek_out;
            int ex_out = exy_out - (2 * xk_out + 1);
            int ey_out = exy_out - (2 * yk_out + 1);

            // out
            if (ey_out + exy_out <= 0) {
                ++yk_out;
                ek_out += (2 * yk_out + 1);
            }
            if (ex_out + exy_out >= 0/* || ek - (2 * yk + 1) + ek > 0*/) {
                ++xk_out;
                ek_out += (2 * xk_out + 1);
            }

            if (xk_out > -r_in && prev_x_out != xk_out) {
                // in
                while (xk_in < 0) {
                    int exy_in = ek_in;
                    int ex_in = exy_in - (2 * xk_in + 1);
                    int ey_in = exy_in - (2 * yk_in + 1);

                    if (ey_in + exy_in <= 0) {
                        ++yk_in;
                        ek_in += (2 * yk_in + 1);
                    }
                    if (ex_in + exy_in >= 0/* || ek - (2 * yk + 1) + ek > 0*/) {
                        ++xk_in;
                        ek_in += (2 * xk_in + 1);
                        break;
                    }
                }
            }

            if (xk_out != 0) {
                if (prev_x_out != xk_out) {
                    int start;
                    if (xk_out > -r_in) {
                        start = yk_in;
                    } else if (xk_out == -r_in) {
                        start = yk_in + 1;
                    } else {
                        start = 1;
                    }
                    for (int i = start; i < yk_out; ++i) {
                        PointRz::draw( xk_out + center.x(),  i + center.y(), c, target);
                        PointRz::draw(-xk_out + center.x(),  i + center.y(), c, target);
                        PointRz::draw( xk_out + center.x(), -i + center.y(), c, target);
                        PointRz::draw(-xk_out + center.x(), -i + center.y(), c, target);
                    }
                    ++prev_x_out;
                }

                PointRz::draw( xk_out + center.x(),  yk_out + center.y(), c, target);
                PointRz::draw(-xk_out + center.x(),  yk_out + center.y(), c, target);
                PointRz::draw( xk_out + center.x(), -yk_out + center.y(), c, target);
                PointRz::draw(-xk_out + center.x(), -yk_out + center.y(), c, target);
            }
        }
    }

    void CircleRz::drawEllipse(
        const Point2I& center, int a, int b, const Color4D& c, Target* target)
    {
        if (a <= 0 || b <= 0) {
            return;
        }

        int xk = -a;
        int yk = 0;
        int ek = a * a - b * b * (2 * a - 1);

        PointRz::draw(-a + center.x(),  0 + center.y(), c, target);
        PointRz::draw( 0 + center.x(),  b + center.y(), c, target);
        PointRz::draw( a + center.x(),  0 + center.y(), c, target);
        PointRz::draw( 0 + center.x(), -b + center.y(), c, target);

        while (xk < 0) {
            int exy = ek;
            int ex = exy - (2 * xk + 1)*b*b;
            int ey = exy - (2 * yk + 1)*a*a;

            if (ex + exy >= 0) {
                ++xk;
                ek += (2 * xk + 1)*b*b;
            }
            if (ey + exy <= 0) {
                ++yk;
                ek += (2 * yk + 1)*a*a;
            }

            if (xk != 0) {
                PointRz::draw( xk + center.x(),  yk + center.y(), c, target);
                PointRz::draw(-xk + center.x(),  yk + center.y(), c, target);
                PointRz::draw( xk + center.x(), -yk + center.y(), c, target);
                PointRz::draw(-xk + center.x(), -yk + center.y(), c, target);
            }
        }

        while (yk < b) {
            PointRz::draw( xk + center.x(),  yk + center.y(), c, target);
            PointRz::draw(-xk + center.x(), -yk + center.y(), c, target);
            ++yk;
        }
    }

    void CircleRz::drawAA(
        const Point2& center, double r, const Color4D& c, Target* target)
    {
    }

    void CircleRz::drawOld(const Point2I& center, int r, const Color4D& c, Target* target) {
        if (r <= 0) {
            return;
        }

        int pk = 1 - r;

        int xk = 0;
        int yk = r;
        PointRz::draw( xk + center.x(),  yk + center.y(), c, target);
        PointRz::draw( yk + center.y(),  xk + center.x(), c, target);
        PointRz::draw( xk + center.x(), -yk + center.y(), c, target);
        PointRz::draw(-yk + center.y(),  xk + center.x(), c, target);

        int _2xk = 0;
        int _2yk = 2 * r;

        while (xk < yk) {
            ++xk;
            _2xk += 2;
            if (pk <= 0) {
                pk += _2xk + 1;
            } else {
                --yk;
                _2yk -= 2;
                pk += _2xk + 1 - _2yk;
            }

            if (xk <= yk) {
                PointRz::draw( xk + center.x(),  yk + center.y(), c, target);
                PointRz::draw(-xk + center.x(),  yk + center.y(), c, target);
                PointRz::draw( xk + center.x(), -yk + center.y(), c, target);
                PointRz::draw(-xk + center.x(), -yk + center.y(), c, target);
                if (xk != yk) {
                    PointRz::draw( yk + center.y(),  xk + center.x(), c, target);
                    PointRz::draw(-yk + center.y(),  xk + center.x(), c, target);
                    PointRz::draw( yk + center.y(), -xk + center.x(), c, target);
                    PointRz::draw(-yk + center.y(), -xk + center.x(), c, target);
                }
            }
        }
    }

}