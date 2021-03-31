// Copyright (c) 2019 ucclkp <ucclkp@gmail.com>.
// This file is part of cyroneno project.
//
// This program is licensed under GPLv3 license that can be
// found in the LICENSE file.

#include "cyroneno-test/test/line_rz_unit_test.h"

#include "cyroneno/rasterizer/common.h"
#include "cyroneno/rasterizer/rasterizer.h"


namespace cyro {
namespace test {

    void TEST_LINE(Rasterizer& rz) {
        rz.setAntiAlias(false);
        rz.drawLine({ 1, 1 }, { 1, 1 }, 1, THICK_CENTER, Color4D(0, 0, 0, 1));

        // 竖线
        rz.drawLine({ 50, 50 }, { 50, 100 }, 1, THICK_CENTER, Color4D(0, 0, 0, 1));
        rz.translate(3, 0);
        rz.drawLine({ 50, 100 }, { 50, 50 }, 1, THICK_CENTER, Color4D(0, 0, 0, 1));

        // 横线
        rz.drawLine({ 58, 50 }, { 108, 50 }, 1, THICK_CENTER, Color4D(0, 0, 0, 1));
        rz.drawLine({ 108, 53 }, { 58, 53 }, 1, THICK_CENTER, Color4D(0, 0, 0, 1));

        // 1L, 3L
        rz.drawLine({ 103, 100 }, { 203, 150 }, 1, THICK_CENTER, Color4D(0, 0, 0, 1));
        rz.drawLine({ 213, 150 }, { 113, 100 }, 1, THICK_CENTER, Color4D(0, 0, 0, 1));

        // 1H, 3H
        rz.drawLine({ 100, 100 }, { 150, 300 }, 1, THICK_START, Color4D(0, 0, 0, 1));
        rz.drawLine({ 160, 300 }, { 110, 100 }, 1, THICK_CENTER, Color4D(0, 0, 0, 1));

        // 2L, 4L
        rz.drawLine({ 220, 150 }, { 320, 100 }, 1, THICK_CENTER, Color4D(0, 0, 0, 1));
        rz.drawLine({ 330, 100 }, { 230, 150 }, 1, THICK_CENTER, Color4D(0, 0, 0, 1));

        // 2H, 4H
        rz.drawLine({ 100, 300 }, { 150, 100 }, 1, THICK_START, Color4D(0, 0, 0, 1));
        rz.drawLine({ 160, 100 }, { 110, 300 }, 1, THICK_CENTER, Color4D(0, 0, 0, 1));
    }

    void TEST_LINE_AA(Rasterizer& rz) {
        rz.setAntiAlias(true);
        rz.drawLine({ 1, 1 }, { 1, 1 }, 1, THICK_CENTER, Color4D(0, 0, 0, 1));

        // 1L, 3L
        rz.drawLine({ 103, 100 }, { 203, 150 }, 1, THICK_CENTER, Color4D(0, 0, 0, 1));
        rz.drawLine({ 213, 150 }, { 113, 100 }, 1, THICK_CENTER, Color4D(0, 0, 0, 1));

        // 1H, 3H
        rz.drawLine({ 100, 100 }, { 150, 300 }, 1, THICK_CENTER, Color4D(0, 0, 0, 1));
        rz.drawLine({ 160, 300 }, { 110, 100 }, 1, THICK_CENTER, Color4D(0, 0, 0, 1));

        // 2L, 4L
        rz.drawLine({ 220, 150 }, { 320, 100 }, 1, THICK_CENTER, Color4D(0, 0, 0, 1));
        rz.drawLine({ 330, 100 }, { 230, 150 }, 1, THICK_CENTER, Color4D(0, 0, 0, 1));

        // 2H, 4H
        rz.drawLine({ 100, 300 }, { 150, 100 }, 1, THICK_CENTER, Color4D(0, 0, 0, 1));
        rz.drawLine({ 160, 100 }, { 110, 300 }, 1, THICK_CENTER, Color4D(0, 0, 0, 1));
    }

