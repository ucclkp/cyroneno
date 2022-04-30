// Copyright (c) 2019 ucclkp <ucclkp@gmail.com>.
// This file is part of cyroneno project.
//
// This program is licensed under GPLv3 license that can be
// found in the LICENSE file.

#ifndef CYRONENO_RASTERIZER_IMAGE_RASTERIZER_H_
#define CYRONENO_RASTERIZER_IMAGE_RASTERIZER_H_

#include <stack>

#include "rasterizer.h"
#include "image_target.h"
#include "../matrix.hpp"


namespace cyro {

    /**
     * TODO:
     * 1. 顶点坐标需要支持浮点数
     * 2. 绘制三角形公共边时需要一些额外处理以防出现空洞
     **/
    class ImageRasterizer : public Rasterizer {
    public:
        explicit ImageRasterizer(Target* target);

        void setAntiAlias(bool enabled) override;

        void save() override;
        void restore() override;
        void translate(double dx, double dy) override;

        void drawPoint(const Point2& p, const Color4D& c) override;

        void drawLine(
            const Point2& p0, const Point2& p1,
            double thick, int thick_type, const Color4D& c) override;
        void drawLineSeg(
            const Point2& p0, const Point2& p1, const Color4D& c) override;

        void drawCircle(
            const Point2& center, double r,
            double thick, int thick_type, const Color4D& c) override;
        void drawEllipse(const Point2& center, double a, double b, const Color4D& c) override;

        void drawQuadBezier(
            const Point2& p1, const Point2& p2, const Point2& p3, const Color4D& c) override;
        void drawQuadBezier(
            const Point2& p1, const Point2& p2, const Point2& p3,
            double w0, double w1, double w2,
            const Color4D& c) override;

        void drawCubicBezier(
            const Point2& p1, const Point2& p2, const Point2& p3, const Point2& p4, const Color4D& c) override;

        void drawTriangle(
            const Point2& p0, const Point2& p1, const Point2& p2,
            const Color4D& c0, const Color4D& c1, const Color4D& c2) override;

    private:
        Target* target_;
        Matrix3x3 matrix_;
        std::stack<Matrix3x3> stack_;
        bool is_antialiased_ = false;
    };

}

#endif  // CYRONENO_RASTERIZER_IMAGE_RASTERIZER_H_