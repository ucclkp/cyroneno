// Copyright (c) 2019 ucclkp <ucclkp@gmail.com>.
// This file is part of cyroneno project.
//
// This program is licensed under GPLv3 license that can be
// found in the LICENSE file.

#ifndef CYRONENO_TEXT_OPENTYPE_OPENTYPE_RASTERIZER_H_
#define CYRONENO_TEXT_OPENTYPE_OPENTYPE_RASTERIZER_H_

#include "cyroneno/rasterizer/rasterizer.h"
#include "cyroneno/rasterizer/glyph_rz.h"
#include "cyroneno/text/opentype/opentype_font.h"


namespace cyro {
namespace otf {

    /**
     * 参考: https://developer.apple.com/fonts/TrueType-Reference-Manual/RM02/Chap2.html#distinguishing
     */
    class OpenTypeRasterizer {
    public:
        static void drawGlyph(
            Rasterizer& rz, int width, int height,
            const OpenTypeFont::GlyphInfo& glyph,
            int off_x, int off_y, double scale, const Color4D& c, bool use_glyph_rz);

        using Func = void(
            bool new_contour, bool is_line,
            const Point2& sp, const Point2& mp, const Point2& ep);

        static void drawGlyph2(
            const OpenTypeFont::GlyphInfo& glyph,
            int off_x, int off_y, double scale, Func func);
    };

}
}

#endif  // CYRONENO_TEXT_OPENTYPE_OPENTYPE_RASTERIZER_H_