    void TEST_LINE_AA_FLOAT(Rasterizer& rz) {
        rz.setAntiAlias(true);
        rz.drawLine({ 5, 5 }, { 5.1, 5.1 }, 1, THICK_CENTER, Color4D(0, 0, 0, 1));
        //rz.drawLine({ 3, 3 }, { 4, 4 }, 1, THICK_CENTER, Color(0, 0, 0, 1));

        if (true) {
            rz.save();

            // 横线，x浮点
            rz.drawLine({ 100.5, 1.5 }, { 200.5, 1.5 }, 1, THICK_CENTER, Color4D(0, 0, 0, 1));
            rz.translate(0, 3);
            rz.drawLine({ 200, 1.5 }, { 100, 1.5 }, 2, THICK_CENTER, Color4D(0, 0, 0, 1));
            rz.translate(0, 5);
            rz.drawLine({ 200.5, 100 }, { 100.5, 100 }, 1, THICK_END, Color4D(0, 0, 0, 1));

            // 横线，y小值
            rz.translate(0, 7);
            rz.drawLine({ 100, 100 }, { 200, 100.5 }, 1, THICK_CENTER, Color4D(0, 0, 0, 1));
            rz.translate(0, 3);
            rz.drawLine({ 200, 100.5 }, { 100, 100 }, 1, THICK_CENTER, Color4D(0, 0, 0, 1));

            // 竖线，x小值
            rz.translate(0, 10);
            rz.drawLine({ 100, 100 }, { 100.5, 200 }, 1, THICK_CENTER, Color4D(0, 0, 0, 1));
            rz.translate(3, 0);
            rz.drawLine({ 100.5, 200 }, { 100, 100 }, 1, THICK_CENTER, Color4D(0, 0, 0, 1));

            rz.restore();
        }

        if (true) {
            rz.save();
            rz.translate(20, 20);

            // 1H, 3H
            rz.drawLine({ 100.719, 100.874 }, { 150.325, 300.147 }, 1, THICK_CENTER, Color4D(0, 0, 0, 1));
            rz.translate(5, 0);
            rz.drawLine({ 150.325, 300.147 }, { 100.719, 100.874 }, 1, THICK_CENTER, Color4D(0, 0, 0, 1));

            // 2H, 4H
            rz.translate(-10, 0);
            rz.drawLine({ 100.5, 300.147 }, { 150.325, 100.874 }, 1, THICK_CENTER, Color4D(0, 0, 0, 1));
            rz.translate(10, 0);
            rz.drawLine({ 150.325, 100.874 }, { 100.5, 300.147 }, 1, THICK_CENTER, Color4D(0, 0, 0, 1));

            //rz.translate(50, 0);

            // 1L, 3L
            rz.drawLine({ 103.3, 100.7 }, { 203.321, 150.587 }, 20, THICK_END, Color4D(0, 0, 0, 1));
            rz.translate(0, 10);
            rz.drawLine({ 203.321, 150.587 }, { 103.3, 100.7 }, 1, THICK_CENTER, Color4D(0, 0, 0, 1));

            // 2L, 4L
            rz.drawLine({ 220, 150 }, { 320, 100 }, 1, THICK_END, Color4D(0, 0, 0, 1));
            rz.translate(10, 0);
            rz.drawLine({ 320, 100 }, { 220, 150 }, 1, THICK_CENTER, Color4D(0, 0, 0, 1));

            rz.restore();
        }
    }

    void TEST_LINE_SEG(Rasterizer& rz) {
        // 1L, 3L
        rz.drawLineSeg({ 100, 100 }, { 200, 150 }, Color4D(0, 0, 0, 0.5));
        rz.drawLineSeg({ 206, 150 }, { 106, 100 }, Color4D(0, 0, 0, 0.5));

        // 1H, 3H
        rz.drawLineSeg({ 100, 100 }, { 150, 300 }, Color4D(0, 0, 0, 1));
        rz.drawLineSeg({ 156, 300 }, { 106, 100 }, Color4D(0, 0, 0, 1));

        // 2L, 4L
        rz.drawLineSeg({ 200, 150 }, { 300, 100 }, Color4D(0, 0, 0, 1));
        rz.drawLineSeg({ 306, 100 }, { 206, 150 }, Color4D(0, 0, 0, 1));

        // 2H, 4H
        rz.drawLineSeg({ 100, 300 }, { 150, 100 }, Color4D(0, 0, 0, 1));
        rz.drawLineSeg({ 156, 100 }, { 106, 300 }, Color4D(0, 0, 0, 1));
    }

}
}