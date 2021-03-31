// Copyright (c) 2019 ucclkp <ucclkp@gmail.com>.
// This file is part of cyroneno project.
//
// This program is licensed under GPLv3 license that can be
// found in the LICENSE file.

#include "scene.h"

#include "surface.h"


namespace cyro {

    Scene::~Scene() {
        for (auto s : surfaces_) {
            delete s;
        }
    }

    bool Scene::hit(const Ray& ray, double t0, double t1, HitRecord& rec) const {
        int index = 0;
        HitRecord hit{};
        const Surface* hit_surface = nullptr;

        for (int i = 0; i < static_cast<int>(surfaces_.size()); ++i) {
            HitRecord cur_hit;
            if (surfaces_[i]->hit(ray, t0, t1, cur_hit)) {
                if (hit_surface && cur_hit.t < hit.t) {
                    hit = cur_hit;
                    hit_surface = surfaces_[i];
                    index = i;
                } else if (!hit_surface) {
                    hit = cur_hit;
                    hit_surface = surfaces_[i];
                    index = i;
                }
            }
        }

        if (hit_surface) {
            rec.index = index;
            rec.shading_method = hit_surface->getShadingMethod();
            rec.t = hit.t;
            rec.p = hit.p;
            rec.n = hit.n;

            return true;
        }

        return false;
    }

    void Scene::addSurface(Surface* s) {
        surfaces_.push_back(s);
    }

    void Scene::removeSurface(size_t index) {
        auto it = surfaces_.begin() + index;
        delete *it;
        surfaces_.erase(it);
    }

    const Surface* Scene::getSurface(size_t index) const {
        return surfaces_[index];
    }

}