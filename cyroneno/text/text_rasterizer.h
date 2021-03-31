// Copyright (c) 2019 ucclkp <ucclkp@gmail.com>.
// This file is part of cyroneno project.
//
// This program is licensed under GPLv3 license that can be
// found in the LICENSE file.

#ifndef CYRONENO_TEXT_TEXT_RASTERIZER_H_
#define CYRONENO_TEXT_TEXT_RASTERIZER_H_

#include <string>
#include <fstream>

#include "../rasterizer/rasterizer.h"
#include "opentype/opentype_font.h"


namespace cyro {

    class TextRasterizer {
    public:
        TextRasterizer() = default;

        bool initialize();

        bool measureText(
            const std::u16string& str, int dpi, int size,
            int* width, int* height, std::vector<otf::OpenTypeFont::GlyphInfo>* glyphs);

        bool drawText(
            Rasterizer& rz, int width, int height,
            const std::vector<otf::OpenTypeFont::GlyphInfo>& glyphs,
            int dpi, int size, bool outline, int x, int y);

    private:
        std::ifstream file_;
        otf::OpenTypeFont font_;
    };

}

#endif  // CYRONENO_TEXT_TEXT_RASTERIZER_H_