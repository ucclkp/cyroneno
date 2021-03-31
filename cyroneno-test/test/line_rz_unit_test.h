// Copyright (c) 2019 ucclkp <ucclkp@gmail.com>.
// This file is part of cyroneno project.
//
// This program is licensed under GPLv3 license that can be
// found in the LICENSE file.

#ifndef CYRONENO_TEST_TEST_LINE_RZ_UNIT_TEST_H_
#define CYRONENO_TEST_TEST_LINE_RZ_UNIT_TEST_H_


namespace cyro {
    class Rasterizer;

namespace test {

    void TEST_LINE(Rasterizer& rz);
    void TEST_LINE_AA(Rasterizer& rz);
    void TEST_LINE_AA_FLOAT(Rasterizer& rz);
    void TEST_LINE_SEG(Rasterizer& rz);

}
}

#endif  // CYRONENO_TEST_TEST_LINE_RZ_UNIT_TEST_H_