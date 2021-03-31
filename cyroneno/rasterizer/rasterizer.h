// Copyright (c) 2019 ucclkp <ucclkp@gmail.com>.
// This file is part of cyroneno project.
//
// This program is licensed under GPLv3 license that can be
// found in the LICENSE file.

#ifndef CYRONENO_RASTERIZER_RASTERIZER_H_
#define CYRONENO_RASTERIZER_RASTERIZER_H_

#include "../color.hpp"
#include "../point.hpp"


namespace cyro {

    class Rasterizer {
    public:
        virtual ~Rasterizer() = default;

        virtual void setAntiAlias(bool enabled) = 0;

        virtual void save() = 0;
        virtual void restore() = 0;
        virtual void translate(double dx, double dy) = 0;

        virtual void drawPoint(const Point2& p, const Color4D& c) = 0;

        virtual void drawLine(
            const Point2& p0, const Point2& p1,
            double thick, int thick_type, const Color4D& c) = 0;
        virtual void drawLineSeg(
            const Point2& p0, const Point2& p1, const Color4D& c) = 0;

        virtual void drawCircle(
            const Point2& center, double r,
            double thick, int thick_type, const Color4D& c) = 0;
        virtual void drawEllipse(
            const Point2& center, double a, double b, const Color4D& c) = 0;

        virtual void drawQuadBezier(
            const Point2& p1, const Point2& p2, const Point2& p3, const Color4D& c) = 0;
        virtual void drawQuadBezier(
            const Point2& p1, const Point2& p2, const Point2& p3,
            double w0, double w1, double w2,
            const Color4D& c) = 0;

        virtual void drawCubicBezier(
            const Point2& p1, const Point2& p2, const Point2& p3, const Point2& p4, const Color4D& c) = 0;

        virtual void drawTriangle(
            const Point2& p0, const Point2& p1, const Point2& p2,
            const Color4D& c0, const Color4D& c1, const Color4D& c2) = 0;
    };

}

#endif  // CYRONENO_RASTERIZER_RASTERIZER_H_