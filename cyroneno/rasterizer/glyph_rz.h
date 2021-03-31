// Copyright (c) 2019 ucclkp <ucclkp@gmail.com>.
// This file is part of cyroneno project.
//
// This program is licensed under GPLv3 license that can be
// found in the LICENSE file.

#ifndef CYRONENO_RASTERIZER_GLYPH_RZ_H_
#define CYRONENO_RASTERIZER_GLYPH_RZ_H_

#include <vector>

#include "../point.hpp"


namespace cyro {

    class Rasterizer;

    class GlyphRz {
    public:
        struct Part {
            bool is_bezier;
            Point2 s, m, e;
        };

        GlyphRz();

        void draw(int w, int h, Rasterizer& rz);

        void addLine(const Point2& s, const Point2& e);
        void addQBezier(const Point2& s, const Point2& m, const Point2& e);
        void newContour();

        bool hitTest(double x, double y);

    private:
        enum Into {
            INTO_IN,
            INTO_OUT,
            INTO_UNKNOWN,
        };

        struct Contour {
            std::vector<Part> parts;
        };

        void drawHScanline(double y, int w, Rasterizer& rz);

        bool isInner(double x, double y);

        static bool intersectH(double y, const Part& part, double* x);
        static Into getInto(const Part& part);

        std::vector<Contour> contours_;
    };

}

#endif  // CYRONENO_RASTERIZER_GLYPH_RZ_H_