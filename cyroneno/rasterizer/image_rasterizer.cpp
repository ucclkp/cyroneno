// Copyright (c) 2019 ucclkp <ucclkp@gmail.com>.
// This file is part of cyroneno project.
//
// This program is licensed under GPLv3 license that can be
// found in the LICENSE file.

#include "image_rasterizer.h"

#include "../equations.h"
#include "line_rz.h"
#include "circle_rz.h"
#include "point_rz.h"
#include "bezier_quad_rz.h"
#include "bezier_cubic_rz.h"
#include "triangle_rz.h"


namespace cyro {

    ImageRasterizer::ImageRasterizer(Target* target)
        : target_(target)
    {
        matrix_.identity();
    }

    void ImageRasterizer::setAntiAlias(bool enabled) {
        is_antialiased_ = enabled;
    }

    void ImageRasterizer::drawPoint(const Point2& p, const Color4D& c) {
        auto tp = matrix_ * p;
        if (is_antialiased_) {
            PointRz::drawAA(tp.x, tp.y, c, target_);
        } else {
            PointRz::draw(int(tp.x), int(tp.y), c, target_);
        }
    }

    void ImageRasterizer::drawLine(
        const Point2& p0, const Point2& p1,
        double thick, int thick_type, const Color4D& c)
    {
        if (is_antialiased_) {
            LineRz::drawAA(matrix_*p0, matrix_*p1, thick, c, target_);
        } else {
            LineRz::draw(matrix_*p0, matrix_*p1, int(thick), thick_type, c, target_);
        }
    }

    void ImageRasterizer::drawLineSeg(const Point2& p0, const Point2& p1, const Color4D& c) {
        LineRz::drawSeg(matrix_*p0, matrix_*p1, c, target_);
    }

    void ImageRasterizer::drawCircle(
        const Point2& center, double r,
        double thick, int thick_type, const Color4D& c)
    {
        //CircleRz::draw(center, int(r), c, &image_);
        CircleRz::draw(center, int(r), int(thick), thick_type, c, target_);
    }

    void ImageRasterizer::drawEllipse(const Point2& center, double a, double b, const Color4D& c) {
        CircleRz::drawEllipse(center, int(a), int(b), c, target_);
    }

    void ImageRasterizer::drawQuadBezier(
        const Point2& p1, const Point2& p2, const Point2& p3, const Color4D& c)
    {
        BezierQuadRz::draw(p1, p2, p3, c, target_);
    }

    void ImageRasterizer::drawQuadBezier(
        const Point2& p1, const Point2& p2, const Point2& p3,
        double w0, double w1, double w2,
        const Color4D& c)
    {
        BezierQuadRz::draw(p1, p2, p3, w0, w1, w2, c, target_);
    }

    void ImageRasterizer::drawCubicBezier(
        const Point2& p1, const Point2& p2, const Point2& p3, const Point2& p4, const Color4D& c)
    {
        BezierCubicRz::draw(matrix_*p1, matrix_*p2, matrix_*p3, matrix_*p4, c, target_);
    }

    void ImageRasterizer::drawTriangle(
        const Point2& p0, const Point2& p1, const Point2& p2,
        const Color4D& c0, const Color4D& c1, const Color4D& c2)
    {
        TriangleRz::draw(p0, p1, p2, c0, c1, c2, target_);
    }

    void ImageRasterizer::save() {
        stack_.push(matrix_);
    }

    void ImageRasterizer::restore() {
        if (!stack_.empty()) {
            matrix_ = stack_.top();
            stack_.pop();
        }
    }

    void ImageRasterizer::translate(double dx, double dy) {
        matrix_ = matrix_ * Matrix3x3::translate(dx, dy);
    }

}