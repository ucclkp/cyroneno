// Copyright (c) 2019 ucclkp <ucclkp@gmail.com>.
// This file is part of cyroneno project.
//
// This program is licensed under GPLv3 license that can be
// found in the LICENSE file.

#include "ray_tracer.h"

#include <algorithm>

#include "utils/message/message.h"
#include "utils/message/message_pump.h"

#include "surface.h"

#define MSG_DATA      22
#define MSG_COMPLETE  33


namespace cyro {

    RayTracer::RayTracer()
        : cycler_(utl::MessagePump::getCurrent()),
          need_stop_(false)
    {
        cycler_.setListener(this);

        initLights();
        initSurfaces();
    }

    RayTracer::~RayTracer() {
    }

    void RayTracer::rayTracer(
        ProjectionType type, int img_width, int img_height, Image* image)
    {
        double l = -img_width;
        double r = img_width;
        double t = img_height;
        double b = -img_height;

        Point3 eye(0, 100, 0);
        Vector3 view_dir(0, -0.3, -1);
        Vector3 up_vector(0, 1, 0);

        Vector3 cb_w = -view_dir.normalize();
        Vector3 cb_u = (up_vector ^ cb_w).normalize();
        Vector3 cb_v = cb_w ^ cb_u;

        for (int j = 0; j < img_height; ++j) {
            for (int i = 0; i < img_width; ++i) {
                double u = l + (r - l)*(i + 0.5) / img_width;
                double v = b + (t - b)*(j + 0.5) / img_height;

                Ray ray;
                if (type == ORTHO) {
                    ray.direction = -cb_w;
                    ray.origin = eye + cb_u * u + cb_v * v;
                } else if (type == PERSP) {
                    float d = 500;
                    ray.direction = -cb_w * d + cb_u * u + cb_v * v;
                    ray.origin = eye;
                }

                auto color = rayColor(ray, 0, std::numeric_limits<double>::max());
                image->setColor(i, j, Color4D(color, 1));
            }
        }
    }

    void RayTracer::rayTracerAsync(
        ProjectionType type, int img_width, int img_height, RayTracerListener* listener)
    {
        listener_ = listener;
        worker_ = std::thread(&RayTracer::run, this, type, img_width, img_height);
    }

    void RayTracer::stop() {
        need_stop_ = true;
        if (worker_.joinable()) {
            worker_.join();
        }
        cycler_.setListener(nullptr);
    }

    void RayTracer::onHandleMessage(const utl::Message& msg) {
        switch (msg.id) {
        case MSG_DATA:
            if (listener_) {
                auto info = static_cast<std::vector<RenderInfo>*>(msg.shared_data.get());
                listener_->onPixelData(*info);
            }
            break;

        case MSG_COMPLETE:
            if (listener_) {
                listener_->onRenderComplete();
            }
            break;
        default:
            break;
        }
    }

    void RayTracer::initLights() {
        Light light1;
        light1.pos = Point3(-300, 300, -800);
        light1.intensity = Color3D(0.4, 0.4, 0.4);
        lights_.push_back(light1);

        Light light2;
        light2.pos = Point3(100, 100, 0);
        light2.intensity = Color3D(0.2, 0.2, 0.2);
        lights_.push_back(light2);
    }

    void RayTracer::initSurfaces() {
        auto sphere1 = new Sphere(Point3(-100, 82, -600), 80);
        sphere1->setKd(Color3D(0.4, 0.4, 0.4));
        sphere1->setKs(Color3D(0.8, 0.8, 0.8));
        sphere1->setPhongExp(80);
        sphere1->setShadingMethod(Surface::DIFFUSE | Surface::SPECULAR);
        scene_.addSurface(sphere1);

        auto sphere2 = new Sphere(Point3(100, 82, -600), 80);
        sphere2->setKd(Color3D(0.86, 0.86, 0.86));
        sphere2->setKs(Color3D(0.8, 0.8, 0.8));
        sphere2->setPhongExp(80);
        sphere2->setShadingMethod(Surface::DIFFUSE | Surface::SPECULAR);
        scene_.addSurface(sphere2);

        double edge_length = 1000;
        auto plane1 = new Plane(
            { Point3(edge_length, 0, 0),
              Point3(-edge_length, 0, 0),
              Point3(-edge_length, 0, -edge_length - 1000),
              Point3(edge_length, 0, -edge_length - 1000) },
            Vector3(0, 1, 0));
        plane1->setKd(Color3D(0.55, 0.55, 0.55));
        plane1->setShadingMethod(Surface::DIFFUSE | Surface::SPECULAR);
        scene_.addSurface(plane1);
    }

