// Copyright (c) 2019 ucclkp <ucclkp@gmail.com>.
// This file is part of cyroneno project.
//
// This program is licensed under GPLv3 license that can be
// found in the LICENSE file.

#ifndef CYRONENO_RAY_TRACER_SURFACE_H_
#define CYRONENO_RAY_TRACER_SURFACE_H_

#include <vector>

#include "../box.h"
#include "../color.hpp"
#include "ray_tracer_structs.h"


namespace cyro {

    class Surface {
    public:
        enum ShadingMethod {
            NONE = 0x0,
            DIFFUSE = 0x1,
            SPECULAR = 0x2,
        };

        Surface()
            : phong_exp_(0),
              shading_method_(DIFFUSE) {}

        virtual ~Surface() = default;

        virtual bool hit(const Ray& ray, double t0, double t1, HitRecord& rec) const = 0;
        virtual Box boundingBox() const = 0;

        void setKd(const Color3D& kd) { kd_ = kd; }
        void setKs(const Color3D& ks) { ks_ = ks; }
        void setPhongExp(double p) { phong_exp_ = p; }
        void setShadingMethod(int sm) { shading_method_ = sm; }

        Color3D getKd() const { return kd_; }
        Color3D getKs() const { return ks_; }
        double getPhongExp() const { return phong_exp_; }
        int getShadingMethod() const { return shading_method_; }

    private:
        Color3D kd_;
        Color3D ks_;
        double phong_exp_;
        int shading_method_;
    };


    class Sphere : public Surface {
    public:
        Sphere(const Point3& c, double r);

        bool hit(const Ray& ray, double t0, double t1, HitRecord& rec) const override;
        Box boundingBox() const override;

    private:
        Point3 center_;
        double radius_;
    };


    class Plane : public Surface {
    public:
        Plane(const std::vector<Point3>& p, const Vector3& n);

        bool hit(const Ray& ray, double t0, double t1, HitRecord& rec) const override;
        Box boundingBox() const override;

    private:
        Vector3 n_;
        std::vector<Point3> points_;
    };

}

#endif  // CYRONENO_RAY_TRACER_SURFACE_H_