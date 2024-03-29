// Copyright (c) 2019 ucclkp <ucclkp@gmail.com>.
// This file is part of cyroneno project.
//
// This program is licensed under GPLv3 license that can be
// found in the LICENSE file.

#ifndef CYRONENO_VECTOR_HPP_
#define CYRONENO_VECTOR_HPP_

#include "utils/math/algebra/matrix.hpp"


namespace cyro {

    using Vector2 = utl::math::VectorT<double, 2>;
    using Vector3 = utl::math::VectorT<double, 3>;
    using Vector4 = utl::math::VectorT<double, 4>;

}

#endif  // CYRONENO_VECTOR_HPP_