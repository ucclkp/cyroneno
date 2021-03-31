// Copyright (c) 2019 ucclkp <ucclkp@gmail.com>.
// This file is part of cyroneno project.
//
// This program is licensed under GPLv3 license that can be
// found in the LICENSE file.

#ifndef CYRONENO_RASTERIZER_IMAGE_TARGET_H_
#define CYRONENO_RASTERIZER_IMAGE_TARGET_H_

#include "target.h"
#include "../image.hpp"


namespace cyro {

    class ImageTarget : public Target {
    public:
        explicit ImageTarget(Image* image);

        void drawColor(int x, int y, const Color4D& c) override;

    private:
        Image* image_;
    };

}

#endif  // CYRONENO_RASTERIZER_IMAGE_TARGET_H_