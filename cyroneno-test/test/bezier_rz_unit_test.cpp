// Copyright (c) 2019 ucclkp <ucclkp@gmail.com>.
// This file is part of cyroneno project.
//
// This program is licensed under GPLv3 license that can be
// found in the LICENSE file.

#include "cyroneno-test/test/bezier_rz_unit_test.h"

#include "cyroneno/rasterizer/common.h"
#include "cyroneno/rasterizer/rasterizer.h"


namespace cyro {
namespace test {

    void TEST_QUAD_BEZIERS(Rasterizer& rz) {
        Point2 p1{200, 400};
        Point2 p3{400, 360};
        for (int i = 0; i < 360; i += 2) {
            double rad = (3.14159265359 / 180) * i;
            double x = 400 * std::sin(rad);
            double y = 400 * std::cos(rad);
            double x2 = std::round((p1.x() + p3.x()) / 2 + x);
            double y2 = std::round((p1.y() + p3.y()) / 2 + y);

            rz.drawQuadBezier(p1, { x2, y2 }, p3, Color4D(0, 0, 0, 0.5f));
        }

        if (false)
        {
            // 斜率较大
            rz.drawQuadBezier({ 400, -40 + 400 }, { 400, 400 }, { 10 + 400, 18 + 400 }, Color4D(0, 0, 0, 0.5f));
            rz.drawQuadBezier({ 10 + 410, 18 + 400 }, { 410, 400 }, { 410, -40 + 400 }, Color4D(0, 0, 0, 0.5f));

            rz.drawQuadBezier({ 450, -40 + 400 }, { 450, 400 }, { -10 + 450, 18 + 400 }, Color4D(0, 0, 0, 0.5f));
            rz.drawQuadBezier({ -10 + 460, 18 + 400 }, { 460, 400 }, { 460, -40 + 400 }, Color4D(0, 0, 0, 0.5f));

            rz.drawQuadBezier({ 400, 40 + 450 }, { 400, 450 }, { 10 + 400, -18 + 450 }, Color4D(0, 0, 0, 0.5f));
            rz.drawQuadBezier({ 10 + 410, -18 + 450 }, { 410, 450 }, { 410, 40 + 450 }, Color4D(0, 0, 0, 0.5f));

            rz.drawQuadBezier({ 450, 40 + 450 }, { 450, 450 }, { -10 + 450, -18 + 450 }, Color4D(0, 0, 0, 0.5f));
            rz.drawQuadBezier({ -10 + 460, -18 + 450 }, { 460, 450 }, { 460, 40 + 450 }, Color4D(0, 0, 0, 0.5f));

            // 斜率较小
            rz.drawQuadBezier({ -40 + 400, 400 }, { 400, 400 }, { 18 + 400, 10 + 400 }, Color4D(0, 0, 0, 0.5f));
            rz.drawQuadBezier({ 18 + 400, 10 + 410 }, { 400, 410 }, { -40 + 400, 410 }, Color4D(0, 0, 0, 0.5f));

            rz.drawQuadBezier({ -40 + 400, 450 }, { 400, 450 }, { 18 + 400, -10 + 450 }, Color4D(0, 0, 0, 0.5f));
            rz.drawQuadBezier({ 18 + 400, -10 + 460 }, { 400, 460 }, { -40 + 400, 460 }, Color4D(0, 0, 0, 0.5f));

            rz.drawQuadBezier({ 40 + 450, 400 }, { 450, 400 }, { -18 + 450, 10 + 400 }, Color4D(0, 0, 0, 0.5f));
            rz.drawQuadBezier({ -18 + 450, 10 + 410 }, { 450, 410 }, { 40 + 450, 410 }, Color4D(0, 0, 0, 0.5f));

            rz.drawQuadBezier({ 40 + 450, 450 }, { 450, 450 }, { -18 + 450, -10 + 450 }, Color4D(0, 0, 0, 0.5f));
            rz.drawQuadBezier({ -18 + 450, -10 + 460 }, { 450, 460 }, { 40 + 450, 460 }, Color4D(0, 0, 0, 0.5f));

            //rz.drawBezier({ -20 + 400, 15 + 400 }, { 400, 400 }, { 10 + 400, -20 + 400 }, Color(0, 0, 0, 0.5f));
            //rz.drawBezier({ 420, 418 }, { 400, 400 }, { 410, 360 }, Color(0, 0, 0, 0.5f));
        }

        rz.drawQuadBezier(
            { 400, 400 }, { 200, 450 }, { 400, 500 },
            0.5, 0.01, 0.5,
            Color4D(0, 0, 0, 0.5f));

        if (false)
        {
            rz.drawQuadBezier({ 400, 400 }, { 200, 450 }, { 400, 500 }, Color4D(0, 0, 0, 0.5f));
            rz.drawQuadBezier({ 400, 500 }, { 600, 550 }, { 400, 600 }, Color4D(0, 0, 0, 0.5f));

            rz.drawQuadBezier({ 400, 300 }, { 350, 500 }, { 300, 300 }, Color4D(0, 0, 0, 0.5f));
            rz.drawQuadBezier({ 300, 300 }, { 350, 500 }, { 400, 300 }, Color4D(0, 0, 0, 0.5f));

            rz.drawQuadBezier({ 500, 300 }, { 450, 100 }, { 400, 300 }, Color4D(0, 0, 0, 0.5f));
            rz.drawQuadBezier({ 400, 300 }, { 450, 100 }, { 500, 300 }, Color4D(0, 0, 0, 0.5f));
        }
    }