    Color3D RayTracer::rayColor(const Ray& ray, double t0, double t1) {
        double ambient_intensity = 0.5;
        Color3D ambient_color(0.25, 0.25, 0.25);
        Color3D background_color(0.125, 0.125, 0.125);

        HitRecord hit;
        if (scene_.hit(ray, t0, t1, hit)) {
            auto color = ambient_color * ambient_intensity;

            if (hit.shading_method & Surface::SPECULAR) {
                // 镜面反射（可能反射多次）
                auto vv = (ray.origin - hit.p).normalize();
                auto re = (-vv + hit.n*(2 * (vv*hit.n))).normalize();

                Ray re_ray;
                re_ray.origin = hit.p;
                re_ray.direction = re;

                color += rayColor(re_ray, 0.001, std::numeric_limits<double>::max()) * 0.5f;
            }

            for (const auto& light : lights_) {
                auto lv = (light.pos - hit.p).normalize();
                auto vv = (ray.origin - hit.p).normalize();
                auto hv = (vv + lv).normalize();

                Ray shadow_ray;
                shadow_ray.direction = lv;
                shadow_ray.origin = hit.p;

                HitRecord shadow_hit;
                if (!scene_.hit(shadow_ray, 0.001, std::numeric_limits<double>::max(), shadow_hit)) {
                    auto cur_surface = scene_.getSurface(hit.index);
                    if (cur_surface->getShadingMethod() & Surface::DIFFUSE) {
                        color += cur_surface->getKd() * light.intensity * std::max(0.0, hit.n*lv);
                    }
                    if (cur_surface->getShadingMethod() & Surface::SPECULAR) {
                        color += cur_surface->getKs() * light.intensity
                            * std::pow(std::max(0., hit.n*hv), cur_surface->getPhongExp());
                    }
                }
            }

            return color;
        }

        return background_color;
    }

    void RayTracer::run(ProjectionType type, int img_width, int img_height) {
        double l = -img_width;
        double r = img_width;
        double t = img_height;
        double b = -img_height;

        Point3 eye(0, 100, 0);
        Vector3 view_dir(0, -0.3, -1);
        Vector3 up_vector(0, 1, 0);

        Vector3 cb_w = -view_dir.normalize();
        Vector3 cb_u = (up_vector ^ cb_w).normalize();
        Vector3 cb_v = cb_w ^ cb_u;

        for (int j = 0; j < img_height; ++j) {
            auto ifs = std::make_shared<std::vector<RenderInfo>>();
            for (int i = 0; i < img_width; ++i) {
                if (need_stop_) {
                    return;
                }

                double u = l + (r - l)*(i + 0.5) / img_width;
                double v = b + (t - b)*(j + 0.5) / img_height;

                Ray ray;
                if (type == ORTHO) {
                    ray.direction = -cb_w;
                    ray.origin = eye + cb_u * u + cb_v * v;
                } else if (type == PERSP) {
                    float d = 500;
                    ray.direction = -cb_w * d + cb_u * u + cb_v * v;
                    ray.origin = eye;
                }

                auto color = rayColor(ray, 0, std::numeric_limits<double>::max());

                RenderInfo info;
                info.x = i;
                info.y = j;
                info.color = color;
                ifs->push_back(info);
            }

            utl::Message msg;
            msg.id = MSG_DATA;
            msg.shared_data = ifs;
            cycler_.post(&msg);
        }

        cycler_.post(MSG_COMPLETE);
    }

}