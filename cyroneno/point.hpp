// Copyright (c) 2019 ucclkp <ucclkp@gmail.com>.
// This file is part of cyroneno project.
//
// This program is licensed under GPLv3 license that can be
// found in the LICENSE file.

#ifndef CYRONENO_POINT_HPP_
#define CYRONENO_POINT_HPP_

#include "utils/math/algebra/point.hpp"


namespace cyro {

    using Point2 = utl::math::PointT<double, 2>;
    using Point3 = utl::math::PointT<double, 3>;
    using Point4 = utl::math::PointT<double, 4>;

    using Point2I = utl::math::PointT<int, 2>;

}

#endif  // CYRONENO_POINT_HPP_