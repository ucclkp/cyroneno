// Copyright (c) 2019 ucclkp <ucclkp@gmail.com>.
// This file is part of cyroneno project.
//
// This program is licensed under GPLv3 license that can be
// found in the LICENSE file.

#ifndef CYRONENO_RASTERIZER_COMMON_H_
#define CYRONENO_RASTERIZER_COMMON_H_


namespace cyro {

    enum ThickType {
        THICK_START,
        THICK_CENTER,
        THICK_END,
    };

    /**
     * 若 val > 0，返回 1；
     * 若 val = 0，返回 0；
     * 若 val < 0，返回 -1
     */
    inline int sgn(int val) {
        return (val > 0) - (val < 0);
    }

    inline int sgn(double val) {
        return (val > 0) - (val < 0);
    }

    /**
     * 若 val >= 0，返回 1；
     * 若 val < 0，返回 -1
     */
    inline int sgn2(int val) {
        return (val >= 0) - (val < 0);
    }

    inline int sgn2(double val) {
        return (val >= 0) - (val < 0);
    }

    /**
     * 若 val = 0，返回 -1；
     * 若 val = 1，返回 1
     */
    inline int divide(int val) {
        return 2 * val - 1;
    }

}

#endif  // CYRONENO_RASTERIZER_COMMON_H_