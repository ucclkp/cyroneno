// Copyright (c) 2019 ucclkp <ucclkp@gmail.com>.
// This file is part of cyroneno project.
//
// This program is licensed under GPLv3 license that can be
// found in the LICENSE file.

#ifndef CYRONENO_RAY_TRACER_RAY_TRACER_STRUCTS_H_
#define CYRONENO_RAY_TRACER_RAY_TRACER_STRUCTS_H_

#include "../color.hpp"
#include "../point.hpp"
#include "../vector.hpp"


namespace cyro {

    enum ProjectionType {
        ORTHO,
        PERSP,
    };

    struct Ray {
        Point3 origin;
        Vector3 direction;
    };

    struct HitRecord {
        int index;
        int shading_method;

        double t;
        Point3 p;
        Vector3 n;
    };

    struct Light {
        Point3 pos;
        Color3D intensity;
    };

}

#endif  // CYRONENO_RAY_TRACER_RAY_TRACER_STRUCTS_H_