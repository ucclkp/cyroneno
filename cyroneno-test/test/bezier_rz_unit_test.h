// Copyright (c) 2019 ucclkp <ucclkp@gmail.com>.
// This file is part of cyroneno project.
//
// This program is licensed under GPLv3 license that can be
// found in the LICENSE file.

#ifndef CYRONENO_TEST_TEST_BEZIER_RZ_UNIT_TEST_H_
#define CYRONENO_TEST_TEST_BEZIER_RZ_UNIT_TEST_H_


namespace cyro {
    class Rasterizer;

namespace test {

    void TEST_QUAD_BEZIERS(Rasterizer& rz);
    void TEST_CUBIC_BEZIERS(Rasterizer& rz);

}
}

#endif  // CYRONENO_TEST_TEST_BEZIER_RZ_UNIT_TEST_H_