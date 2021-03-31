// Copyright (c) 2019 ucclkp <ucclkp@gmail.com>.
// This file is part of cyroneno project.
//
// This program is licensed under GPLv3 license that can be
// found in the LICENSE file.

#ifndef CYRONENO_BOX_H_
#define CYRONENO_BOX_H_

#include "point.hpp"


namespace cyro {

    class Box {
    public:
        Box(const Point3& min, const Point3& max);

        Point3 min_, max_;
    };

}

#endif  // CYRONENO_BOX_H_