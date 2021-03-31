// Copyright (c) 2019 ucclkp <ucclkp@gmail.com>.
// This file is part of cyroneno project.
//
// This program is licensed under GPLv3 license that can be
// found in the LICENSE file.

#include "cyroneno-test/test/ellipse_rz_unit_test.h"

#include "cyroneno/rasterizer/common.h"
#include "cyroneno/rasterizer/rasterizer.h"


namespace cyro {
namespace test {

    void TEST_ELLIPSES(Rasterizer& rz) {
        //rz.drawCircle({ 400, 400 }, 4, Color4D(0, 0, 0, 0.5));
        //rz.drawEllipse({ 400, 400 }, 4, 3, Color4D(0, 0, 0, 0.5));

        //rz.drawCircle({ 400, 400 }, 5, Color4D(0, 0, 0, 0.5));
        rz.drawCircle({ 400, 400 }, 4, 9, THICK_START, Color4D(0, 0, 0, 0.5));
    }

}
}
