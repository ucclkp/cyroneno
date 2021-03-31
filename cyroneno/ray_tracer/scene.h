// Copyright (c) 2019 ucclkp <ucclkp@gmail.com>.
// This file is part of cyroneno project.
//
// This program is licensed under GPLv3 license that can be
// found in the LICENSE file.

#ifndef CYRONENO_RAY_TRACER_SCENE_H_
#define CYRONENO_RAY_TRACER_SCENE_H_

#include <vector>

#include "ray_tracer_structs.h"


namespace cyro {

    class Surface;

    class Scene {
    public:
        Scene() = default;
        ~Scene();

        bool hit(const Ray& ray, double t0, double t1, HitRecord& rec) const;

        void addSurface(Surface* s);
        void removeSurface(size_t index);
        const Surface* getSurface(size_t index) const;

    private:
        std::vector<Surface*> surfaces_;
    };

}

#endif  // CYRONENO_RAY_TRACER_SCENE_H_