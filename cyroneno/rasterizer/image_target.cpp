// Copyright (c) 2019 ucclkp <ucclkp@gmail.com>.
// This file is part of cyroneno project.
//
// This program is licensed under GPLv3 license that can be
// found in the LICENSE file.

#include "image_target.h"


namespace cyro {

    ImageTarget::ImageTarget(Image* image)
        : image_(image) {}

    void ImageTarget::drawColor(int x, int y, const Color4D& c) {
        switch (mode_) {
        case BM_ADD:
            image_->drawColor(x, y, c, blend_c4d_add);
            break;

        default:
            image_->drawColor(x, y, c, blend_c4d);
            break;
        }
    }

}
