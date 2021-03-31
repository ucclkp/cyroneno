// Copyright (c) 2019 ucclkp <ucclkp@gmail.com>.
// This file is part of cyroneno project.
//
// This program is licensed under GPLv3 license that can be
// found in the LICENSE file.

#include "box.h"


namespace cyro {

    Box::Box(const Point3& min, const Point3& max)
        : min_(min), max_(max) {
    }

}