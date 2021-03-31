// Copyright (c) 2019 ucclkp <ucclkp@gmail.com>.
// This file is part of cyroneno project.
//
// This program is licensed under GPLv3 license that can be
// found in the LICENSE file.

#ifndef CYRONENO_RAY_TRACER_RAY_TRACER_H_
#define CYRONENO_RAY_TRACER_RAY_TRACER_H_

#include <atomic>
#include <thread>

#include "utils/message/cycler.h"

#include "../color.hpp"
#include "../image.hpp"
#include "ray_tracer_structs.h"
#include "scene.h"


namespace cyro {

    struct RenderInfo {
        int x, y;
        Color3D color;
    };

    class RayTracerListener {
    public:
        virtual ~RayTracerListener() = default;

        virtual void onPixelData(const std::vector<RenderInfo>& row) = 0;
        virtual void onRenderComplete() = 0;
    };

    class RayTracer : public utl::CyclerListener {
    public:
        RayTracer();
        ~RayTracer();

        void rayTracer(
            ProjectionType type, int img_width, int img_height, Image* image);
        void rayTracerAsync(
            ProjectionType type, int img_width, int img_height, RayTracerListener* listener);

        void stop();

    protected:
        // ukive::CyclerListener
        void onHandleMessage(const utl::Message& msg) override;

    private:
        void initLights();
        void initSurfaces();
        Color3D rayColor(const Ray& ray, double t0, double t1);

        void run(ProjectionType type, int img_width, int img_height);

        Scene scene_;
        std::vector<Light> lights_;

        std::thread worker_;
        utl::Cycler cycler_;
        std::atomic_bool need_stop_;
        RayTracerListener* listener_ = nullptr;
    };

}

#endif  // CYRONENO_RAY_TRACER_RAY_TRACER_H_