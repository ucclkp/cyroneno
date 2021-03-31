// Copyright (c) 2019 ucclkp <ucclkp@gmail.com>.
// This file is part of cyroneno project.
//
// This program is licensed under GPLv3 license that can be
// found in the LICENSE file.

#ifndef CYRONENO_RASTERIZER_TARGET_H_
#define CYRONENO_RASTERIZER_TARGET_H_

#include "../color.hpp"


namespace cyro {

    class Target {
    public:
        enum BlendMode {
            BM_NORMAL,
            BM_ADD,
        };

        virtual ~Target() = default;

        void setBlendMode(BlendMode mode) { mode_ = mode; }

        virtual void drawColor(int x, int y, const Color4D& c) = 0;

    protected:
        BlendMode mode_ = BM_NORMAL;
    };

}

#endif  // CYRONENO_RASTERIZER_TARGET_H_