    void TEST_CUBIC_BEZIERS(Rasterizer& rz) {
        if (false)
        {
            rz.drawCubicBezier(
                Point2{200, 200},
                Point2{200, 250},
                Point2{250, 300},
                Point2{300, 300},
                Color4D(0, 0, 0, 0.5f));

            rz.drawCubicBezier(
                Point2{200, 200},
                Point2{200, 300},
                Point2{300, 250},
                Point2{400, 500},
                Color4D(0, 0, 0, 0.5f));

            rz.drawCubicBezier(
                Point2{200, 200},
                Point2{500, 500},
                Point2{200, 500},
                Point2{500, 200},
                Color4D(0, 0, 0, 0.5f));

            rz.drawCubicBezier(
                Point2{200, 200},
                Point2{550, 550},
                Point2{150, 550},
                Point2{500, 200},
                Color4D(0, 0, 0, 0.5f));

            rz.drawCubicBezier(
                Point2{200, 200},
                Point2{600, 900},
                Point2{-50, 50},
                Point2{400, 400},
                Color4D(0, 0, 0, 0.5f));
        }

        if (false)
        {
            double t1 = 0.2;
            double t2 = 0.5;

            Point2 p0{200, 200};
            Point2 p1{250, 400};
            Point2 p2{400, 450};
            Point2 p3{700, 350};

            rz.drawLineSeg(p0, p1, Color4D(0, 0, 0, 1));
            rz.drawLineSeg(p1, p2, Color4D(0, 0, 0, 1));
            rz.drawLineSeg(p2, p3, Color4D(0, 0, 0, 1));

            {
                double t21 = (t2 - t1) / (1 - t1);

                auto pt01 = p0 + (p1 - p0)*t1;
                auto p11 = p1 + (p2 - p1)*t1;
                auto p21 = p2 + (p3 - p2)*t1;
                auto pt02 = pt01 + (p11 - pt01)*t1;
                auto pt04 = p11 + (p21 - p11)*t1;
                auto pt03 = pt02 + (pt04 - pt02)*t1;

                rz.drawLineSeg(pt01, p11, Color4D(0, 0, 0, 1));
                rz.drawLineSeg(p11, p21, Color4D(0, 0, 0, 1));
                rz.drawLineSeg(pt02, pt04, Color4D(0, 0, 0, 1));
                rz.drawCircle(pt03, 2, 1, THICK_CENTER, Color4D(0, 0, 0, 1));

                auto pt11 = pt03 + (pt04 - pt03)*t21;
                auto pt14 = pt04 + (p21 - pt04)*t21;
                auto pt16 = p21 + (p3 - p21)*t21;
                auto pt15 = pt14 + (pt16 - pt14)*t21;
                auto pt12 = pt11 + (pt14 - pt11)*t21;
                auto pt13 = pt12 + (pt15 - pt12)*t21;

                rz.drawLineSeg(pt11, pt14, Color4D(0, 0, 1, 1));
                rz.drawLineSeg(pt14, pt16, Color4D(0, 0, 1, 1));
                rz.drawLineSeg(pt12, pt15, Color4D(0, 0, 1, 1));
                rz.drawCircle(pt13, 2, 1, THICK_CENTER, Color4D(0, 0, 1, 1));
            }

            /*{
                auto pt01 = p0 + (p1 - p0)*t2;
                auto p11 = p1 + (p2 - p1)*t2;
                auto p21 = p2 + (p3 - p2)*t2;
                auto pt02 = pt01 + (p11 - pt01)*t2;
                auto pt04 = p11 + (p21 - p11)*t2;
                auto pt03 = pt02 + (pt04 - pt02)*t2;

                rz.drawLineSeg(pt01, p11, Color(0.8f, 0.6f, 0, 1));
                rz.drawLineSeg(p11, p21, Color(0.8f, 0.6f, 0, 1));
                rz.drawLineSeg(pt02, pt04, Color(0.8f, 0.6f, 0, 1));
                rz.drawCircle(pt03, 2, Color(0.8f, 0.6f, 0, 1));
            }*/
        }
    }

}
}