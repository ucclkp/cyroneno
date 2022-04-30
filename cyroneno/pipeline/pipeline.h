// Copyright (c) 2019 ucclkp <ucclkp@gmail.com>.
// This file is part of cyroneno project.
//
// This program is licensed under GPLv3 license that can be
// found in the LICENSE file.

#ifndef CYRONENO_PIPELINE_PIPELINE_H_
#define CYRONENO_PIPELINE_PIPELINE_H_

#include <array>
#include <functional>
#include <vector>

#include "../image.hpp"
#include "../point.hpp"
#include "../equations.h"
#include "../rasterizer/image_rasterizer.h"
#include "../rasterizer/target.h"


namespace cyro {

    struct Vertex {
        Point3 pos;
    };

    struct Triangle {
        int idx[3];
        bool clipped;
    };

    struct Polygon {
        // 原始顶点数据
        std::vector<Vertex> vertices;
        // 经过坐标变换和裁剪的顶点数据
        std::vector<Point4> t_vertices_4d;
        // 索引数据（可能为空）
        std::vector<std::array<int, 3>> indices;

        // 被裁剪的三角形的索引
        std::vector<bool> cts_;

        // 在世界坐标中裁剪生成的三角形
        std::vector<Point3> cgtv_3d;
        // 在屏幕坐标中裁剪生成的三角形
        std::vector<Point4> cgtv_4d;
        std::vector<std::array<int, 3>> cgti;
    };

    class Pipeline {
    public:
        explicit Pipeline(Target* target);

        void launch(int vp_width, int vp_height);

        void constructCube();
        void constructSphere();

    private:
        enum Flags {
            CLIP_BEFORE = 1 << 0,
            CLIP_AFTER = 1 << 1,
        };

        using TriangleHandler = std::function<void()>;

        void processVertices(int vp_width, int vp_height);
        void rasterize();

        void clipTrianglesBefore(
            double left, double right, double top, double bottom, double near, double far,
            const Point3& eye, const Vector3& look, const Vector3& up);
        void clipTrianglesAfter(
            double left, double right, double top, double bottom, double near, double far,
            bool is_persp);

        void clipEdge(const PlaneEqu& cp, Point3& a, Point3& b, int* which_clipped);
        bool clipTriangle(
            const PlaneEqu& cp, const Point3 p[3], const int indices[3], Polygon* polygon);

        void clipEdge(const PlaneEqu4D& cp, Point4& a, Point4& b, int* which_clipped);
        bool clipTriangle(
            const PlaneEqu4D& cp, const Point4 p[3], const int indices[3], Polygon* polygon);

        ImageRasterizer rasterizer_;
        std::vector<Polygon> polygons_;

        bool is_persp_;
        uint32_t flags_;
    };

}

#endif  // CYRONENO_PIPELINE_PIPELINE_H_